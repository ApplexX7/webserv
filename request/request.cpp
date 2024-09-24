/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:28:36 by mohilali          #+#    #+#             */
/*   Updated: 2024/09/24 16:32:51 by mohilali         ###   ########.fr       */
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

void Request::SetHost(std::string _Host){
    this->Host = _Host;
}

std::string Request::getHost(){
    return (this->Host);
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