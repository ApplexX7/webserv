/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:12:11 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/30 15:18:19 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( void ) {};

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
