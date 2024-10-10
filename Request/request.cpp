/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:28:36 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/10 17:30:06 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(){
}

Request::Request(const Request &obj){
	*this = obj;
}

Request &Request::operator=(const Request &ope){
	if (this != &ope){
		this->methode = ope.methode;
		this->Uri = ope.Uri;
	}
	return (*this);
}

void Request::setHeaders(std::string &name, std::string &value){
	if (!name.empty() && !value.empty()) {
		this->headers[name] = value;
	} else {
		std::cerr << "Error: Header name or value is empty!" << std::endl;
	}
	// std:: cout << "******"<< "*****" << std::endl;
	// std::cout << name << " : " << value << std::endl;
	// std:: cout << "******"<< "*****" << std::endl;
}

void Request::setBody(std::string _Body){
	this->body = _Body;
}

void Request::printmap(){
	// std::cout << "HEADER  :" << std::endl;
	// for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it){
	// 	std::cout << it->first << " : " << it->second << std::endl;
	// }
}

int Request::CheckserverhostName(Client & ClientData){
    size_t i;
    //find the listenning server
    for (i = 0; i < ClientData.getServers().size(); i++){
        if (ClientData.getServers()[i]->getListenField() == ClientData.getRequest().getport()){
            for(size_t j = 0; j < ClientData.getServers()[i]->getFields()["server_name"].getValues().size(); j++){
                if (ClientData.getServers()[i]->getFields()["server_name"].getValues()[j] ==  ClientData.getRequest().gethostName()){
                    ClientData.getRequest().setserverNode(ClientData.getServers()[i]);
                    return (0);
                }
            }
            ClientData.getRequest().setserverNode(ClientData.getServers()[0]);
			return (0);
        }
    }
	return 1;
}

std::string Request::getpathName( void ){
	return (this->pathName);
}

void Request::setpathName(std::string _Name){
	this->pathName = _Name;
}

int Request::Validmethode(std::string &_Methode){
	if (_Methode == "GET"){
		this->methode = _Methode;
	}
	else if (_Methode == "POST"){
		this->methode = _Methode;
	}
	else if (_Methode == "DELETE"){
		this->methode = _Methode;
	}
	else
		return (1);
	return (0);
}

int Request::ParseRequestLine(std::string &RqLine, Client &ClientData){
	std::vector<std::string> RequestLineChunks;
	std::stringstream str(RqLine);
	std::string token;
	size_t pos;

	if (RqLine.empty()){
		ClientData.getResponse().SetStatusCode(400);
		return (1);
	}
	if (std::count(RqLine.begin(), RqLine.end(), ' ') != 2){
		ClientData.getResponse().SetStatusCode(400);
		return (1);
	}
	while (std::getline(str, token, ' ')){
		RequestLineChunks.push_back(token);
	}
	if (RequestLineChunks.size() != 3){
		ClientData.getResponse().SetStatusCode(400);
		return (1);
	}
	for (size_t i = 0; i < RequestLineChunks.size(); i++){
		if (RequestLineChunks[i].empty()){
			ClientData.getResponse().SetStatusCode(400);
			return (1);
		}
	}
	//valide methode;
	if (this->Validmethode(RequestLineChunks[0])){
		ClientData.getResponse().SetStatusCode(501);
		return (1);
	}
	// check the Uri
	pos = RequestLineChunks[1].find("?");
	if (pos != std::string::npos){
		this->Uri = RequestLineChunks[1].substr(0, pos);
		this->quertyString = RequestLineChunks[1].substr(pos + 1);
	}
	else{
		this->Uri = RequestLineChunks[1];
	}
	// checkk HTTPS VERSION;
	if (RequestLineChunks[2] != "HTTP/1.1"){
		ClientData.getResponse().SetStatusCode(505);
		return (1);
	}
	return (0);
}

