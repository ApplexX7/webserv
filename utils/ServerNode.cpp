/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 11:47:25 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/10/27 11:15:30 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerNode.hpp"
#include "../parsing/Parser.hpp"
#include <arpa/inet.h>

void ServerNode::initializeServer( ListNode* server ) {
    std::map<std::string,  Location>::iterator it;
    std::vector<std::string> fields = server->getFields();
    std::vector<std::string> splitField;
    std::string path;
    std::string trimedField;
    std::string sizeStr;
    unsigned long long size;
    char unit;
    int length;

    ListNode *child = server->getChild();

    for (int i = 0; i < (int) fields.size(); i++) {
        trimedField = Parser::strTrim(fields[i]);
        if (trimedField.length() == 0)
            continue;
        splitField = (Parser::strSplit(trimedField, ' '));
        if (splitField.size() == 1)
            throw Parser::ParsingException("Directive has no arguments " + splitField[0]);
        if (splitField[0] != "error_page" && this->fieldExists(splitField[0]))
            throw Parser::ParsingException("Duplicate directives for " + splitField[0]);

        for (int j = 1; j < (int) splitField.size(); j++) {

            // check for status code format in case of error pages
            if (splitField[0] == "error_page")
            {
                if (j < (int) splitField.size() - 1
                && !Parser::isNumber(splitField[j]))
                    throw Parser::ParsingException("Status code must be numeric");
                if (j < (int) splitField.size() - 1
                && (std::stol(splitField[j]) < 300 || std::stol(splitField[j]) > 599))
                    throw Parser::ParsingException("Status code must be between 300 and 599");
            }

            this->addField(splitField[0], splitField[j]);
        }

        std::vector<std::string> values = this->getField(splitField[0]).getValues();

        // Parser::validateField(splitField[0], values);
        if (splitField[0] == "listen" && values[0].find(":") == values[0].npos)
            this->getField(splitField[0]).updateValue("127.0.0.1:" + values[0], 0);
        Parser::validateField(splitField[0], this->getField(splitField[0]).getValues());
    }

    // if server has no listen directive
    if (!this->fieldExists("listen"))
        this->addField("listen", "127.0.0.1:8080");
    
    // if server has no server_name
    if (!this->fieldExists("server_name"))
        this->addField("server_name", "");

    // if server has no root
    if (!this->fieldExists("root"))
        this->addField("root", "/Users/wbelfatm");

    // if server has no autoindex
    if (!this->fieldExists("autoindex"))
        this->addField("autoindex", "off");
    
    // if server has no max size
    if (!this->fieldExists("client_max_body_size"))
        this->addField("client_max_body_size", "1m");

    // insert locations
    while (child != NULL)
    {
        fields = child->getFields();
        splitField = (Parser::strSplit(child->getContent(), ' '));
        if (splitField.size() != 2 || splitField[0] != "location")
            throw Parser::ParsingException("Expected location directive but got \"" + splitField[0] + "\"");

        path = splitField[1];
        if (this->locationExists(path))
            throw Parser::ParsingException("Duplicate locations for path " + path);

        for (int i = 0; i < (int) fields.size(); i++) {
            trimedField = Parser::strTrim(fields[i]);
            if (trimedField.length() == 0)
                continue ;
            splitField = (Parser::strSplit(trimedField, ' '));
            if (splitField.size() == 1)
                throw Parser::ParsingException("Directive has no arguments " + splitField[0]);
            if (splitField[0] != "error_page" && this->locationFieldExists(path, splitField[0]))
            {
                throw Parser::ParsingException("Duplicate directives for " + splitField[0]);
            }

            for (int j = 1; j < (int) splitField.size(); j++) {
                // check for status code format in case of error pages
                if (splitField[0] == "error_page")
                {
                    if (j == (int) splitField.size() - 1
                    && Parser::isNumber(splitField[j]))
                        throw Parser::ParsingException("Invalid path for error pages");
                    if (j < (int) splitField.size() - 1
                    && !Parser::isNumber(splitField[j]))
                        throw Parser::ParsingException("Status code must be numeric");
                    if (j < (int) splitField.size() - 1
                    && (std::stol(splitField[j]) < 300 || std::stol(splitField[j]) > 599))
                        throw Parser::ParsingException("Status code must be between 300 and 599");
                }
                this->addLocationField(path, splitField[0], splitField[j]);
            }

            Parser::validateField(splitField[0], this->locations[path].getField(splitField[0]).getValues());
        }
        this->locations[path].setServer(this);
        child = child->getNext();
    }
    
    length = this->getField("client_max_body_size").getValues()[0].length();
    sizeStr = this->getField("client_max_body_size").getValues()[0].substr(0, length - 1);
    unit = std::toupper(this->getField("client_max_body_size").getValues()[0].substr(length - 1, 1)[0]);
    
    size = std::stoull(sizeStr);

    if (unit == 'K')
        size *= 1024;
    if (unit == 'M')
        size *= 1024 * 1024;
    if (unit == 'G')
        size *= 1024 * 1024 * 1024;
    this->maxSize = size;
}

