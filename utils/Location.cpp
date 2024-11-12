/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 09:31:10 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/11/12 12:09:03 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location( void ) {
    this->maxSize = 4096;
};

Location::Location( ServerNode* server ) {
    this->server = server;
}

void Location::setServer( ServerNode* server ) {
    this->server = server;
}

std::map<std::string, Field > Location::getFields( void ) {
    return this->fields;
}

void Location::addField( std::string key, std::string value ) {
    this->fields[key].addValue(value);
}

Field &Location::getField( std::string key ) {
    if (this->fields.find(key) != this->fields.end())
        return this->fields[key];
    return this->server->getField(key);
}

void Location::addCgiPath( std::string key, std::string value ) {
    this->cgiPaths[key] = value;
}

std::string Location::getCgiPath( std::string key ) {
   return this->cgiPaths[key];
}

void Location::setMaxBodySize( void ) {
    std::string values;
    std::string sizeStr;
    unsigned long long size;
    char unit;
    int length;
    bool hasUnit;

    this->maxSize = 4096;
    if (this->fields.count("client_max_body_size") != 1)
        return ;

    values = this->getField("client_max_body_size").getValues()[0];
    length = values.length();
    hasUnit = std::isalpha(values[length - 1]);

    if (hasUnit) {
        sizeStr = values.substr(0, length - 1);
        unit = std::toupper(values.substr(length - 1, 1)[0]);    
    }
    else {
        sizeStr = values;
    }
    
    size = std::atoll(sizeStr.data());
    
    if (hasUnit) {
        if (unit == 'K')
            size *= 1024;
        if (unit == 'M')
            size *= 1024 * 1024;
        if (unit == 'G')
            size *= 1024 * 1024 * 1024;
    }
    this->maxSize = size;
}

unsigned long long Location::getMaxBodySize( void ) const {
    return this->maxSize;
}