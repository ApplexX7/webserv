/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:12:11 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/10/01 10:24:59 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( std::vector<ServerNode*>& servers ): servers(servers), keepAlive(false) {};

Client::Client( std::vector<ServerNode*>& servers, std::string message, int fd):
servers(servers), message(message), fd(fd), keepAlive(false) {}

Client::~Client( void ) {};

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
