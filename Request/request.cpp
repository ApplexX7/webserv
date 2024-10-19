/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:28:36 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/19 18:15:30 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(){
	this->Uri = "";
	this->finishReading = false;
	this->compliteHeaderparser = false;
	this->bodyType = NONE;
	this->serverLocation = NULL;
}

bool Request::getFinishReading(){
	return (this->finishReading);
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

void Request::printmap(){
	std::cout << "HEADER  :" << std::endl;
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it){
		std::cout << it->first << " : " << it->second << std::endl;
	}
}


void Request::findServer(Client &clientData){
	std::vector<std::string> serverName;
	std::vector<ServerNode*> &servers = clientData.getServers();
	std::string Listen;

	for (size_t i = 0; i < servers.size(); i++){
		if (servers[i]->getField("listen").getValues()[0] == clientData.getListen()){
			if (this->listenningServer == NULL){
				this->listenningServer = servers[i];
			}
			serverName = servers[i]->getField("server_name").getValues();
			for (size_t j = 0; j < serverName.size(); j++){
				if (this->getValue("Host") == serverName[j]){
					this->listenningServer = servers[j];
					return ;
				}
			}
		}
	}
}

std::string Request::getpathName( void ){
	return (this->pathName);
}

int Request::getContentLenght(){
	return (this->contentLenght);
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
		ClientData.getResponse().setStatusCode(400);
		return (1);
	}
	if (std::count(RqLine.begin(), RqLine.end(), ' ') != 2){
		ClientData.getResponse().setStatusCode(400);
		return (1);
	}
	while (std::getline(str, token, ' ')){
		RequestLineChunks.push_back(token);
	}
	if (RequestLineChunks.size() != 3){
		ClientData.getResponse().setStatusCode(400);
		return (1);
	}
	for (size_t i = 0; i < RequestLineChunks.size(); i++){
		if (RequestLineChunks[i].empty()){
			ClientData.getResponse().setStatusCode(400);
			return (1);
		}
	}
	//valide methode;
	if (this->Validmethode(RequestLineChunks[0])){
		ClientData.getResponse().setStatusCode(501);
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
		ClientData.getResponse().setStatusCode(505);
		return (1);
	}
	return (0);
}

Location *Request::getServerLocation(){
	return (&this->serverLocation);
}


void Request::findLocationtobeUsed(){
	std::string LongestMatch = "";
	for (std::map<std::string, Location>::iterator it = this->listenningServer->getLocations().begin();
		it != this->listenningServer->getLocations().end(); it++){

			if (this->getUri().find(it->first) == 0){
				if (it->first.length() > LongestMatch.length()){
					LongestMatch = it->first;
			}
		}
	}
	if (!LongestMatch.empty()){
		this->serverLocation = this->listenningServer->getLocations()[LongestMatch];
		return ;
	}
	this->serverLocation = NULL;
}

std::string Request::FindHost(std::string HostLine){
    size_t pos = HostLine.find("Host:");
    if (pos != std::string::npos){
        std::string hostPort = HostLine.substr(pos + 5);
        hostPort.erase(std::remove_if(hostPort.begin(), hostPort.end(), ::isspace), hostPort.end());
        size_t colonPos = hostPort.find(":");
        if (colonPos != std::string::npos){
            this->port = hostPort.substr(colonPos + 1);
            return hostPort.substr(0, colonPos);
        }
        this->port = "DEFAULT";
        return hostPort;
    }
    return "";
}


std::string Request::getlocationName(){
	return (this->locationName);
}

int Request::ParsingTheRequest(Client &ClientData) {
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
		if (ChunkLine == "\r" || ChunkLine.empty())
			break;
		pos = ChunkLine.find(':');
		if (pos == std::string::npos){
			ClientData.getResponse().setStatusCode(400);
			return 0;
		}
		name = ChunkLine.substr(0, pos);
		Value = ChunkLine.substr(pos + 1);
		size_t spa = name.find(' ');
		size_t tab = name.find('\t');
		if (spa != std::string::npos || tab != std::string::npos){
			ClientData.getResponse().setStatusCode(400);
			return (0);
		}
		Value.erase(std::remove_if(Value.begin(),Value.end(), ::isspace), Value.end());
		if (Value.empty() || name.empty()){
			ClientData.getResponse().setStatusCode(400);
			return (1);
		}
		this->setHeaders(name, Value);
	}
	this->findServer(ClientData);
	this->findLocationtobeUsed();
	size_t headerEnd = ClientData.getMessage().find("\r\n\r\n");
	if (headerEnd != std::string::npos){
		ClientData.getMessage().erase(0,headerEnd + 4);
    	this->compliteHeaderparser= true;
	}
	if (this->methode == "GET" && this->compliteHeaderparser){
		this->compliteHeaderparser = false;
		this->finishReading = true;
		return (1);
	}
	else if (this->methode == "POST" && this->compliteHeaderparser){
		this->ParsePostHeaders();
	}
	return 0;
}


// this funct need to be modified later
int Request::requestParserStart(Client &clientData) {
	std::cout <<  clientData.getMessage() << std::endl;
	if (!this->compliteHeaderparser && this->ParsingTheRequest(clientData)){
		return (1);
	}
	if (this->methode == "POST" && this->compliteHeaderparser && !clientData.getMessage().empty()){
 		this->bodybuffer += clientData.getMessage();
		if (this->parseBodyTypeBuffer(this->bodybuffer)){
			this->finishReading = true;
			std::cout << "Complite the body  read\n\n\n\n\n " << std::endl;
		}
		if (clientData.getResponse().postBodyResponse(clientData)) {
			return (1);
		}
		return (0);
	}
	return (0);
}


std::string Request::getTransferCoding(){
	return (this->TransferCoding);
}

std::string Request::getEndofBoundary(){
	return (this->endofBoundary);
}

std::string Request::getStartBoundary(){
	return (this->startofBoundary);
}

TypeTransf  Request::getTheBodyType(){
	return (this->bodyType);
}

ServerNode& Request::getserverNode() const{
	return *this->listenningServer;
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

std::string& Request::getBody(){
	return (this->bodybuffer);
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