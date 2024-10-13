/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:25:41 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/10/13 11:51:50 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

# define CHUNK_SIZE 8096

Webserv::Webserv( void ) {
    this->listHead = NULL;
}

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
    // std::cout << "ALL GOOD" << std::endl;
}

Webserv::~Webserv( void ) {
    ListNode::freeListNode(this->listHead);
    
    // free servers
    for (int i = 0; i < (int) this->servers.size(); i++)
    {
        delete this->servers[i];
    }

    std::cout << "WEBSERV DESTRUCTOR CALLED" << std::endl;
}

bool isServerFd(std::vector<int> serverFds, int fd) {
    return (std::find(serverFds.begin(), serverFds.end(), fd) != serverFds.end());
}



void Webserv::listen( void ) {
    std::vector<int> serverFds;
    std::vector<int> clientFds;
    std::string message;

    std::map<int, Client*> clients;
    
    struct pollfd newPollFd;
    int size;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    int new_fd;
    char buf[CHUNK_SIZE + 1];

    addr_size = sizeof(their_addr);

    for (int i = 0; i < (int) this->servers.size(); i++) {
        serverFds.push_back(this->servers[i]->generateServerFd());
    }

    size = serverFds.size();
    std::vector<struct pollfd> fds(size);

    for (int i = 0; i < (int) size; i++)
    {
        fds[i].fd = serverFds[i];
        fds[i].events = POLLIN | POLLOUT | POLLHUP;
    }

    while (1) {
        if (poll(fds.data(), fds.size(), 100) > 0) {
            for (int i = 0; i < (int) fds.size(); i++)
            {
                if (fds[i].revents & POLLHUP)
                {
                    std::cout << "client disconnected " << fds[i].fd << std::endl;
                    delete clients[fds[i].fd];
                    close(fds[i].fd);
                    clientFds.erase(std::remove(clientFds.begin(), clientFds.end(), fds[i].fd), clientFds.end());
                    fds.erase(fds.begin() + i);
                    continue ;
                }

                if (fds[i].revents & POLLIN
                && isServerFd(serverFds, fds[i].fd))
                {
                    new_fd = accept(fds[i].fd, (struct sockaddr *)&their_addr, &addr_size);
                    std::cout << "new connection on " << fds[i].fd << std::endl;
                    newPollFd.events = POLLIN | POLLHUP;
                    clientFds.push_back(new_fd);
                    newPollFd.fd = new_fd;
                    fds.push_back(newPollFd);
                    clients[new_fd] = new Client(this->servers, "", new_fd);
                    clients[new_fd]->setListen(this->getServerByFd(fds[i].fd)->getListenField());
                }
                else if (!isServerFd(serverFds, fds[i].fd)
                && fds[i].revents & POLLIN)
                {
                    int bytes_read;
                    bytes_read = recv(fds[i].fd, buf, CHUNK_SIZE, MSG_EOF);

                    if (bytes_read == -1)
                    {
                        std::cout << "error reading" << std::endl;
                        exit(0);
                    }

                    else if (bytes_read < CHUNK_SIZE)
                    {
                        buf[bytes_read] = 0;
                        message.assign(buf);
                        clients[fds[i].fd]->appendMessage(message);

                        std::cout << clients[fds[i].fd]->getMessage() << std::endl;

                        
                        clients[fds[i].fd]->getRequest().ParsingTheRequest(*clients[fds[i].fd]);

                        // std::vector<std::string> vals = clients[fds[i].fd]->getParentServer().getField("listen").getValues();

                        fds[i].events = POLLOUT | POLLHUP;
                    }
                    else
                    {
                        buf[bytes_read] = 0;
                        message.assign(buf);
                        clients[fds[i].fd]->appendMessage(message);
                    }
                }
                else if (!isServerFd(serverFds, fds[i].fd)
                && fds[i].revents & POLLOUT)
                {
                    clients[fds[i].fd]->findParentServer();

                    // send response
                    std::string res = clients[fds[i].fd]->getResponse().createGetResponse();

                    send(fds[i].fd, res.data(), res.size(), MSG_SEND);
                    // std::cout << "sent: " << send(fds[i].fd, res.data(), res.size(), MSG_SEND) << std::endl;

                    // reset message 
                    clients[fds[i].fd]->setMessage("");

                    std::string connection = clients[fds[i].fd]->getRequest().getValue("Connection");
                    
                    if (clients[fds[i].fd]->getResponse().getStatus() == FINISHED) {
                        if (connection == "keep-alive"
                        && clients[fds[i].fd]->getResponse().getStatusCode() < 400) {
                            fds[i].events = POLLIN | POLLHUP;
                            clients[fds[i].fd]->getResponse().reset();
                        }
                        else {
                            delete clients[fds[i].fd];
                            close(fds[i].fd);
                            clientFds.erase(std::remove(clientFds.begin(), clientFds.end(), fds[i].fd), clientFds.end());
                            fds.erase(fds.begin() + i);
                        }
                    }
                }

                
            }
        }
    }
    std::cout << "\n\nCoucou\n\n" << std::endl;
}

ServerNode *Webserv::getServerByFd( int fd ) {
    for (int i = 0; i < (int) this->servers.size(); i++) {
        if (this->servers[i]->getFd() == fd)
            return (this->servers[i]);
    }
    return NULL;
}