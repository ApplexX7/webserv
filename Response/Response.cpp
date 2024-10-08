/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 11:19:17 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/08 16:28:15 by wbelfatm         ###   ########.fr       */
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
	} catch (const std::out_of_range& e){
		std::cerr << "Expection: Key not found" << e.what() << std::endl;
		return "";
	}
}

std::string Response::createGetResponse( void ) {
	return "HTTP/1.1 404 Not Found\r\n\
Content-Length: 11\r\n\
Content-Type: text/html\r\n\
Last-Modified: Wed, 12 Aug 1998 15:03:50 GMT\r\n\
Accept-Ranges: bytes\r\n\
ETag: \"04f97692cbd1:377\"\r\n\
Date: Thu, 19 Jun 2008 19:29:07 GMT\r\n\
\r\n\
hello there";
}

Response::~Response(){
}