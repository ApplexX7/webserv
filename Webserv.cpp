/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:25:41 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/29 10:05:05 by wbelfatm         ###   ########.fr       */
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

void Webserv::init( std::string configPath ) {
    Parser parser(configPath);
    std::string content = parser.getContent();
    std::vector<std::string> fields;
    ListNode* head;
    ListNode* tmp;
    std::string listenValue;
    std::string otherListenValue;
    std::vector<std::string> serverNames;
    std::vector<std::string> otherServerNames;

    if (Parser::checkValidContent(content) == false)
        throw Parser::ParsingException("Invalid braces in config file");

    head = parser.extractBlocks(parser.getContent(), 0);
    this->listHead = head;
    if (Parser::checkValidList(head, 0) == false)
        throw Parser::ParsingException("Invalid server block name");

    tmp = head;
    try
    {
        while (tmp) {
            // printServerNode(tmp);
            this->servers.push_back(new ServerNode(tmp));

            // check servername conflicts
            for (int i = 0; i < (int) this->servers.size() - 1; i++) {
                // check if there's a server with the same host:port
                listenValue = this->servers[this->servers.size() - 1]->getField("listen").getValues()[0];
                otherListenValue = this->servers[i]->getField("listen").getValues()[0];
                
                
                // same host:port
                if (listenValue == otherListenValue) {
                        // check server_names
                        serverNames = this->servers[this->servers.size() - 1]->getField("server_name").getValues();
                        otherServerNames = this->servers[i]->getField("server_name").getValues();
                        
                        for (int j = 0; j < (int) serverNames.size(); j++) {
                            if (std::find(otherServerNames.begin(), otherServerNames.end(), serverNames[j]) != otherServerNames.end())
                            {
                                // conflicting server name
                                std::cout << "[WARN] Conflicting server name \"" << serverNames[j] << "\" at " << listenValue << std::endl;
                            }
                        }
                }
            }

            tmp = tmp->getNext();
        }
        std::cout << "ALL GOOD" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Config file error: " << e.what() << std::endl;
    }
}

Webserv::~Webserv( void ) {
    ListNode::freeListNode(this->listHead);
    
    // free servers
    for (int i = 0; i < (int) this->servers.size(); i++)
    {
        delete this->servers[i];
    }

    std::cout << "FREED" << std::endl;
}