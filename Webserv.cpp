/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:25:41 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/11/10 11:57:53 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

# define CHUNK_SIZE 1024

Webserv::Webserv( void ) {
    this->listHead = NULL;
}

// void printChilds( ListNode *child)
// {
//     std::vector<std::string> fields;
//     while (child)
//     {
//         std::cout << "\tchild start: " + child->getContent() << std::endl;
//         fields = child->getFields();
//         for (size_t i = 0; i < fields.size(); i++)
//             std::cout << "\t\t" << fields[i] << std::endl;
//         child = child->getNext();
//     }
// }

// void printServerNode(ListNode* server) {
//     ServerNode n(server);
//     std::map<std::string, Field > fields;

//     std::map<std::string, Field > f = n.getFields();

//     std::map<std::string, Field >::iterator it;
//     for (it = f.begin(); it != f.end(); it++)
//     {
//         std::cout << it->first << " => ";
//         for (int i = 0; i < (int) it->second.getValues().size(); i++) {
//             std::cout << it->second.getValues()[i] << " ";
//         }
//         std::cout << std::endl;
//     }

//     std::map<std::string, Location > locations = n.getLocations();

//     std::map<std::string, Location >::iterator loc_it;
        
    
//     for (loc_it = locations.begin(); loc_it != locations.end(); loc_it++) {
//         std::cout << loc_it->first << ": " << std::endl;
//         fields = loc_it->second.getFields();
//         for (it = fields.begin(); it != fields.end(); it++) {
//             std::cout << "\t" << it->first << " => ";
//             for (int i = 0; i < (int) it->second.getValues().size(); i++) {
//                 std::cout << it->second.getValues()[i] << " ";
//             }
//             std::cout << std::endl;
//         }
//     }
// }


void printVector(std::vector<std::string> arr) {
    for (int i = 0; i < (int) arr.size(); i++) {
        std::cout << arr[i] << " ---- ";
    }
    std::cout << std::endl;
}

/*
    - Initializes the servers
    - Displays warning in the case of servers with conflicting ports/names
    
*/

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
    std::map<std::string, std::vector<std::string> > servers;
    std::map<std::string, std::vector<std::string> >::iterator it;
    int occurences = 0;

    if (Parser::checkValidContent(content) == false)
        throw Parser::ParsingException("Invalid braces in config file");

    head = parser.parse(parser.getContent());
    this->listHead = head;
    if (Parser::checkValidList(head, 0) == false)
        throw Parser::ParsingException("Invalid server block name");

    tmp = head;

    while (tmp) {
        // printServerNode(tmp);
        this->servers.push_back(new ServerNode(tmp));

        // check servername conflicts
        listenValue = this->servers[this->servers.size() - 1]->getField("listen").getValues()[0];
        serverNames = this->servers[this->servers.size() - 1]->getField("server_name").getValues();
        for (int j = 0; j < (int) serverNames.size(); j++) {
            if (std::count(servers[listenValue].begin(), servers[listenValue].end(), serverNames[j]) > 1)
               continue ; 
            servers[listenValue].push_back(serverNames[j]);
        }
        tmp = tmp->getNext();
    }

    for (it = servers.begin(); it != servers.end(); it++) {
        serverNames = it->second;
        for (int i = 0; i < (int) serverNames.size(); i++) {
            occurences = std::count(serverNames.begin(), serverNames.end(), serverNames[i]);
            if (occurences > 1) {
                std::cout << "[WARN] Conflicting server name \"|" << serverNames[i] << "|\" at " << it->first << std::endl;
                std::string tmp = serverNames[i];
                while (occurences >= 1)
                {
                    serverNames.erase(std::find(serverNames.begin(), serverNames.end(), tmp));
                    occurences = std::count(serverNames.begin(), serverNames.end(), tmp);
                }
                i = -1;
            }
        }
        servers[it->first] = serverNames;
    }
}



bool isServerFd(std::vector<int> serverFds, int fd) {
    return (std::find(serverFds.begin(), serverFds.end(), fd) != serverFds.end());
}

void disconnectClient(
    std::map<int, Client*>& clients,
    std::vector<struct pollfd>& fds,
    std::vector<int> &clientFds,
    int i
    ) {

    delete clients[fds[i].fd];
    close(fds[i].fd);
    clientFds.erase(std::remove(clientFds.begin(), clientFds.end(), fds[i].fd), clientFds.end());
    fds.erase(fds.begin() + i);
}


/*
    - Starts up the web server
    - Opens socket for each server
    - Listens for events on sockets using poll (new connection, input, output, disconnection)
    - Parses Requests and generates responses
*/

