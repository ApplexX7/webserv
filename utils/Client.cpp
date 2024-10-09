/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:12:11 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/10/08 16:01:39 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( std::vector<ServerNode*>& servers):
servers(servers),
keepAlive(false) {
    this->responseMessage = new Response();
    this->requestMessage = new Request();
    this->parentServer = NULL;
};

Client::Client( std::vector<ServerNode*>& servers, std::string message, int fd):
servers(servers), message(message), fd(fd), keepAlive(false) {
    this->responseMessage = new Response();
    this->requestMessage = new Request();
     this->parentServer = NULL;
}

Client::~Client( void ) {
    delete this->responseMessage;
    delete this->requestMessage;
};

std::string Client::getMessage( void ) {
    return this->message;
}

void Client::setMessage( std::string message ) {
    this->message = message;
}

void Client::appendMessage( std::string message ) {
    this->message += message;
}

int Client::getFd( void )
{
    return this->fd;
}

void Client::setFd( int fd ) {
    this->fd = fd;
}

bool Client::isResponseReady( void ) {
    return this->responseReady;
}

Response& Client::getResponse( void ){
    return (*this->responseMessage);
}

Request& Client::getRequest( void ){
    return (*this->requestMessage);
}

bool Client::isKeepAlive( void ) {
    return this->keepAlive;
}

std::vector<ServerNode*>& Client::getServers( void ) {
    return this->servers;
}

void Client::setListen( std::string listen ) {
    this->listen = listen;
}

std::string Client::getListen( void ) {
    return this->listen;
}

void Client::findParentServer( void ) {
    std::string host = this->getRequest().getValue("Host");
    std::vector<ServerNode*> &servers = this->servers;
    std::vector<std::string> serverNames;
    std::string serverListen;
    bool matched = false;

    for (int i = 0; i < (int) servers.size(); i++) {
        serverListen = servers[i]->getField("listen").getValues()[0];
        
        
        if (serverListen == this->listen) {
            // std::cout << "|" << serverListen << "| |" << this->listen << "|" <<std::endl;
            if (this->parentServer == NULL) {
                // std::cout << "default found on " << serverListen << std::endl;
                this->parentServer = servers[i];
            }

            serverNames = servers[i]->getField("server_name").getValues();

            for (int j = 0; j < (int) serverNames.size(); j++) {
                if (host == serverNames[j]) {
                    // match found
                    // std::cout << "Matched with " << serverListen << " on " << host << std::endl;
                    this->parentServer = servers[i];
                    matched = true;
                    break ;
                }
            }
        }
        if (matched)
            break ;
    }
}

ServerNode& Client::getParentServer( void ) {
    if (this->parentServer == NULL)
        std::cout << "server is null" << std::endl;
    return *(this->parentServer);
}
