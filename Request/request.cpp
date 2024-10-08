/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:28:36 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/08 10:28:30 by mohilali         ###   ########.fr       */
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
}

void Request::setBody(std::string _Body){
	this->body = _Body;
}

void Request::printmap(){
	std::cout << "HEADER  :" << std::endl;
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it){
		std::cout << it->first << " : " << it->second << std::endl;
	}
}

int Request::CheckServerHostName(Client & ClientData){
    size_t i;
    //find the listenning server
    for (i = 0; i < ClientData.getServers().size(); i++){
        if (ClientData.getServers()[i]->getListenField() == ClientData.getRequest().getPort()){
            for(size_t j = 0; j < ClientData.getServers()[i]->getFields()["server_name"].getValues().size(); j++){
                if (ClientData.getServers()[i]->getFields()["server_name"].getValues()[j] ==  ClientData.getRequest().getHostName()){
                    ClientData.getRequest().setServerNode(ClientData.getServers()[i]);
                    return (0);
                }
            }
            ClientData.getRequest().setServerNode(ClientData.getServers()[0]);
			return (0);
        }
    }
	return 1;
}

std::string Request::getPathName( void ){
	return (this->PathName);
}

void Request::setPathName(std::string _Name){
	this->PathName = _Name;
}

int Request::ValidMethode(std::string &_Methode){
	if (_Methode == "GET"){
		this->Methode = _Methode;
	}
	else if (_Methode == "POST"){
		this->Methode = _Methode;
	}
	else if (_Methode == "DELETE"){
		this->Methode = _Methode;
	}
	else
		return (1);
	return (0);
}

int Request::ParseRequestLine(std::string &RqLine, Client &ClientData){
	std::vector<strign> RequestLineChunks;
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
	for (int i = 0; i < RequestLineChunks.size(); i++){
		if (RequestLineChunks[i].empty()){
			ClientData.getResponse().SetStatusCode(400);
			return (1);
		}
	}
	//valide methode;
	if (this->ValidMethode(RequestLineChunks[0])){
		ClientData.getResponse().SetStatusCode(501);
		return (1);
	}
	// check the Uri
	pos = RequestLineChunks[1].find("?");
	if (pos != std::string::npos){
		this->Uri = RequestLineChunks[1].substr(0, pos);
		this->Querystring = RequestLineChunks[1].substr(pos + 1);
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

int Request::FindLocationtobeUsed(){
	std::string LongestMatch = "";
	for (std::map<std::string, Location>::iterator it = this->Server->getLocations().begin;
		it != this->Server->getLocations.end(); it++){

			if (this->getUri().find(it->first) == 0){
				if (it->first.length() > LongestMatch.length()){
					LongestMatch = it->first;
			}
		}
	}
	if (!LongestMatch.empty()){
		this->LocationName = LongestMatch;
		return (0);
	}
	this->LocationName = "";
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
			this->Port = Host.substr(pos + 1);
		}
		else
			this->Port = "DEFAULT"; // temporary;
		return (Host);
	}
	return ("");
}

int Request::ParsingTheRequest(Client &ClientData){	
	size_t pos;
	std::string Host;
	std::stringstream Message(ClientData.getMessage());
	std::string ChunkLine;

	std::getline(Message, ChunkLine, '\r');
	Message.ignore(1);
	if (!this->ParseRequestLine(ChunkLine, ClientData)){
		return (1);
	}
	std::getline(Message, ChunkLine, '\r');
	Message.ignore(1);
	this->HostName = this->FindHost(ChunkLine);
	if (this->HostName.empty()){
		ClientData.getResponse().SetStatusCode(400);
		return (1);
	}
	// find the listenning server
	if (this->CheckServerHostName(ClientData)){
		std::cerr << "Error : Unknown Server"<< std::endl;
		return (1);
	}
	//Find Location to be used;
	if (!this->FindLocationtobeUsed(ClientData)){
		this->Location = true;
	}

	// parsing Headers
	std::getline(Message, ChunkLine, '\r\n\r\n');
	if (parsetheRestoftheheaders(ChunkLine)){
		
	}
	// headers parsing;
	std::string name;
	std::string line;
	std::string Value;
	std::istringstream HeadersParser(Message.substr(Message.find("\r\n")));

	while (std::getline(HeadersParser, line)){
		if (line == "\r")
			break;
		pos = line.find(':');
		if (pos == std::string::npos){
			ClientData.getResponse().SetStatusCode(400);
			return 0;
		}
		name = line.substr(0, pos);
		Value = line.substr(pos + 1);
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
		return (0);
	}
	else if (this->methode == "POST"){
		//readbody;
		pos = Message.find("\r\n\r\n");
		size_t end = Message.find("\r\n\r\n", pos + 4);
		this->body = Message.substr(pos + 4, end);
		// parsing the body for the port Methode
	}
	return 0;
}

void Request::setServerNode(ServerNode *_ServerNode){
	this->Server = _ServerNode;
}

ServerNode& Request::getServerNode() const{
	return *this->Server;
}

void Request::setPort(std::string _Port){
	this->Port =  _Port;
}

std::string Request::getPort(){
	return (this->Port);
}

std::string Request::getHostName(){
	return (this->HostName);
}

void Request::setHostName(std::string _HostName){
	this->HostName = _HostName;
}

std::string Request::getValue(std::string _Key){
	return (this->headers[_Key]);
}

std::string Request::getBody(){
	return (this->body);
}

void Request::Setmethode(std::string _methode){
	this->methode = _methode;
}
std::string Request::getMethode(){
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
		this->querystring = _Uri.substr(pos + 1);
	}
}
Request::~Request(){
	
}
