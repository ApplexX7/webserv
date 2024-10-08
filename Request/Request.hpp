/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:18:27 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/07 21:48:43 by mohilali         ###   ########.fr       */
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
        bool Location;
        std::string LocationName;
        std::string PathName;
        std::string Querystring;
        ServerNode *Server;
        std::string Port;
        std::string HostName;
        std::string Methode;
        std::string Uri;
        std::map<std::string,std::string> headers;
        std::string body;
    public:
        Request();
        Request(const Request &obj);
        Request& operator=(const Request &ope);
        ~Request();
        void setPathName(std::string _Name);
        std::string getPathName( void );
        void setBody(std::string _Body);
        void setHeaders(std::string &name, std::string &value);
        void Setmethode(std::string _metode);
        void setPort(std::string _Port);
        void setServerNode(ServerNode *_ServerNode);
        void setLocationName();
        int ParseRequestLine(std::string Message, Client &ClientData);
        int ParsingRequestHeaders(std::string RequestHeader);
        std::string getLocationName();
        ServerNode &getServerNode() const;
        int ValidMethode(std::string &Methode);
        std::string FindHost(std::string HostLine);
        std::string FindLocationtobeUsed(Client &ClientData);
        //checkHostName
        int CheckServerHostName(Client& ClientData);
        //GET parser function
        int CheckDirectory(Client &ClientData);
        
        void setHostName(std::string _HostName);
        
        std::string  getPort();
        std::string getHostName();
        std::string getValue(std::string _Key);
        std::string getMethode();
        std::string getUri();
        std::string getBody();

        //parsing the request;
        int ParsingTheRequest(Client& ClientData);
        // for debugging
        void printmap();
        void SetUri(std::string _Uri);
};

#endif