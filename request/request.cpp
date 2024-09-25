/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:28:36 by mohilali          #+#    #+#             */
/*   Updated: 2024/09/25 16:21:23 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

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
    this->Uri = _Uri;
}
Request::~Request(){
    
}