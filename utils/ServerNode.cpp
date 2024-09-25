/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 11:47:25 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/25 11:16:26 by wbelfatm         ###   ########.fr       */
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
        splitField = (Parser::strSplit(trimedField));
        if (splitField.size() == 1)
            throw Parser::ParsingException("Invalid number of arguments for " + splitField[0]);
        if (this->fieldExists(splitField[0]))
            throw Parser::ParsingException("Duplicate directives for " + splitField[0]);
        for (int j = 1; j < (int) splitField.size(); j++) {
            this->addField(splitField[0], splitField[j]);
        }

        Parser::validateField(splitField[0], this->getField(splitField[0]).getValues());
    }

    while (child != NULL)
    {
        fields = child->getFields();
        splitField = (Parser::strSplit(child->getContent()));
        if (splitField.size() != 2 || splitField[0] != "location")
            throw Parser::ParsingException("Expected location directive but got \"" + splitField[0] + "\"");
        
        path = splitField[1];

        if (this->locationExists(path))
            throw Parser::ParsingException("Duplicate locations for path " + path);

        for (int i = 0; i < (int) fields.size(); i++) {
            trimedField = Parser::strTrim(fields[i]);
            if (trimedField.length() == 0)
                continue ;
            splitField = (Parser::strSplit(trimedField));
            if (splitField.size() == 1)
                throw Parser::ParsingException("Invalid number of arguments for " + splitField[0]);
            
            for (int j = 1; j < (int) splitField.size(); j++) {
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

ServerNode::ServerNode( ServerNode& cpy ) {
    (void) cpy;
};

ServerNode& ServerNode::operator=( ServerNode& rhs ) {
    return rhs;
};

ServerNode::~ServerNode( void ) {};

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

Field ServerNode::getField( std::string key ) {
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