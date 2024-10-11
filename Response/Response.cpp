/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 11:19:17 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/11 18:19:36 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void initializeStatusTexts(std::map<int, std::string>& statusTexts) {
	// 1xx
	statusTexts[CONTINUE] = "Continue";
	statusTexts[SWITCHING_PROTOCOLS] = "Switching Protocols";

	// 2xx
	statusTexts[SUCCESS] = "OK";
	statusTexts[CREATED] = "Created";
	statusTexts[ACCEPTED] = "Accepted";
	statusTexts[NO_CONTENT] = "No Content";

	// 4xx
	statusTexts[BAD_REQUEST] = "Bad Request";
	statusTexts[UNAUTHORIZED] = "Unauthorized";
	statusTexts[FORBIDDEN] = "Forbidden";
	statusTexts[NOT_FOUND] = "Not Found";
	statusTexts[METHOD_NOT_ALLOWED] = "Method Not Allowed";
	statusTexts[NOT_ACCEPTABLE] = "Not Acceptable";
	statusTexts[REQUEST_TIMEOUT] = "Request Timeout";
	
	// 5xx
	statusTexts[INTERNAL_SERVER_ERROR] = "Internal Server Error";
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
	this->status = IDLE;
	initializeStatusTexts(this->statusTexts);
	this->location = NULL;
	this->body = "";
	this->contentLength = 0;
}



void    Response::setStatusLine(std::string _Status){
	this->StatusLine = _Status;
}

std::string Response::getStatusLine(){
	return (this->StatusLine);
}

int Response::getStatusCode(){
	return (this->statusCode);
}

void Response::setStatusCode(int statusCode){
	// std::cout << this->statusCode << std::endl;
	this->statusCode = statusCode;
}

void Response::setMap(std::string _name, std::string _Value){
	this->ResponseMeth[_name] = _Value;
}

Response::Response(const Response &Obj){
	this->mimeTypes = Obj.mimeTypes;
	this->ResponseMeth = Obj.ResponseMeth;
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

	if (locations.find(path) == locations.end()) 
		root = server.getField("root").getValues()[0];
	else
	{
		location = &locations[path];
		root = locations[path].getField("root").getValues()[0];
	}
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
		// is dir
		if (location) {
			// path matches a location
			autoIndex = location->getField("autoindex").getValues()[0] == "on";
		}
		else {
			autoIndex = server.getField("autoindex").getValues()[0] == "on";
		}

		if (autoIndex)
			fullPath += "index.html";
	}
	this->setPath(fullPath);
	this->checkPath();
	stat(fullPath.data(), &fileStat);
	if (!S_ISDIR(fileStat.st_mode)) {
		this->contentLength = fileStat.st_size;
		this->extractFileName();
	}
	else {
		this->fileName = "";
	}
	std::cout << "Filename : " << this->fileName << std::endl;
	return fullPath;
}

bool Response::checkPath( void ) {
	std::cout << "Searching on: " << this->path << std::endl;

	if (access(this->path.data(), F_OK) != 0) {

		// doesn't exist
		this->statusCode = NOT_FOUND;
		throw ResponseException("Page Not Found");
	}
	if (access(this->path.data(), R_OK) != 0) {
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
	int start = path.length() - 1;

	for (int i = start; i >= 0; i--) {
		if (path[i] == '/') {
			start = i + 1;
			break ;
		}
	}
	this->fileName = path.substr(start, path.length());
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

	header += std::to_string(this->statusCode) + " " + this->getStatusText() + "\r\n";
	header += "Content-Length: " + std::to_string(this->contentLength) + "\r\n";
	header += "Content-Type: " + this->contentType + "\r\n";
	header += "Date: " + getDateResponse() + "\r\n";
	header += "\r\n";

	if (this->body != "") {
		header += body;
	}
	std::cout << header << std::endl;
	return header;
}

std::string Response::getFileChunk( void ) {
	char buf[1025];

	if (!this->file.is_open()) {
		this->file.open(this->path);
		
		if (!this->file.is_open())
		{
			this->status = FINISHED;
			throw ResponseException("Couldn't open file");
		}
	}

	file.read(buf, 1024);
	buf[file.gcount()] = 0;
	std::string chunk(buf);

	if (chunk.length() != 1024) {
		this->status = FINISHED;
		this->file.close();
	}
	return chunk;
}

bool Response::checkAllowedMethod( std::string path ) {
	ServerNode &server = this->client->getParentServer();
	std::map<std::string, Location> locations = server.getLocations();
	Location *location = NULL;
	std::string method = this->client->getRequest().getmethode();
	std::vector<std::string> allowedMethods;

	if (locations.find(path) != locations.end()) {
		location = &locations[path];
	}

	if (!location)
		return true;

	allowedMethods = location->getField("limit_except").getValues();

	std::cout << method << " " << allowedMethods.size() << std::endl;
	if (allowedMethods.size() == 0)
		return true;
	if (std::find(allowedMethods.begin(), allowedMethods.end(), method) == allowedMethods.end()) {
		this->statusCode = METHOD_NOT_ALLOWED;
		throw ResponseException("Method not allowed");
	}
	return true;
}


std::string Response::createGetResponse( void ) {
 	// todo: check method is allowed

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
			if (this->fileName == "") {
				this->body = getDirectoryLinks(this->path, path);
				this->contentLength = body.length();
				this->status = FINISHED;
			}
			else {
				this->status = ONGOING;
				this->body = "";
			}

		}
		else {
			// read from file
			chunk = this->getFileChunk();
			return chunk;
		}

	} catch (ResponseException e) {
		this->status = FINISHED;
		std::cout << "Something went wrong with response: " << e.what() << std::endl;
	}

	return this->constructHeader();
}

Response::~Response(){
}

const char* Response::ResponseException::what() const throw() {
    return message.c_str();
};

Response::ResponseException::ResponseException(std::string msg): message(msg) {};

Response::ResponseException::~ResponseException() throw() {};