/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 11:19:17 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/08 14:00:56 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::setStatusMaps(){
	this->statusMaps[200] = "OK";
	this->statusMaps[201] = "Created";
	this->statusMaps[400] = "Bad Request";
	this->statusMaps[401] = "Unauthorized";
	this->statusMaps[403] = "Forbidden";
	this->statusMaps[404] = "Not Found";
	this->statusMaps[204] = "No Content";
	this->statusMaps[405] = "Method Not Allowed";
	this->statusMaps[413] = "Payload too Large";
	this->statusMaps[414] = "URI Too Long";
	this->statusMaps[415] = "Unsupported Media Type";
	this->statusMaps[416] = "Range Not Satisfiable";
	this->statusMaps[501] = "Not Implemented";
	this->statusMaps[505] = "HTTP Version Not Supported";
	this->statusMaps[422] = "Unprocessable Entity";
}


Response::Response(){
	this->statusCode = 200;
	this->statusLine = "";
	this->bodyResponse = "";
	this->setStatusMaps();
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
		return (this->MIMeType.at(_Key));
}

Response::~Response(){
}