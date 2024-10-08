/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:12:11 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/10/07 17:50:17 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( std::vector<ServerNode*>& servers): servers(servers), keepAlive(false), RequestMessage(), ResponseMessage(){};
Client::Client( std::vector<ServerNode*>& servers, std::string message, int fd):
servers(servers), message(message), fd(fd), keepAlive(false){
    this->ResponseMessage = new Response();
    this->RequestMessage = new Request();
}

Client::~Client( void ) {
    delete this->ResponseMessage;
    delete this->RequestMessage;
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
    return (*this->ResponseMessage);
}

Request& Client::getRequest( void ){
    return (*this->RequestMessage);
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
