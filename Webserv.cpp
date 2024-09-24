/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:25:41 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/24 12:38:38 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv( void ) {}

void printChilds( ListNode *child)
{
    std::vector<std::string> fields;
    while (child)
    {
        std::cout << "\tchild start: " + child->getContent() << std::endl;
        fields = child->getFields();
        for (size_t i = 0; i < fields.size(); i++)
            std::cout << "\t\t" << fields[i] << std::endl;
        child = child->getNext();
    }
}

void printServerNode(ListNode* server) {
    ServerNode n(server);
    std::map<std::string, Field > fields;

    std::map<std::string, Field > f = n.getFields();

    std::map<std::string, Field >::iterator it;
    for (it = f.begin(); it != f.end(); it++)
    {
        std::cout << it->first << " => ";
        for (int i = 0; i < (int) it->second.getValues().size(); i++) {
            std::cout << it->second.getValues()[i] << " ";
        }
        std::cout << std::endl;
    }

    std::map<std::string, Location > locations = n.getLocations();

    std::map<std::string, Location >::iterator loc_it;
        
    
    for (loc_it = locations.begin(); loc_it != locations.end(); loc_it++) {
        std::cout << loc_it->first << ": " << std::endl;
        fields = loc_it->second.getFields();
        for (it = fields.begin(); it != fields.end(); it++) {
            std::cout << "\t" << it->first << " => ";
            for (int i = 0; i < (int) it->second.getValues().size(); i++) {
                std::cout << it->second.getValues()[i] << " ";
            }
            std::cout << std::endl;
        }
    }
}

Webserv::Webserv( std::string configPath ) {

    std::cout << "Here" << std::endl;

    Parser parser(configPath);
    std::string content = parser.getContent();
    std::vector<std::string> fields;
    ListNode* head;

    if (Parser::checkValidContent(content) == false)
        throw Parser::ParsingException("INVALID BRACES IN CONFIG");

    head = parser.extractBlocks(parser.getContent(), 0);

    if (Parser::checkValidList(head, 0) == false)
        throw Parser::ParsingException("INVALID CONFIG FILE");

    while (head) {
        try
        {
            printServerNode(head);
            std::cout << "==========================" << std::endl;
            head = head->getNext();
            
        }
        catch (Parser::ParsingException e) {
            std::cout << "Config file error: " << e.what() << std::endl;
            break ;
        }
    }
}

Webserv::~Webserv( void ) {
    std::cout << "FREE here" << std::endl;
}