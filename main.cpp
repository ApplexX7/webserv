/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 08:04:58 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/07 15:32:35 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void printChilds( ListNode *child, int level)
{
    std::string prefix = "\t";
    for (int i = 0; i < level; i++)
        prefix += "\t";

    while (child)
    {
        std::cout << prefix + "child: " + child->getContent() << std::endl;
        printChilds(child->getChild(), level + 1);
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
    ListNode* head = Parser::extractBlock(parser.getContent());
    (void) head;
    while (head)
    {
        std::cout << head->getContent() << std::endl;
        printChilds(head->getChild(), 0);
        head = head->getNext();
    }
}