/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 11:19:17 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/17 14:35:20 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "../parsing/Parser.hpp"

void initializeStatusTexts(std::map<int, std::string>& statusTexts) {
	// 1xx
	statusTexts[CONTINUE] = "Continue";
	statusTexts[SWITCHING_PROTOCOLS] = "Switching Protocols";

	// 2xx
	statusTexts[SUCCESS] = "OK";
	statusTexts[CREATED] = "Created";
	statusTexts[ACCEPTED] = "Accepted";
	statusTexts[NO_CONTENT] = "No Content";
	statusTexts[PARTIAL_CONTENT] = "Partial Content";

	// 30x
	statusTexts[NOT_MODIFIED] = "Not Modified";

	// 4xx
	statusTexts[BAD_REQUEST] = "Bad Request";
	statusTexts[UNAUTHORIZED] = "Unauthorized";
	statusTexts[FORBIDDEN] = "Forbidden";
	statusTexts[NOT_FOUND] = "Not Found";
	statusTexts[METHOD_NOT_ALLOWED] = "Method Not Allowed";
	statusTexts[NOT_ACCEPTABLE] = "Not Acceptable";
	statusTexts[REQUEST_TIMEOUT] = "Request Timeout";
	statusTexts[413] = "Payload too Large";
	statusTexts[414] = "URI Too Long";
	statusTexts[415] = "Unsupported Media Type";
	statusTexts[416] = "Range Not Satisfiable";
	statusTexts[422] = "Unprocessable Entity";

	// 5xx
	statusTexts[INTERNAL_SERVER_ERROR] = "Internal Server Error";
	statusTexts[501] = "Not Implemented";
	statusTexts[505] = "HTTP Version Not Supported";
}

Response::Response(){
	this->statusCode = 200;
	this->contentType = "text/html; charset=UTF-8";
	this->mimeTypes[".html"] = "text/html; charset=UTF-8";
	this->mimeTypes[".css"] = "text/css";
	this->mimeTypes[".js"] = "application/javascript";
	this->mimeTypes[".jpg"] = "image/jpeg";
	this->mimeTypes[".jpeg"] = "image/jpeg";
	this->mimeTypes[".png"] = "image/png";
	this->mimeTypes[".gif"] = "image/gif";
	this->mimeTypes[".json"] = "application/json";
	this->mimeTypes[".xml"] = "application/xml";
	this->mimeTypes[".txt"] = "text/plain";
	this->mimeTypes[".mp4"] = "video/mp4";
	this->mimeTypes[".pdf"] = "application/pdf";
	this->status = IDLE;
	initializeStatusTexts(this->statusTexts);
	this->location = NULL;
	this->body = "";
	this->contentLength = 0;
	this->bytesSent = 0;
	this->rangeStart = 0;
	this->isError = false;
	this->isBody = false;
}


int Response::getStatusCode(){
	return (this->statusCode);
}

void Response::setStatusCode(int statusCode){
	this->statusCode = statusCode;
}

void Response::setMap(std::string _name, std::string _Value){
	this->ResponseMeth[_name] = _Value;
}

Response& Response::operator=(const Response &Obj){
	if (this != &Obj){
		this->ResponseMeth = Obj.ResponseMeth;
		this->mimeTypes = Obj.mimeTypes;
	}
	return (*this);
}

std::map<std::string, std::string> Response::getMap() const {
	return (this->ResponseMeth);
}

std::string Response::getMimeType(std::string _Key){
	try{
		return (this->mimeTypes.at(_Key));
	} catch (const std::out_of_range& e) {
		std::cerr << "Expection: Key not found" << e.what() << std::endl;
		return "";
	}
}

Client* Response::getClient( void ) {
	return this->client;
}

void Response::setClient( Client *client ) {
	this->client = client;
}

