/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 11:19:17 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/11 11:36:40 by wbelfatm         ###   ########.fr       */
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
	std::cout << this->statusCode << std::endl;
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

	if (this->statusCode != SUCCESS)
		return fullPath;

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

		if (autoIndex) {
			fullPath += "index.html";
		}
	}
	this->setPath(fullPath);
	this->checkPath();
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

std::string readFileContent( std::ifstream file ) {
	char buf[1025];
	file.read(buf, 1024);
	buf[file.gcount()] = 0;
	std::string content(buf);
	return content;
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
		std::cout << "uri: " << uri << std::endl;
		if (uri.length() == 0 || uri == "/") {
			res += "<li><a href=\"/" + tmp + "\">";
		}
		else
			res += "<li><a href=\"" + uri + "/" + tmp + "\">";
		res += tmp;
		res += "</a></li>";
		entry = readdir(dir);
	}
	return res + "</ul>";
}

std::string getDateReaponse(){
	char buffer[100];
	time_t rawtime;
	time(&rawtime);

	struct tm*  Info = gmtime(&rawtime);
	std::strftime(buffer, 100, "%a %d %b %Y %H:%M:%S GMT", Info);
	return ((std::string) buffer);
}

std::string Response::constructHeader( void ) {
	// todo: create res header
	
	std::string header = "HTTP/1.1 ";

	header += std::to_string(this->statusCode) + " " + this->getStatusText() + "\r\n";
	header += "Content-Length: 0\r\n";
	header += "Content-Type: " + this->contentType + "\r\n";
	header += "Date: " + getDateReaponse() + "\r\n";
	header += "\r\n";
	std::cout << header << std::endl;
	return header;
}

std::string Response::createGetResponse( void ) {
	// todo: add response header generator
	this->status = FINISHED;
	// return this->constructHeader();

 	// todo: check method is allowed

	std::string path = this->client->getRequest().getUri();

	try {
		this->getFullPath(path);
	} catch (ResponseException e) {
		std::cout << "Something went wrong with response: " << e.what() << std::endl;
		// this->body = "" + e.what();
	}

	return this->constructHeader();
	
	// struct stat fileStat;

	// // todo: get request path and check permissions
	// if (this->status == IDLE) {
	// 	if (this->checkPath()) {
	// 		stat(fullPath.data(), &fileStat);

	// 		if (S_ISDIR(fileStat.st_mode)) {
	// 			content = getDirectoryLinks(fullPath, path);
	// 			this->status = FINISHED;
	// 			return (httpCode + "Content-length: " + std::to_string(content.length()) + restHeader + content);
	// 		}
	// 		else {
	// 			this->status = ONGOING;
	// 			return (httpCode + "Content-length: " + std::to_string(fileStat.st_size) + restHeader);
	// 		}
	// 	}
	// 	else {
	// 		this->status = FINISHED;
	// 		content = "Page not found on " + path;
	// 		httpCode = "HTTP/1.1 404 Not Found\r\n";
	// 		return (httpCode + "Content-length: " + std::to_string(content.length()) + restHeader + content);
	// 	}
	// }
	// if (!this->file.is_open()) {
	// 	this->file.open(fullPath);
	// 	// todo: check if open failed
	// }

	// //todo: extract file reading logic
	// // read from file
	// char buf[1025];
	// file.read(buf, 1024);
	// buf[file.gcount()] = 0;
	// std::string con(buf);

	// if (con.length() != 1024) {
	// 	this->status = FINISHED;
	// 	this->file.close();
	// }
	// return con;
}

Response::~Response(){
}

const char* Response::ResponseException::what() const throw() {
    return message.c_str();
};

Response::ResponseException::ResponseException(std::string msg): message(msg) {};

Response::ResponseException::~ResponseException() throw() {};