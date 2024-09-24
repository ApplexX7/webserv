/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 11:47:25 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/24 11:24:34 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerNode.hpp"
#include "../parsing/Parser.hpp"

// canonical form

ServerNode::ServerNode( void ) {};

ServerNode::ServerNode( ListNode *server ) {
    std::map<std::string,  Location>::iterator it;

    // loop through the server fields, get the first word as key and others as value
    std::vector<std::string> fields = server->getFields();
    std::vector<std::string> splitField;
    std::string path;
    std::string trimedField;

    ListNode *child = server->getChild();

    for (int i = 0; i < (int) fields.size(); i++) {
        trimedField = Parser::strTrim(fields[i]);
        if (trimedField.length() == 0)
            continue;
        splitField = *(Parser::strSplit(trimedField));
        if (splitField.size() < 2)
        {
            std::cerr << "Field " + splitField[0] + " has no value" << std::endl;
            exit(EXIT_FAILURE);
        }
        for (int j = 1; j < (int) splitField.size(); j++) {
            this->addField(splitField[0], splitField[j]);
        }
    }
    
    // loop through children, set second word as key and all other fields as value
    while (child != NULL)
    {
        fields = child->getFields();
        splitField = *(Parser::strSplit(child->getContent()));
        if (splitField.size() != 2)
        {
            std::cerr << "LOCATION PATH INVALID" << std::endl;
            exit(EXIT_FAILURE);
        }
        
        path = splitField[1];

        if (fields.size() == 0)
        {
            std::cerr << "LOCATION HAS NO DIRECTIVES" << std::endl;
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < (int) fields.size(); i++) {
            trimedField = Parser::strTrim(fields[i]);
            if (trimedField.length() == 0)
                continue ;
            splitField = *(Parser::strSplit(trimedField));
            if (splitField.size() < 2)
            {
                std::cerr << "Field \"" + splitField[0] +  "\" has no value" << std::endl;
                exit(EXIT_FAILURE);
            }
            for (int j = 1; j < (int) splitField.size(); j++) {
                this->addLocationField(path, splitField[0], splitField[j]);
            }
        }
        if (this->locations.find(path) == this->locations.end())
        {
            // location has no directives
            this->locations[path] = NULL;
        }
        this->locations[path].setServer(this);
        child = child->getNext();
    }
};

ServerNode::ServerNode( ServerNode& cpy ) {
    (void) cpy;
};

ServerNode& ServerNode::operator=( ServerNode& rhs ) {
    return rhs;
};

ServerNode::~ServerNode( void ) {};

void ServerNode::addField( std::string key, std::string value) {
    this->fields[key].addValue(value);
}

void ServerNode::addLocationField( std::string path, std::string key, std::string value) {
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