std::string Response::getFullPath( std::string path ) {
	ServerNode &server = this->client->getParentServer();
	std::map<std::string, Location> locations = server.getLocations();
	Location *location = NULL; 
	std::string root;
	std::string fullPath;
	struct stat fileStat;
	bool autoIndex = true;

	location = this->getPathLocation(path);

	if (!location) 
		root = server.getField("root").getValues()[0];
	else
	{
		this->location = location;
		root = location->getField("root").getValues()[0];
	}

	this->setPath(root);
	this->checkPath();
	
	
	fullPath = root + path;

	std::cout << "FULL PATH: " << fullPath << std::endl;

	this->setPath(fullPath);
	this->checkPath();

	stat(fullPath.data(), &fileStat);

	/*
		check file type (dir/regular)
			if dir
				check autoindex, if so, append index.html to path
				otherwise return dirs listing
			if file, nothing to be done
	*/

	if (S_ISDIR(fileStat.st_mode)) {
		
		if (path[path.length() - 1] != '/')
			fullPath += "/";

		fullPath += "index.html";

		// is dir
		if (location) {
			// path matches a location
			autoIndex = location->getField("autoindex").getValues()[0] == "on";
		}
		else {
			autoIndex = server.getField("autoindex").getValues()[0] == "on";
		}

		try {
			this->setPath(fullPath);
			this->checkPath();
		} catch (Response::ResponseException e) {
			
			// index doesn't exist or not enough permissions
			if (autoIndex) {
				fullPath = root + path;
				this->setPath(fullPath);
			}
			else {
				this->setStatusCode(NOT_FOUND);
				throw Response::ResponseException("Not found");
			}
		}
		
	}
	stat(fullPath.data(), &fileStat);
	if (!S_ISDIR(fileStat.st_mode)) {
		this->contentLength = fileStat.st_size;
		this->extractFileName();
	}
	else {
		this->fileName = "";
	}
	return fullPath;
}

bool Response::checkPath( void ) {
	if (access(this->path.data(), F_OK) != 0) {

		// todo: check if location has permission
		

		// doesn't exist
		if (!this->isError)
			this->statusCode = NOT_FOUND;
		throw ResponseException("Page Not Found");
	}
	
	if (access(this->path.data(), R_OK) != 0) {
		if (!this->isError)
			this->statusCode = FORBIDDEN;
		throw ResponseException("Permission Denied");
		
	}

	return true;
}

std::string Response::getPath( void ) const {
	return this->path;
}

void Response::setPath( std::string path ) {
	this->path = path;
}

t_response_status Response::getStatus( void ) const {
	return this->status;
}

void Response::setStatus(t_response_status status) {
	this->status = status;
}

std::string Response::getBody( void ) const {
	return this->body;
}

void Response::setBody( std::string body ) {
	this->body = body;
}

std::string Response::getContentType( void ) const {
	return this->contentType;
}

void Response::setContentType( std::string contentType ) {
	this->contentType = contentType;
}

std::string Response::getStatusText( void ) {
	return this->statusTexts[this->statusCode];
}

void Response::extractFileName( void ) {
	std::string path = this->path;
	std::string extension;
	int start = path.length() - 1;

	for (int i = start; i >= 0; i--) {
		if (path[i] == '/') {
			start = i + 1;
			break ;
		}
	}
	this->fileName = path.substr(start, path.length());
	start = this->fileName.length() - 1;
	for (int i = start; i >= 0; i--) {
		if (this->fileName[i] == '.')
		{
			start = i;
			break;
		}
	}
	extension = this->fileName.substr(start, this->fileName.length());
	if (this->mimeTypes.find(extension) != this->mimeTypes.end())
		this->contentType = this->mimeTypes[extension];
}

std::string getDirectoryLinks(std::string path, std::string uri) {
	DIR *dir = opendir(path.data());
	struct dirent *entry;
	std::string res = "<ul>";
	std::string tmp;

	if (!dir)
		return "Error opening directory";
	entry = readdir(dir);
	while (entry) {
		tmp = entry->d_name;
		
		if (tmp[0] != '.') {
			if (uri.length() == 0 || uri == "/") {
				res += "<li><a href=\"/" + tmp + "\">";
			}
			else
				res += "<li><a href=\"" + uri + "/" + tmp + "\">";
			res += tmp;
			res += "</a></li>";
		}
		entry = readdir(dir);
	}
	return res + "</ul>";
}

