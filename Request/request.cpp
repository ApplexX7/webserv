/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:28:36 by mohilali          #+#    #+#             */
/*   Updated: 2024/11/11 16:43:28 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(){
	this->requestLine = false;
	this->handleCgi = new Cgi;
	this->isaCgi = false;
	this->maxBodySize = CHUNK_SIZE;
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

bool Request::getIsACgi(){
	return (this->isaCgi);
}

void Request::setHeaders(std::string &name, std::string &value){
	if (!name.empty() && !value.empty()) {
		this->headers[name] = value;
	} else {
		std::cerr << "Error: Header name or value is empty!" << std::endl;
	}
}

void Request::findServer(Client &clientData){
	std::vector<std::string> serverName;
	std::vector<ServerNode*> &servers = clientData.getServers();
	std::string Listen;
	this->listenningServer = NULL;

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

int Request::isCgi(){
	size_t pos = 0;
	std::string cgiExtension;
	pos = this->Uri.find(".");
	if (pos == std::string::npos)
		return (0);
	cgiExtension = this->Uri.substr(pos);

	if (this->serverLocation && !this->serverLocation->getCgiPath(cgiExtension).empty()){
		this->handleCgi->setExtension(cgiExtension);
		this->handleCgi->setCgiPath(this->serverLocation->getCgiPath(cgiExtension));
		if(this->serverLocation->getField("root").getValues().size() > 0){
			this->handleCgi->setDirectPath(this->serverLocation->getFields()["root"].getValues()[0]);
		}
		else
			this->handleCgi->setDirectPath(this->listenningServer->getFields()["root"].getValues()[0]);
		if (this->serverLocation->getCgiPath(cgiExtension).empty()){
			return (0);
		}
		this->isaCgi = true;
		return (1);
	}
	else if (!this->listenningServer->cgiPaths[cgiExtension].empty()){
		this->handleCgi->setExtension(cgiExtension);
		this->handleCgi->setCgiPath(this->listenningServer->cgiPaths[cgiExtension]);
		this->handleCgi->setDirectPath(this->listenningServer->getFields()["root"].getValues()[0]);
		this->isaCgi = true;
		return (1);
	}
	return (0);
}

int Request::Validmethode(std::string &_Methode, Client &clientData){
	(void)clientData;
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

int Request::parseUri(std::string &uri){
	size_t pos = 0;

	if (uri.length() > 800){
		return (1);
	}
	pos = uri.find("?");
	if (pos != std::string::npos) {
		this->Uri = uri.substr(0, pos);
		this->quertyString = uri.substr(pos + 1);
	}
	else{
		this->Uri = uri;
	}
	return (0);
}

int Request::ParseRequestLine(std::string &RqLine, Client &clientData){
	std::vector<std::string> RequestLineChunks;
	std::stringstream str(RqLine);
	std::string token;

	if (RqLine.empty()){
		clientData.getResponse().setStatusCode(400);
		return (1);
	}
	if (std::count(RqLine.begin(), RqLine.end(), ' ') != 2){
		clientData.getResponse().setStatusCode(400);
		return (1);
	}
	while (std::getline(str, token, ' ')){
		RequestLineChunks.push_back(token);
	}
	if (RequestLineChunks.size() != 3){
		clientData.getResponse().setStatusCode(400);
		return (1);
	}
	for (size_t i = 0; i < RequestLineChunks.size(); i++){
		if (RequestLineChunks[i].empty()){
			clientData.getResponse().setStatusCode(400);
			return (1);
		}
	}
	// check the Uri // and modify some check for the Uri
	if (this->parseUri(RequestLineChunks[1])){
		clientData.getResponse().setStatusCode(414);
		return (1);
	}
	// checkk HTTPS VERSION;
	if (RequestLineChunks[2] != "HTTP/1.1"){
		clientData.getResponse().setStatusCode(505);
		return (1);
	}
	//valide methode;
	if (this->Validmethode(RequestLineChunks[0], clientData)){
		clientData.getResponse().setStatusCode(501);
		return (1);
	}
	return (0);
}

Location *Request::getServerLocation(){
	return (this->serverLocation);
}

Location *Request::findLocationtobeUsed(){
	std::map<std::string, Location> locations = this->listenningServer->getLocations();
	std::map<std::string, Location>::iterator it;
	std::string path = this->Uri;
	Location *location = NULL;
	std::string lastMatch = "";
	std::string root;
	std::string rest;
	size_t found;

	if (locations.size()) {
		for (it = locations.begin(); it != locations.end(); it++)
		{
			found = path.find(it->first);
			if (found != path.npos && found == 0)
			{
				rest = path.substr(found + it->first.length());
				// location match
				if (it->first.length() > lastMatch.length() && (rest.length() == 0 || rest[0] == '/' || it->first[it->first.length() - 1] == '/'))
				{
					location = &it->second;
					lastMatch = it->first;
				}
			}
		}
		
	}
	return location;
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

void Request::deleteMethode(Client &clientData){
	// check for the Uri;
	struct  stat ss;
	Location *PathLcoation =  this->getServerLocation();
	if (PathLcoation != NULL){
		this->pathName = PathLcoation->getField("root").getValues()[0] + "/" + this->Uri;
	}
	else{
		this->pathName = clientData.getRequest().getserverNode().getFields()["root"].getValues()[0] + "/" + this->Uri;
	}
	if (stat(this->pathName.c_str(), &ss) != 0){
		clientData.getResponse().setStatusCode(204);
	}
	else{
		if (ss.st_mode & S_IFDIR){
			clientData.getResponse().setStatusCode(403);
			return ;
		}
		else if (ss.st_mode & S_IFREG){
			if (access(this->pathName.c_str(), O_WRONLY) == 0){
					std::cout << this->pathName<< std::endl;
				if (!remove(this->pathName.c_str())){
					clientData.getResponse().setStatusCode(202);
				}
				else
					clientData.getResponse().setStatusCode(500);
				return ;
			}
			else
				clientData.getResponse().setStatusCode(403);
			
		}
		else{
			clientData.getResponse().setStatusCode(404);
			return ;
		}
	}
}


std::string stringToupper(std::string str){
	for (int i = 0; str[i]; i++){
		if (str[i] == '-')
			str[i] = '_';
		else
			str[i] = std::toupper(str[i]);
	}
	return (str);
}

int Request::handleEnvForCgi(){
	std::vector<std::string> cgiEnv;
	std::string headr;

	cgiEnv.push_back("REQUEST_METHOD=" + this->methode);
	cgiEnv.push_back("SERVER_PORT=" + this->port);
	cgiEnv.push_back("QUERY_STRING=" + this->quertyString);
	cgiEnv.push_back("SERVER_NAME=webser1.1");
	cgiEnv.push_back("SCRIPT_NAME=" + this->Uri);
	
	for (std::map<std::string, std::string>::iterator it = this->getHeaders().begin(); it != this->getHeaders().end(); it++){
		cgiEnv.push_back(stringToupper(it->first) + "=" + it->second);
	}
	this->handleCgi->setUpCgenv(cgiEnv);
	return (0);
}

int Request::ParsingTheRequest(Client &ClientData) {
	size_t pos;
	std::string name;
	std::string Value;
	std::string ChunkLine;
	std::stringstream Message(ClientData.getMessage());


	std::getline(Message, ChunkLine, '\r');
	Message.ignore(1);
	if (!this->requestLine && this->ParseRequestLine(ChunkLine, ClientData)){
		this->finishReading = true;
		return (1);
	}
	this->requestLine = true;
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
			this->finishReading = true;
			return (1);
		}
		this->setHeaders(name, Value);
	}
	this->findServer(ClientData);
	this->serverLocation  = this->findLocationtobeUsed();
	size_t headerEnd = ClientData.getMessage().find("\r\n\r\n");
	if (headerEnd != std::string::npos){
		ClientData.getMessage().erase(0,headerEnd + 4);
    	this->compliteHeaderparser = true;
	}
	else {
		return (0);
	}
	if (this->isCgi() && this->compliteHeaderparser){
		this->handleEnvForCgi();
		if (this->methode == "GET")
			this->finishReading = true;
		else if (this->methode == "POST" && this->compliteHeaderparser){
			this->ParsePostHeaders();
		}
		else{
			this->isaCgi = false;
			ClientData.getResponse().setStatusCode(406);
			this->finishReading= true;
			return (1);
		}
		return (0);
	}
	if (this->methode == "GET" && this->compliteHeaderparser){
		this->compliteHeaderparser = false;
		this->finishReading = true;
		ClientData.responseReady = true;
		return (1);
	}
	else if (this->methode == "POST" && this->compliteHeaderparser){
		this->ParsePostHeaders();
		if (this->serverLocation){
			this->maxBodySize = std::atoi(this->serverLocation->getField("client_max_body_size").getValues()[0].c_str());
		}
		return (0);
	}
	if (this->methode == "DELETE"){
		this->deleteMethode(ClientData);
		this->finishReading = true;
		ClientData.responseReady = true;
		return (1);
	}
	return 0;
}


int Request::ParsePostMethode(Client &clientData){
 	this->bodybuffer = clientData.getMessage();
	int returValue = this->parseBodyTypeBuffer(this->bodybuffer);
	if (returValue != 3 && returValue != 0){
		this->finishReading = true;
	}
	else if (returValue == 3){
		size_t pos = this->bodybuffer.find("\r\n");
		std::cout << this->bodybuffer << std::endl;
		if (pos != std::string::npos){
			this->finishReading =  true;
		}
	}
	if (clientData.getResponse().postBodyResponse(clientData)){
		this->finishReading = true;
		return (1);
	}
	return (0);
}

int Request::requestParserStart(Client &clientData) {
	if (!this->isaCgi && !this->compliteHeaderparser && this->ParsingTheRequest(clientData)){
		if (this->finishReading == true){
			clientData.responseReady = true;
		}
		return (1);
	}
	else if (this->isaCgi){
		if (this->methode == "POST" &&  this->compliteHeaderparser)
			if (this->ParsePostMethode(clientData))
				return (1);
		if (this->finishReading){
			if (this->handleCgi->executeCgi(clientData))
				clientData.responseReady = true;
		}
		return (0);
	}
	if (this->methode == "POST" && this->compliteHeaderparser){
		if (this->ParsePostMethode(clientData))
			return (1);
	}
	return (0);
}

void Request::setFinishReading(bool var){
	this->finishReading = var;
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

long int &Request::getClientMaxSizeBody(){
	return (this->maxBodySize);
}

std::map<std::string,std::string> &Request::getHeaders( void ) {
	return this->headers;
}

void Request::reset( void ) {
	this->Uri = "";
	this->finishReading = false;
	this->compliteHeaderparser = false;
	this->isaCgi = false;
	this->requestLine = false;
	this->handleCgi->reset();
}

Request::~Request() {
	delete this->handleCgi;
}