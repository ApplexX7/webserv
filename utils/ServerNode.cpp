/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 11:47:25 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/26 08:56:59 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerNode.hpp"
#include "../parsing/Parser.hpp"

void ServerNode::initializeServer( ListNode* server ) {
    std::map<std::string,  Location>::iterator it;
    std::vector<std::string> fields = server->getFields();
    std::vector<std::string> splitField;
    std::string path;
    std::string trimedField;

    ListNode *child = server->getChild();

    for (int i = 0; i < (int) fields.size(); i++) {
        trimedField = Parser::strTrim(fields[i]);
        if (trimedField.length() == 0)
            continue;
        splitField = (Parser::strSplit(trimedField, ' '));
        if (splitField.size() == 1)
            throw Parser::ParsingException("Directive has no arguments " + splitField[0]);
        if (splitField[0] != "error_page" && this->fieldExists(splitField[0]))
            throw Parser::ParsingException("Duplicate directives for " + splitField[0]);

        for (int j = 1; j < (int) splitField.size(); j++) {

            // check for status code format in case of error pages
            if (splitField[0] == "error_page")
            {
                if (j < (int) splitField.size() - 1
                && !Parser::isNumber(splitField[j]))
                    throw Parser::ParsingException("Status code must be numeric");
                if (j < (int) splitField.size() - 1
                && (std::stol(splitField[j]) < 300 || std::stol(splitField[j]) > 599))
                    throw Parser::ParsingException("Status code must be between 300 and 599");
            }

            this->addField(splitField[0], splitField[j]);
        }

        std::vector<std::string> values = this->getField(splitField[0]).getValues();

        Parser::validateField(splitField[0], values);
        if (splitField[0] == "listen" && values[0].find(":") == values[0].npos)
            this->getField(splitField[0]).updateValue("0.0.0.0:" + values[0], 0);
    }

    // if server has no listen directive
    if (this->fields.find("listen") == this->fields.end())
        this->addField("listen", "0.0.0.0:8000");

    // insert locations
    while (child != NULL)
    {
        fields = child->getFields();
        splitField = (Parser::strSplit(child->getContent(), ' '));
        if (splitField.size() != 2 || splitField[0] != "location")
            throw Parser::ParsingException("Expected location directive but got \"" + splitField[0] + "\"");
        
        path = splitField[1];

        if (this->locationExists(path))
            throw Parser::ParsingException("Duplicate locations for path " + path);

        for (int i = 0; i < (int) fields.size(); i++) {
            trimedField = Parser::strTrim(fields[i]);
            if (trimedField.length() == 0)
                continue ;
            splitField = (Parser::strSplit(trimedField, ' '));
            if (splitField.size() == 1)
                throw Parser::ParsingException("Directive has no arguments " + splitField[0]);
            if (splitField[0] != "error_page" && this->locationFieldExists(path, splitField[0]))
            {
                
                throw Parser::ParsingException("Duplicate directives for " + splitField[0]);
            }
            
            for (int j = 1; j < (int) splitField.size(); j++) {
                // check for status code format in case of error pages
                if (splitField[0] == "error_page")
                {
                    if (j < (int) splitField.size() - 1
                    && !Parser::isNumber(splitField[j]))
                        throw Parser::ParsingException("Status code must be numeric");
                    if (j < (int) splitField.size() - 1
                    && (std::stol(splitField[j]) < 300 || std::stol(splitField[j]) > 599))
                        throw Parser::ParsingException("Status code must be between 300 and 599");
                }
                this->addLocationField(path, splitField[0], splitField[j]);
            }

            Parser::validateField(splitField[0], this->locations[path].getField(splitField[0]).getValues());
        }
        if (this->locations.find(path) == this->locations.end())
        {
            // location has no directives
            this->locations[path] = NULL;
        }
        this->locations[path].setServer(this);
        child = child->getNext();
    }
}

// canonical form
ServerNode::ServerNode( void ) {};

ServerNode::ServerNode( ServerNode& cpy ) {
    (void) cpy;
};

ServerNode& ServerNode::operator=( ServerNode& rhs ) {
    return rhs;
};

ServerNode::~ServerNode( void ) {};

void setAllowedFields( ServerNode* server ) {
    // allowed server fields
    server->allowedFields.push_back("root");
    server->allowedFields.push_back("listen");
    server->allowedFields.push_back("server_name");
    server->allowedFields.push_back("client_max_body_size");
    server->allowedFields.push_back("upload_allowed");
    server->allowedFields.push_back("error_page");

    // allowed location fields
    server->allowedLocationFields.push_back("root");
    server->allowedLocationFields.push_back("client_max_body_size");
    server->allowedLocationFields.push_back("limit_except");
    server->allowedLocationFields.push_back("error_page");
    
}

ServerNode::ServerNode( ListNode *server ) {
    setAllowedFields(this);
    initializeServer(server);
};

void ServerNode::addField( std::string key, std::string value) {
    if (std::find(
        this->allowedFields.begin(),
        this->allowedFields.end(),
        key) == this->allowedFields.end())
        throw Parser::ParsingException("Unknown directive " + key);
    this->fields[key].addValue(value);
}

void ServerNode::addLocationField( std::string path, std::string key, std::string value) {
    if (std::find(
        this->allowedLocationFields.begin(),
        this->allowedLocationFields.end(),
        key) == this->allowedLocationFields.end())
        throw Parser::ParsingException("Unknown directive " + key + " for location block");
    this->locations[path].addField(key, value);
}

std::map<std::string, Field > ServerNode::getFields( void ) {
    return this->fields;
}

std::map<std::string, Location > ServerNode::getLocations( void ) {
    return this->locations;
}

Field& ServerNode::getField( std::string key ) {
    return this->fields[key];
}

bool ServerNode::fieldExists( std::string key ) {
    if (this->fields.find(key) != this->fields.end())
        return true;
    return false;
}

bool ServerNode::locationExists( std::string path ) {
    if (this->locations.find(path) != this->locations.end())
        return true;
    return false;
}

bool ServerNode::locationFieldExists( std::string path, std::string key ) {
    
    std::map<std::string, Field > fields =  this->locations[path].getFields();

    if (fields.find(key) != fields.end())
        return true;
    return false;
}