std::string getDateResponse(){
	char buffer[100];
	time_t rawtime;
	time(&rawtime);

	struct tm*  Info = gmtime(&rawtime);
	std::strftime(buffer, 100, "%a %d %b %Y %H:%M:%S GMT", Info);
	return ((std::string) buffer);
}

std::string Response::constructHeader( void ) {
	std::string header = "HTTP/1.1 ";

	// if (this->isError && this->contentLength == 0) {
	// 	this->statusCode = NOT_FOUND;
	// }
	

	header += std::to_string(this->statusCode) + " " + this->getStatusText() + "\r\n";

	header += "Content-Type: " + this->contentType + "\r\n";
	header += "Connection: keep-alive\r\n";
	if (this->statusCode == PARTIAL_CONTENT)
	{
		unsigned long totalSize = this->rangeStart + this->contentLength;
		header += "Content-Range: bytes " + std::to_string(this->rangeStart)
		+ "-" + std::to_string(totalSize - 1) + "/" + std::to_string(totalSize) + "\r\n";
	}
	header += "Content-Length: " + std::to_string(this->contentLength) + "\r\n";
	header += "Accept-Ranges: bytes\r\n";
	header += "Date: " + getDateResponse() + "\r\n";
	header += "\r\n";

	if (this->contentLength == 0)
	{
		this->status = FINISHED;
	}

	// std::cout << "HEADER: \n" << header << std::endl;

	return header;
}

std::string Response::getFileChunk( void ) {
	std::vector<char> buff(CHUNK_SIZE + 1);

	if (!this->file.is_open()) {
		this->file.open(this->path, std::ios::binary);
		this->file.seekg(this->rangeStart, std::ios::beg);
		if (!this->file.is_open())
		{
			this->status = FINISHED;
			throw ResponseException("Couldn't open file");
		}
	}

	file.read(buff.data(), CHUNK_SIZE);
	
	std::streamsize bytesRead = this->file.gcount();
	if (bytesRead < 0)
	{
		std::cout << "ERROR READING FROM FILE" << std::endl;
		this->status = FINISHED;
	}

	std::string chunk(buff.data(), bytesRead);

	this->bytesSent += bytesRead;

	if (this->bytesSent >= this->contentLength) {
		this->status = FINISHED;
		// this->file.close();
	}
	return chunk;
}

bool Response::checkAllowedMethod( std::string path ) {
	ServerNode &server = this->client->getParentServer();
	std::map<std::string, Location> locations = server.getLocations();
	Location *location = NULL;
	std::string method = this->client->getRequest().getmethode();
	std::vector<std::string> allowedMethods;

	location = this->getPathLocation(path);

	if (!location)
		return true;

	allowedMethods = location->getField("limit_except").getValues();

	if (allowedMethods.size() == 0)
		return true;
	if (std::find(allowedMethods.begin(), allowedMethods.end(), method) == allowedMethods.end()) {
		this->statusCode = METHOD_NOT_ALLOWED;
		throw ResponseException("Method not allowed");
	}
	return true;
}

void Response::reset( void ) {
	std::cout << "\n\nRESET\n\n" << std::endl;
	this->contentLength = 0;
	this->bytesSent = 0;
	this->rangeStart = 0;
	this->contentType = "text/html; charset=UTF-8";
	this->fileName = "";
	this->statusCode = SUCCESS;
	this->status = IDLE;
	this->file.close();
	this->path = "";
	this->isError = false;
	this->isBody = false;
}

void Response::extractRange( void ) {
	std::string rangeHeader = this->client->getRequest().getValue("Range");
	std::string range;
	int end = rangeHeader.length() - 1;
	int start = end;

	if (rangeHeader.length() == 0)
		return ;

	for (int i = 0; i < (int) rangeHeader.length(); i++) {
		if (std::isdigit(rangeHeader[i]))
		{
			start = i;
			break ;
		}
	}
	
	if (start != end) {
		range = rangeHeader.substr(start, end - 1);
		
		range.erase(range.length() - 1);
		
		this->rangeStart = std::stoul(range);
	}
	this->statusCode = PARTIAL_CONTENT;
	if (this->contentLength < this->rangeStart) {
		exit(0);
	}
	this->contentLength -= this->rangeStart;
}