// canonical form
ServerNode::ServerNode( void ) {};

ServerNode::ServerNode( ServerNode& cpy ) {
    (void) cpy;
};

ServerNode& ServerNode::operator=( ServerNode& rhs ) {
    return rhs;
};

ServerNode::~ServerNode( void ) {};

void setAllowedFields( ServerNode* server ) {
    // allowed server fields
    server->allowedFields.push_back("root");
    server->allowedFields.push_back("listen");
    server->allowedFields.push_back("server_name");
    server->allowedFields.push_back("client_max_body_size");
    server->allowedFields.push_back("error_page");
    server->allowedFields.push_back("autoindex");

    // allowed location fields
    server->allowedLocationFields.push_back("root");
    server->allowedLocationFields.push_back("client_max_body_size");
    server->allowedLocationFields.push_back("limit_except");
    server->allowedLocationFields.push_back("autoindex");
    server->allowedLocationFields.push_back("file_upload");
    server->allowedLocationFields.push_back("error_page");
}

ServerNode::ServerNode( ListNode *server ) {
    this->fd = -1;
    setAllowedFields(this);
    initializeServer(server);
};

void ServerNode::addField( std::string key, std::string value) {
    if (std::find(
        this->allowedFields.begin(),
        this->allowedFields.end(),
        key) == this->allowedFields.end())
        throw Parser::ParsingException("Unknown directive " + key);
    this->fields[key].addValue(value);
}

void ServerNode::addLocationField( std::string path, std::string key, std::string value) {
    if (std::find(
        this->allowedLocationFields.begin(),
        this->allowedLocationFields.end(),
        key) == this->allowedLocationFields.end())
        throw Parser::ParsingException("Unknown directive " + key + " for location block");
    this->locations[path].addField(key, value);
}

std::map<std::string, Field > ServerNode::getFields( void ) {
    return this->fields;
}

std::map<std::string, Location >& ServerNode::getLocations( void ) {
    return this->locations;
}

Field& ServerNode::getField( std::string key ) {
    return this->fields[key];
}

bool ServerNode::fieldExists( std::string key ) {
    if (this->fields.find(key) != this->fields.end())
        return true;
    return false;
}

bool ServerNode::locationExists( std::string path ) {
    if (this->locations.find(path) != this->locations.end())
        return true;
    return false;
}

bool ServerNode::locationFieldExists( std::string path, std::string key ) {
    std::map<std::string, Field > fields =  this->locations[path].getFields();

    if (fields.find(key) != fields.end())
        return true;
    return false;
}

/*
    - Opens a new socket and returns its fd
*/

int ServerNode::generateServerFd( void ) {
    std::vector<std::string> splitListen;
    struct addrinfo hints;
    struct addrinfo *servinfo;
    int sockfd;
    int optval;

    splitListen = Parser::strSplit(this->fields["listen"].getValues()[0], ':');
    if (splitListen[0] == "localhost")
        splitListen[0] = "127.0.0.1";
    Parser::ft_memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV | AI_CANONNAME;
    getaddrinfo(splitListen[0].data(), splitListen[1].data(), &hints, &servinfo);
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        freeaddrinfo(servinfo);
        throw ServerNode::SocketException("Error opening server socket");
    }

    optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    // bind socket to port and address
    if (bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) != 0)
    {
        freeaddrinfo(servinfo);
        close(sockfd);
        throw ServerNode::SocketException("Error binding server socket for: " + splitListen[0] + ":" + splitListen[1]);
        return -1;
    }

    // start listening on socket
    if (listen(sockfd, SOMAXCONN) != 0)
    {
        freeaddrinfo(servinfo);
        close(sockfd);
        throw ServerNode::SocketException("Error listening on server socket");
        return -1;
    }
    freeaddrinfo(servinfo);
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    this->fd = sockfd;
    return (sockfd);
}

int ServerNode::getFd( void ) {
    return this->fd;
}

void ServerNode::setFd( int fd ) {
    this->fd = fd;
}

std::string ServerNode::getListenField( void ) {
    std::string listen = this->getField("listen").getValues()[0];
    return listen;
}

const char* ServerNode::SocketException::what() const throw() {
    return message.c_str();
};

ServerNode::SocketException::SocketException(std::string msg): message(msg) {};

ServerNode::SocketException::~SocketException() throw() {};