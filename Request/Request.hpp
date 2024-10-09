/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:18:27 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/09 12:15:45 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cctype>
#include <string>
#include <stdexcept>
#include <fstream>
#include <map>
#include "../utils/Client.hpp"

class Client;

class Request{
    private:
        std::string locationName;
        std::string pathName;
        std::string quertyString;
        ServerNode *server;
        std::string port;
        std::string hostName;
        std::string methode;
        std::string Uri;
        std::map<std::string,std::string> headers;
        std::string body;

        // for Post Body
        int contentLenght;
        std::string contentType;
        std::string startofBoundary;
        std::string endofBoundary;
        std::string TransferCoding;
    public:
        Request();
        Request(const Request &obj);
        Request& operator=(const Request &ope);
        ~Request();
        void setpathName(std::string _Name);
        std::string getpathName( void );
        void setBody(std::string _Body);
        void setHeaders(std::string &name, std::string &value);
        void Setmethode(std::string _metode);
        void setport(std::string _port);
        void setserverNode(ServerNode *_serverNode);
        void setlocationName();
        int ParseRequestLine(std::string &Message, Client &ClientData);
        int ParsingRequestHeaders(std::string RequestHeader);
        std::string getlocationName();
        ServerNode &getserverNode() const;
        int Validmethode(std::string &methode);
        std::string FindHost(std::string HostLine);

        int findLocationtobeUsed();
        //checkhostName
        int CheckserverhostName(Client& ClientData);
        //GET parser function
        int CheckDirectory(Client &ClientData);
        
        void sethostName(std::string _hostName);
        // POST parse Body
        int ParsePostHeaders();

    
        std::string  getport();
        std::string gethostName();
        std::string getValue(std::string _Key);
        std::string getmethode();
        std::string getUri();
        std::string getBody();

        //parsing the request;
        int ParsingTheRequest(Client& ClientData);
        // for debugging
        void printmap();
        void SetUri(std::string _Uri);
};

#endif