void Webserv::listen( void ) {
    std::vector<int> serverFds;
    std::vector<int> &clientFds = this->clientFds;
    std::string message;
    std::map<int, Client*> clients;

    struct pollfd newPollFd;
    int size;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    int new_fd;
    char buf[CHUNK_SIZE + 1];
    int bytes_read;
    std::string res;

    addr_size = sizeof(their_addr);
    
    // open server sockets
    for (int i = 0; i < (int) this->servers.size(); i++) {
        try {
            serverFds.push_back(this->servers[i]->generateServerFd());
        } catch (ServerNode::SocketException& e) {
            std::cerr << e.what() << std::endl;
            return ;
        }
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
                // client disconnected
                if (fds[i].revents & POLLHUP)
                {
                    std::cout << "client disconnected " << fds[i].fd << std::endl;
                    disconnectClient(clients, fds, clientFds, i);
                    continue ;
                }

                // client wants to connect
                if (fds[i].revents & POLLIN
                && isServerFd(serverFds, fds[i].fd))
                {
                    new_fd = accept(fds[i].fd, (struct sockaddr *)&their_addr, &addr_size);

                    if (new_fd < 0)
                    {
                        std::cout << "Error opening connection on " << fds[i].fd << std::endl;
                    }
                    else {
                        // std::cout << "new connection on " << new_fd << std::endl;
                        newPollFd.events = POLLIN | POLLHUP;
                        clientFds.push_back(new_fd);
                        newPollFd.fd = new_fd;
                        fds.push_back(newPollFd);
                        clients[new_fd] = new Client(this->servers, "", new_fd);
                        clients[new_fd]->setListen(this->getServerByFd(fds[i].fd)->getListenField());
                    }
                }

                // client wants to send data
                else if (!isServerFd(serverFds, fds[i].fd)
                && fds[i].revents & POLLIN)
                {
                    // read from client
                    bytes_read = recv(fds[i].fd, buf, CHUNK_SIZE, 0);
                    if (bytes_read < 0)
                    {
                        std::cout << "error reading" << std::endl;
                        bytes_read = 0;
                    }
                    
                    buf[bytes_read] = 0;
                    message.assign(buf, bytes_read);
                    clients[fds[i].fd]->setMessage(message);
                    clients[fds[i].fd]->getRequest().requestParserStart(*clients[fds[i].fd]);
                    if ((*clients[fds[i].fd]).getRequest().getFinishReading())
                    {
                        // std::cout << "client finished writing on: " << (*clients[fds[i].fd]).getRequest().getUri() << std::endl;
    
                        // find server responsible for this client
                        clients[fds[i].fd]->findParentServer();

                        // listen for client readiness to receive
                        fds[i].events = POLLOUT;
                    }
                }

                // client wants ready to receive data
                else if (!isServerFd(serverFds, fds[i].fd)
                && fds[i].revents & POLLOUT)
                {
                    if (clients[fds[i].fd]->getRequest().getIsACgi())
                        clients[fds[i].fd]->getRequest().requestParserStart(*clients[fds[i].fd]);
                    if ((clients[fds[i].fd]->responseReady)) {
                        // send response
                        res = clients[fds[i].fd]->getResponse().generateResponse();

                        send(fds[i].fd, res.data(), res.size(), MSG_SEND);

                        // reset message
                        clients[fds[i].fd]->setMessage("");

                        std::string connection = clients[fds[i].fd]->getRequest().getValue("Connection");

                        // finished sending response
                        if (clients[fds[i].fd]->getResponse().getStatus() == FINISHED) {
                            if (connection == "keep-alive") {
                                fds[i].events = POLLIN | POLLHUP;
                                clients[fds[i].fd]->getResponse().reset();
                                clients[fds[i].fd]->responseReady = false;
                                clients[fds[i].fd]->getRequest().reset();
                            }
                            else
                                disconnectClient(clients, fds, clientFds, i);
                        }
                    }
            }
        }
    }
    }
}

ServerNode *Webserv::getServerByFd( int fd ) {
    for (int i = 0; i < (int) this->servers.size(); i++) {
        if (this->servers[i]->getFd() == fd)
            return (this->servers[i]);
    }
    return NULL;
}

/*
    - Frees up parsing linked list's resources
    - Closes server sockets
*/
Webserv::~Webserv( void ) {
    ListNode::freeListNode(this->listHead);
    
    // free servers
    for (int i = 0; i < (int) this->servers.size(); i++)
    {
        close(this->servers[i]->getFd());
        delete this->servers[i];
    }

    for (int i = 0; i < (int) this->clientFds.size(); i++)
        close(this->clientFds[i]);

    std::cout << "WEBSERV DESTRUCTOR CALLED" << std::endl;
}