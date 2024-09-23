/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 08:06:07 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/23 12:14:28 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include <fstream>
#include <algorithm>
#include <stack>

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
        content += " ";
    }
    
    this->_fileContent = content;
}

std::string Parser::getContent( void )
{
    return this->_fileContent;
}

ListNode *Parser::extractBlocks( std::string str, int level )
{
    std::string server;
    std::string tmp;
    size_t start = 0;
    size_t end = 0;
    ListNode* head;
    int isInsideBlock = 0;

    if (Parser::strTrim(str).empty())
        return NULL;

    while (end < str.length() && str[end] != '{')
        end++;

    server = str.substr(start, end);
    head = new ListNode(server);
    start = end + 2;
    if (str[start - 1] != '\n')
        start--;
    end = start;

    // either it is a simple field: add to vector
    while (end < str.length())
    {
        while (end < str.length() && str[end] != '{' && str[end] != '}' && str[end] != ';')
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
                head->addChild(Parser::extractBlocks(str.substr(start, end - start), level + 1));
            }
            else
            {
                start = end;
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
                tmp += str.substr(start, end - start + 1);
                
                std::replace(tmp.begin(), tmp.end(), '\n', ' ');
                head->addField(tmp);
            }
        }
        else if (tmpChar == ';')
            head->addField(tmp);
        else if (tmpChar == '}')
        {
            if (str[end - 1] != ';')
                head->addField(tmp);
            head->addNext(Parser::extractBlocks(str.substr(end + 1, str.length()), level));
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

bool Parser::checkValidList( ListNode *head, int level )
{
    ListNode *tmp = head;
    bool isValid = true;
    std::string content;
    std::vector<std::string> fields;


    if (level > 1)
        return false;

    // check if content contains braces
    while (tmp && isValid) {
        content = Parser::strTrim(tmp->getContent());
        fields = tmp->getFields();
        if ((level == 0 && content != "server") || content.find('{') != content.npos
         || content.find('}') != content.npos)
            return false;

        for (size_t i = 0; i < fields.size(); i++) {
            if (fields[i].find('{') != fields[i].npos
            ||  fields[i].find('}') != fields[i].npos)
                return false;
        }

        if (tmp->hasChild())
            isValid = isValid && Parser::checkValidList(tmp->getChild(), level + 1);
        tmp = tmp->getNext();
    }

    return isValid;
}

bool Parser::checkValidContent( std::string str ) {
    std::stack<char> st;

    for (size_t i = 0; i < str.length(); i++) {
        if (st.empty() && (str[i] == '{' || str[i] == '}'))
        {
            st.push(str[i]);
            continue;
        }
        if (str[i] == '}')
            st.pop();
        if (str[i] == '{')
            st.push(str[i]);
            
    }
    return st.empty();
}

std::vector<std::string> *Parser::strSplit( std::string str ) {
    size_t start;
    size_t end;
    std::vector<std::string> *strs = new std::vector<std::string>;

    while (str.length() > 0)
    {
        start = 0;
        while (start < str.length() && str[start] == ' ')
            start++;
        if (start >= str.length())
            break;
        end = start;
        while (end < str.length() && str[end] != ' ')
            end++;
        strs->push_back(str.substr(start, end - start));
        // std::cout << str.substr(start, end - start) << std::endl;

        if (end >= str.length())
            break ;
        str = str.substr(end + 1, str.length() - 1);
    }

    return strs;
}