int Request::findLocationtobeUsed(){
	std::string LongestMatch = "";
	for (std::map<std::string, Location>::iterator it = this->server->getLocations().begin();
		it != this->server->getLocations().end(); it++){

			if (this->getUri().find(it->first) == 0){
				if (it->first.length() > LongestMatch.length()){
					LongestMatch = it->first;
			}
		}
	}
	if (!LongestMatch.empty()){
		this->locationName = LongestMatch;
		return (0);
	}
	this->locationName = "";
	return (1);
}

std::string Request::FindHost(std::string HostLine){
	size_t pos;
	std::string Host;

	pos = HostLine.find("Host: ");
	if (pos != std::string::npos){
		Host = HostLine.substr(pos + 1);
		Host.erase(std::remove_if(Host.begin(), Host.end(), ::isspace), Host.end());
		pos = Host.find(":");
		if (pos != std::string::npos){
			Host = Host.substr(0, pos);
			this->port = Host.substr(pos + 1);
		}
		else
			this->port = "DEFAULT"; // temporary;
		return (Host);
	}
	return ("");
}

int Request::ParsingTheRequest(Client &ClientData){
	size_t pos;
	std::string name;
	std::string Value;
	std::string ChunkLine;
	std::stringstream Message(ClientData.getMessage());

	std::getline(Message, ChunkLine, '\r');
	Message.ignore(1);
	if (this->ParseRequestLine(ChunkLine, ClientData)){
		return (1);
	}
	// headers parsing;
	while (std::getline(Message, ChunkLine)){
		if (ChunkLine == "\r")
			break;
		pos = ChunkLine.find(':');
		if (pos == std::string::npos){
			ClientData.getResponse().SetStatusCode(400);
			return 0;
		}
		name = ChunkLine.substr(0, pos);
		Value = ChunkLine.substr(pos + 1);
		size_t spa = name.find(' ');
		size_t tab = name.find('\t');
		if (spa != std::string::npos || tab != std::string::npos){
			ClientData.getResponse().SetStatusCode(400);
			return (0);
		}
		Value.erase(std::remove_if(Value.begin(),Value.end(), ::isspace), Value.end());
		if (Value.empty() || name.empty()){
			ClientData.getResponse().SetStatusCode(400);
		}
		this->setHeaders(name, Value);
	}
	if (this->methode == "GET"){
		// this->CheckDirectory(ClientData);
		//pass to the  getresponse
		return (0);
	}
	else if (this->methode == "POST"){
		this->ParsePostHeaders();
	}
	// body part
	return 0;
}

void Request::setserverNode(ServerNode *_ServerNode){
	this->server = _ServerNode;
}

ServerNode& Request::getserverNode() const{
	return *this->server;
}

void Request::setport(std::string _Port){
	this->port =  _Port;
}

std::string Request::getport(){
	return (this->port);
}

std::string Request::gethostName(){
	return (this->hostName);
}

void Request::sethostName(std::string _HostName){
	this->hostName = _HostName;
}

std::string Request::getValue(std::string _Key){
	std::map<std::string, std::string>::iterator it = this->headers.find(_Key);
	if (it != this->headers.end()){
		return (this->headers[_Key]);
	}
	else
		return ("");
}

std::string Request::getBody(){
	return (this->body);
}

void Request::Setmethode(std::string _methode){
	this->methode = _methode;
}
std::string Request::getmethode(){
	return (this->methode);
}

std::string Request::getUri(){
	return (this->Uri);
}

void Request::SetUri(std::string _Uri){
	size_t pos;
	pos = _Uri.find("?");
	if (pos == std::string::npos){
		pos = _Uri.find("#");
		if (pos != std::string::npos){
			this->Uri = Uri.substr(0, pos);
		}
		else{
			this->Uri = _Uri;
		}
	}
	else{
		this->Uri = _Uri.substr(0, pos);
		this->quertyString = _Uri.substr(pos + 1);
	}
}

std::map<std::string,std::string> Request::getHeaders( void ) {
	return this->headers;
}

Request::~Request() {}