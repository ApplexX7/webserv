/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 08:06:07 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/07 15:34:45 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include <fstream>

Parser::Parser( void ) {};

Parser::Parser( std::string fileName ) {
    this->_fileName = fileName;
    this->loadFile();
};

Parser::Parser( Parser& copy ){
    (void) copy;
};

Parser::~Parser( void ) {};

Parser& Parser::operator=( Parser& rhs ){
    return rhs;
};

void Parser::loadFile( void )
{
    std::string path = this->_fileName;
    std::ifstream file;
    std::string content;
    std::string line;

    file.open(path);

    if (!file.is_open())
    {
        std::cout << "Error opening file" << std::endl;
        exit(EXIT_FAILURE);
    }

    content = "";
    while (std::getline(file, line))
    {
        content += line;
        content += "\n";
    }
    
    this->_fileContent = content;
}

std::string Parser::getContent( void )
{
    return this->_fileContent;
}

ListNode *Parser::extractBlock( std::string str )
{
    ListNode *head;

    int size = str.size();
    std::string key = "";
    std::string val = "";
    int inside = 0;
    int start;

    int i = 0;
    
    while (i < size)
    {
        if (str[i] == '{' || str[i] == '\n')
            break ;
        key += str[i];
        i++;
    }

    head = new ListNode(key);
    if (i >= size)
    {
        return head;
    }
    
    
    if (str[i] == '{')
    {
        inside = 0;
        i++;
        while (i < size && (str[i] == ' ' || str[i] == '\n'))
            i++;
        start = i;
        while (i < size)
        {
            if (str[i] == '}' && inside <= 0)
                break ;
            if (str[i] == '{')
                inside++;
            if (str[i] == '}')
                inside--;
            i++;
        }
        if (i >= size || inside < 0)
        {
            std::cout << "Invalid braces" << std::endl;
            exit(EXIT_FAILURE);
        }
        val = str.substr(start, i - start);
    }

    if (i + 1 < size)
    {
        head->addNext(Parser::extractBlock(str.substr(i + 1, size - i + 1)));
    }
    
    if (!val.empty())
        head->addChild(Parser::extractBlock(val));


    return head;
}