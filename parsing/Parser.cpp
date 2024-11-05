/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 08:06:07 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/11/05 17:18:32 by wbelfatm         ###   ########.fr       */
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
    file.close();
}

std::string Parser::getContent( void )
{
    return this->_fileContent;
}

std::string extractServer(std::string str, size_t& start) {
    std::string serverBlock = "";
    size_t end = start;
    int isInside = 0;

    while (end < str.length()) {
        if (str[end] == '}' && isInside == 1)
            break;
        if (str[end] == '{')
            isInside++;
        if (str[end] == '}')
            isInside--;
        end++;
    }
    
    serverBlock = str.substr(start, end - start + 1);
    start = end + 1;
    return serverBlock;
}

ListNode *Parser::parse( std::string str ) {
    ListNode* head = NULL;
    size_t start = 0;
    std::string serverBlock;
    
    while (start < str.length()) {
        serverBlock = extractServer(str, start);
        if (head) {
            head->addNext(Parser::extractBlocks(serverBlock, 0));
        }
        else
            head = Parser::extractBlocks(serverBlock, 0);
    }
    return head;
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

    while (start <= end && std::isspace(str[start]))
        start++;
    while (end >= 0 && std::isspace(str[end]))
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

std::vector<std::string> Parser::strSplit( std::string str, char c ) {
    size_t start;
    size_t end;
    std::vector<std::string> strs;

    while (str.length() > 0)
    {
        start = 0;
        while (start < str.length() && str[start] == c)
            start++;
        if (start >= str.length())
            break;
        end = start;
        while (end < str.length() && str[end] != c)
            end++;
        strs.push_back(str.substr(start, end - start));
        // std::cout << str.substr(start, end - start) << std::endl;

        if (end >= str.length())
            break ;
        str = str.substr(end + 1, str.length() - 1);
    }

    return strs;
}

const char* Parser::ParsingException::what() const throw() {
    return message.c_str();
};

Parser::ParsingException::ParsingException(std::string msg): message(msg) {};

Parser::ParsingException::~ParsingException() throw() {};

void Parser::validateField( std::string key, std::vector<std::string> values ) {
    if (key == "root" || key == "index" || key == "upload_store")
        Parser::validatePathDirective(values, key);
    if (key == "listen")
        Parser::validateListen(values);
    if (key == "limit_except")
        Parser::validateAllowedMethods(values);
    if (key == "client_max_body_size")
        Parser::validateBodySize(values);
    if (key == "error_page")
        Parser::validateErrorPage(values);
    if (key == "autoindex")
        Parser::validateAutoIndex(values);
    if (key == "file_upload")
        Parser::validateFileUpload(values);
    if (key == "cgi_path")
        Parser::validateCgiPath(values);
    if (key == "return")
        Parser::validateRedirection(values);
    
}

void Parser::validatePathDirective( std::vector<std::string> values, std::string name ) {
    if (values.size() != 1)
        throw Parser::ParsingException("Invalid number of arguments for \"" + name + "\"");
}

void Parser::validateCgiPath( std::vector<std::string> values ) {
    if (values.size() < 2 || values.size() % 2 == 1)
        throw Parser::ParsingException("Invalid number of arguments for \"cgi_path\"");
}

void Parser::validateListen( std::vector<std::string> values ) {
    std::string listen;
    std::vector<std::string> splitListen;
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;

    if (values.size() != 1)
        throw Parser::ParsingException("Invalid number of arguments for \"listen\"");
    
    listen = values[0];
    
    // check if it has ':' in it, if so split it, 1st element shoud be numeric
    if (listen.find(":") != listen.npos)
    {
        splitListen = Parser::strSplit(listen, ':');
        if (splitListen.size() != 2 || listen[0] == ':' || listen[listen.size() - 1] == ':')
            throw Parser::ParsingException("Invalid host:port");
        if (!Parser::isNumber(splitListen[1]) || std::stoi(splitListen[1]) < 1 || std::stoi(splitListen[1]) > 65535)
            throw Parser::ParsingException("Invalid port number");
    }
    else {
        if (!Parser::isNumber(listen) || std::stoi(listen) < 1 || std::stoi(listen) > 65535)
            throw Parser::ParsingException("Invalid port number");
    }

    // check host validity
    Parser::ft_memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(splitListen[0].data(), splitListen[1].data(), &hints, &servinfo);
    if (status != 0) {
        throw ParsingException("Error opening host " + splitListen[0]);
    }

    freeaddrinfo(servinfo);
}

void Parser::validateAllowedMethods( std::vector<std::string> values ) {
    if (values.size() == 0)
        throw Parser::ParsingException("Invalid number of arguments for \"limit_except\"");
    for (int i = 0; i < (int) values.size(); i++)
    {
        if (values[i] != "GET"
        &&  values[i] != "POST"
        &&  values[i] != "DELETE")
            throw Parser::ParsingException
            ("Invalid method name \"" + values[i]
            + "\" for \"limit_except\", should be GET, POST or DELETE"
            );
    }
}

void Parser::validateBodySize( std::vector<std::string> values ) {
    std::string size;
    char unit;
    int length;

    if (values.size() != 1)
        throw Parser::ParsingException("Invalid number of arguments for \"client_max_body_size\"");
    if (values[0].length() < 2)
        throw Parser::ParsingException("Invalid argument for \"client_max_body_size\"");
    
    length = values[0].length();
    
    size = values[0].substr(0, length - 1);
    unit = std::toupper(values[0].substr(length - 1, 1)[0]);
    
    if (!Parser::isNumber(size))
        throw Parser::ParsingException("Invalid size for \"client_max_body_size\"");
    if (unit != 'K' && unit != 'M' && unit != 'G')
        throw Parser::ParsingException("Invalid unit for \"client_max_body_size\"");
    
}

void Parser::validateErrorPage( std::vector<std::string> values ) {
    if (values.size() < 2)
        throw Parser::ParsingException("Invalid number of arguments for \"error_page\"");
}

void Parser::validateRedirection( std::vector<std::string> values ) {

    int status = 0;

    if (values.size() != 2)
        throw Parser::ParsingException("Invalid number of arguments for \"return\"");
    if (!Parser::isNumber(values[0]))
        throw Parser::ParsingException("First arg must be numeric for \"return\"");
    status = std::atoi(values[0].data());
    if (status < 300 || status >= 310)
        throw Parser::ParsingException("Invalid redirection status code for \"return\"");
}

void Parser::validateAutoIndex( std::vector<std::string> values ) {
    if (values.size() != 1)
        throw Parser::ParsingException("Invalid number of arguments for \"autoindex\"");
    if (values[0] != "on" && values[0] != "off")
        throw Parser::ParsingException("Invalid arguments for \"autoindex\", value should be 'on' or 'off'");
}

void Parser::validateFileUpload( std::vector<std::string> values ) {
    if (values.size() != 1)
        throw Parser::ParsingException("Invalid number of arguments for \"file_upload\"");
    if (values[0] != "on" && values[0] != "off")
        throw Parser::ParsingException("Invalid arguments for \"file_upload\", value should be 'on' or 'off'");
}

bool Parser::isNumber( std::string str ) {
    for (int i = 0; i < (int) str.length(); i++)
    {
        if (!std::isdigit(str[i]))
            return false;
    }
    return true;
}

void	*Parser::ft_memset(void *b, int c, size_t len)
{
	size_t	i;
    unsigned char *p = (unsigned char *) b;

	i = 0;
	while (i < len)
	{
		p[i] = (unsigned char) c;
		i++;
	}
	return (p);
}
