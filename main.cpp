/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 08:04:58 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/13 10:13:55 by wbelfatm         ###   ########.fr       */
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

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: ./webserv <config_file>" << std::endl;
        return 1;
    }

    std::string configPath(argv[1]);
    Parser parser(configPath);
    
    ListNode* head = parser.extractBlock(parser.getContent(), 0);
    std::vector<std::string> fields;

    while (head)
    {
        std::cout << head->getContent() << std::endl;
        fields = head->getFields();
        for (size_t i = 0; i < fields.size(); i++)
            std::cout << fields[i] << std::endl;
        printChilds(head->getChild());
        head = head->getNext();
        std::cout << std::endl;
    }
}