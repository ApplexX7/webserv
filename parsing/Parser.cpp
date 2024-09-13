/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 08:06:07 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/13 10:15:19 by wbelfatm         ###   ########.fr       */
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
    std::string tmpLine;

    file.open(path);

    if (!file.is_open())
    {
        std::cout << "Error opening file" << std::endl;
        exit(EXIT_FAILURE);
    }

    content = "";
    while (std::getline(file, line))
    {
        tmpLine = Parser::strTrim(line);
        
        if (tmpLine.empty())
            continue ;
        content += Parser::strTrim(line);
        content += "\n";
    }
    
    this->_fileContent = content;
}

std::string Parser::getContent( void )
{
    return this->_fileContent;
}

ListNode *Parser::extractBlock( std::string str, int level )
{
    std::string server;
    std::string tmp;
    size_t start = 0;
    size_t end = 0;
    ListNode* head;
    int isInsideBlock = 0;

    if (str.empty())
        return NULL;

    while (end < str.length() && str[end] != '{')
        end++;

    server = str.substr(start, end);
    head = new ListNode(server);
    start = end + 2;
    end = start;

    // std::cout << server << std::endl;

    // either it is a simple field: add to vector
    while (end < str.length())
    {
        // std::cout << str.substr(start, str.length()) << std::endl << std::endl;
        while (end < str.length() && str[end] != '{' && str[end] != '}' && str[end] != '\n')
            end++;
        tmp = str.substr(start, end - start);

        char tmpChar = str[end];

        // if it is '{' then there's a subfield
            // check if level is 0, create a subfield
            // otherwise take it as field
        if (tmpChar == '{')
        {
            if (level == 0)
            {
                end++;
                isInsideBlock = 0;
                while (end < str.length() && ((str[end] == '}' && isInsideBlock > 0) || (str[end] != '}')))
                {
                    if (str[end] == '{')
                        isInsideBlock++;
                    if (str[end] == '}')
                        isInsideBlock--;
                    end++;
                }

                end++;

                head->addChild(Parser::extractBlock(str.substr(start, end - start), level + 1));
            }
            else
            {
                end++;
                tmp += '{';
                head->addField(tmp);
            }
        }
        else if (tmpChar == '\n')
            head->addField(tmp);
        else if (tmpChar == '}')
        {
            head->addNext(Parser::extractBlock(str.substr(end + 1, str.length()), level));
            break ;
        }
        end++;
        start = end;
    }
    return head;
}

std::string Parser::strTrim( std::string str )
{
    int start = 0;
    int end = str.length() - 1;

    while (std::isspace(str[start]))
        start++;
    while (std::isspace(str[end]))
        end--;
    return str.substr(start, end - start + 1);
}