std::string Response::createGetResponse( void ) {
	std::string path = this->client->getRequest().getUri();
	std::string chunk;
	try {

		/* 
			only check full path is res on IDLE
			if not in on IDLE, read file and send
		*/

		if (this->status == IDLE) {
			this->checkAllowedMethod(path);
			this->getFullPath(path);

			// empty filename means it's a dir
			if (this->fileName == "") {
				this->body = getDirectoryLinks(this->path, path);

				this->contentLength = body.length();
				this->statusCode = SUCCESS;
				this->status = ONGOING;
				this->isBody = true;
				return constructHeader();
			}
			else {
				// this is a file, set status to ONGOING to start sending chunks
				this->status = ONGOING;
				this->body = "";

				// extract range from header
				this->extractRange();
			}
		}
		else {
			// read from file
			if (this->isBody) {
				chunk = this->body.substr(0, CHUNK_SIZE);
				if (this->body.length() >= CHUNK_SIZE) {
					this->body = this->body.substr(CHUNK_SIZE, this->body.length());
				}
				else
					this->status = FINISHED;
			}
			else 
				chunk = this->getFileChunk();
			
			return chunk;
		}
	} catch (ResponseException e) {
		std::cout << "Something went wrong with response: " << e.what() << std::endl;
		if (!this->isError)
		{
			this->status = IDLE;
			return this->getErrorResponse();
		}
		else
		{
			this->status = FINISHED;
			this->body = "<h1>" + std::to_string(this->statusCode) + " " + this->getStatusText() + "</h1>";
			this->contentLength = this->body.length();
			return this->constructHeader() + this->body;
		}
	}
	return this->constructHeader();
}

void printVector(std::vector<std::string> arr) {
	std::cout << "[";
	for (int i = 0; i < (int) arr.size(); i++) {
		std::cout << arr[i];

		if (!(i == (int) arr.size() - 1))
			std::cout << ", ";
	}
	std::cout << "]\n";
}

Location *Response::getPathLocation(std::string path) {
	ServerNode &server = this->client->getParentServer();
	std::map<std::string, Location> &locations = server.getLocations();
	std::map<std::string, Location>::iterator it;
	Location *location = NULL;
	std::string lastMatch = "";
	std::string root;

	for (it = locations.begin(); it != locations.end(); it++)
	{
		if (path.find(it->first) != path.npos) {
			// location match
			if (it->first.length() > lastMatch.length())
			{
				location = &it->second;
				lastMatch = it->first;
			}
		}
	}
	if (location) {
		
		// check location permission
		root = location->getField("root").getValues()[0];
		this->setPath(root + "/" + lastMatch);
		this->checkPath();
	}

	return location;
}

std::string Response::getErrorResponse( void ) {
	/* 
		steps:
			- look for error directive value
			- look for corresponding location
			- if no location found, use my default
	*/

	std::vector<std::string> errorField;
	std::vector<std::string> errorPages;
	std::string path = "";
	bool found = false;

	if (this->location)
		errorPages = this->location->getField("error_page").getValues();
	errorField = this->client->getParentServer().getField("error_page").getValues();
	errorPages.insert(errorPages.end(), errorField.begin(), errorField.end());
	
	for (int i = 0; i < (int) errorPages.size(); i++) {
		if (Parser::isNumber(errorPages[i]))
		{
			if (std::stoi(errorPages[i]) == this->statusCode)
				found = true;
		}
		else {
			if (found) {
				path = errorPages[i];
				break ;
			}
		}
	}
	if (path != "")
		this->client->getRequest().SetUri(path);
	this->isError = true;
	std::cout << "ERROR PATH: " << path << std::endl;

	return this->createGetResponse();
}

Response::~Response(){
}

const char* Response::ResponseException::what() const throw() {
    return message.c_str();
};

Response::ResponseException::ResponseException(std::string msg): message(msg) {};

Response::ResponseException::~ResponseException() throw() {};