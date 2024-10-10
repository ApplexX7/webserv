/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 11:19:17 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/10 13:53:55 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"


Response::Response(){
	this->StatusCode = 200;
	this->MIMeType[".html"] = "text/html; charset=UTF-8";
	this->MIMeType[".css"] = "text/css";
	this->MIMeType[".js"] = "application/javascript";
	this->MIMeType[".jpg"] = "image/jpeg";
	this->MIMeType[".jpeg"] = "image/jpeg";
	this->MIMeType[".png"] = "image/png";
	this->MIMeType[".gif"] = "image/gif";
	this->MIMeType[".json"] = "application/json";
	this->MIMeType[".xml"] = "application/xml";
	this->MIMeType[".txt"] = "text/plain";
	this->status = IDLE;
}



void    Response::setStatusLine(std::string _Status){
	this->StatusLine = _Status;
}

std::string Response::getStatusLine(){
	return (this->StatusLine);
}

int Response::GetStatusCode(){
	return (this->StatusCode);
}

void Response::SetStatusCode(int _StatusCode){
	std::cout << this->StatusCode << std::endl;
	this->StatusCode = _StatusCode;
}

void Response::setMap(std::string _name, std::string _Value){
	this->ResponseMeth[_name] = _Value;
}

Response::Response(const Response &Obj){
	this->MIMeType = Obj.MIMeType;
	this->ResponseMeth = Obj.ResponseMeth;
}

Response& Response::operator=(const Response &Obj){
	if (this != &Obj){
		this->ResponseMeth = Obj.ResponseMeth;
		this->MIMeType = Obj.MIMeType;
	}
	return (*this);
}

std::map<std::string, std::string> Response::getMap() const {
	return (this->ResponseMeth);
}

std::string Response::getMIMeType(std::string _Key){
	try{
		return (this->MIMeType.at(_Key));
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

std::string Response::getFullPath( std::string path ) const {
	ServerNode &server = this->client->getParentServer();
	std::map<std::string, Location> locations = server.getLocations();
	std::string root;
	std::string fullPath;

	if (locations.find(path) == locations.end()) 
		root = server.getField("root").getValues()[0];
	else
		root = locations[path].getField("root").getValues()[0];
	fullPath = root + path;
	return fullPath;
}

bool Response::checkPath( std::string path ) const {
	std::cout << "Searching on: " << path << std::endl;

	if (access(path.data(), F_OK) == 0) {
		// file exists
		return true;
	}
	else {
		// doesn't exist
		return false;
	}
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

std::string Response::createGetResponse( void ) {
	std::string httpCode = "HTTP/1.1 200 Success\r\n";
	std::string restHeader = "\r\n\
Content-Type: text/html\r\n\
Last-Modified: Wed, 12 Aug 1998 15:03:50 GMT\r\n\
Accept-Ranges: bytes\r\n\
ETag: \"04f97692cbd1:377\"\r\n\
Date: Thu, 19 Jun 2008 19:29:07 GMT\r\n\
\r\n";

	std::string content = "";
	std::string path = this->client->getRequest().getUri();
	std::string fullPath = this->getFullPath(path);
	struct stat fileStat;

	// todo: get request path and check permissions
	std::cout << "\npath: " << path << std::endl;

	if (this->status == IDLE) {
		if (this->checkPath(fullPath)) {
			stat(fullPath.data(), &fileStat);
			
			if (S_ISDIR(fileStat.st_mode)) {
				// todo: read directory content and provide links for inner files/dirs

				content = "This is a directory";
				this->status = FINISHED;
				return (httpCode + "Content-length: " + std::to_string(content.length()) + restHeader + content);
			}
			else {
				this->status = ONGOING;
				return (httpCode + "Content-length: " + std::to_string(fileStat.st_size) + restHeader);
			}
		}
		else {
			this->status = FINISHED;
			content = "Page not found on " + path;
			httpCode = "HTTP/1.1 404 Not Found\r\n";
			return (httpCode + "Content-length: " + std::to_string(content.length()) + restHeader + content);
		}
	}
	if (!this->file.is_open()) {
		this->file.open(fullPath);
		// todo: check if open failed
	}

	// read from file
	char buf[1025];
	file.read(buf, 1024);
	buf[file.gcount()] = 0;
	std::string con(buf);

	
	if (con.length() != 1024) {
		this->status = FINISHED;
		this->file.close();
	}
	return con;
}

Response::~Response(){
}