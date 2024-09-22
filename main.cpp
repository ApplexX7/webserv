/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 08:04:58 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/22 10:16:24 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void printChilds( ListNode *child)
{
    std::vector<std::string> fields;
    while (child)
    {
        std::cout << "child start: " + child->getContent() << std::endl;
        fields = child->getFields();
        for (size_t i = 0; i < fields.size(); i++)
            std::cout << '\t' << fields[i] << std::endl;
        child = child->getNext();
    }
}

void printServerNode(ListNode* server) {
    ServerNode n(server);

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

    std::map<std::string, std::map<std::string, Field > > locations = n.getLocations();

    std::map<std::string, std::map<std::string, Field > >::iterator loc_it;
    
    for (loc_it = locations.begin(); loc_it != locations.end(); loc_it++) {
        std::cout << loc_it->first << ": " << std::endl;
        for (it = loc_it->second.begin(); it != loc_it->second.end(); it++) {
            std::cout << "\t" << it->first << " => ";
            for (int i = 0; i < (int) it->second.getValues().size(); i++) {
                std::cout << it->second.getValues()[i] << " ";
            }
            std::cout << std::endl;
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: ./webserv <config_file>" << std::endl;
        return 1;
    }

    std::string configPath(argv[1]);
    Parser parser(configPath);

    std::string content = parser.getContent();

    if (Parser::checkValidContent(content) == false)
        return printf("INVALID BRACES IN CONFIG");

    ListNode* head = parser.extractBlocks(parser.getContent(), 0);
    std::vector<std::string> fields;

    if (Parser::checkValidList(head, 0) == false)
        return printf("INVALID CONFIG FILE");

    // while (head)
    // {
    //     std::cout << head->getContent() << std::endl;
    //     fields = head->getFields();
    //     for (size_t i = 0; i < fields.size(); i++)
    //         std::cout << fields[i] << std::endl;
    //     printChilds(head->getChild());
    //     head = head->getNext();
    //     std::cout << std::endl;
    // }

    while (head) {
         printServerNode(head);
         std::cout << "==========================" << std::endl;
        head = head->getNext();
    }
}