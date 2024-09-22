/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 11:47:25 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/22 08:54:35 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerNode.hpp"
#include "../parsing/Parser.hpp"

// canonical form

ServerNode::ServerNode( void ) {};

ServerNode::ServerNode( ListNode *server ) {
    // loop through the server fields, get the first word as key and others as value
    std::vector<std::string> fields = server->getFields();
    std::vector<std::string> splitField;

    for (int i = 0; i < (int) fields.size(); i++) {
        splitField = *(Parser::strSplit(fields[i]));
        for (int j = 1; j < (int) splitField.size(); j++) {
            this->addField(splitField[0], splitField[j]);
        }
    }
    // loop through children set second word as key and all other fields as value
};

ServerNode::ServerNode( ServerNode& cpy ) {
    (void) cpy;
};

ServerNode& ServerNode::operator=( ServerNode& rhs ) {
    return rhs;
    
};

ServerNode::~ServerNode( void ) {};

void ServerNode::addField( std::string key, std::string value) {
    this->fields[key].push_back(value);
}

std::map<std::string, std::vector<std::string> > ServerNode::getFields( void ) {
    return this->fields;
}