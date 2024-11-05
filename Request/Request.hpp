/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:18:27 by mohilali          #+#    #+#             */
/*   Updated: 2024/11/05 14:58:57 by mohilali         ###   ########.fr       */
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
#include <sys/stat.h>
#include <cstdio>
#include "../utils/Client.hpp"
#include "../cgi/Cgi.hpp"

class Cgi;
class Client;

enum TypeTransf{
    NONE,
    BOUNDARY,
    FIXEDSIZE,
    ENCODING,
};

class Request{
    private:
    // for Cgi
        Cgi *handleCgi;
        char **env;

        bool isaCgi;
        bool finishReading;
        bool compliteHeaderparser;
        long int maxBodySize;
        std::string locationName;
        std::string pathName;
        std::string quertyString;
        ServerNode *listenningServer;
        std::string port;
        std::string hostName;
        std::string methode;
        std::string Uri;
        std::map<std::string,std::string> headers;
        std::string bodybuffer;
        Location serverLocation;

        // for the Post Methode
        int contentLenght;
        TypeTransf bodyType;
        std::string contentType;
        std::string startofBoundary;
        std::string endofBoundary;
        std::string TransferCoding;
    public:
        Request();
        Request(const Request &obj);
        Request& operator=(const Request &ope);
        ~Request();
        
        /*For Cgi*/
        int isCgi();
        int handleEnvForCgi();
        /*******************************/
        int getContentLenght();
        bool getFinishReading();
        bool getIsACgi();
        int requestParserStart(Client &clientData);
        void setpathName(std::string _Name);
        std::string getpathName( void );
        void setHeaders(std::string &name, std::string &value);
        void Setmethode(std::string _metode);
        void setport(std::string _port);
        void setlocationName();
        int ParseRequestLine(std::string &Message, Client &ClientData);
        int ParsingRequestHeaders(std::string RequestHeader);
        Location *getServerLocation();
        std::string getlocationName();
        ServerNode &getserverNode() const;
        int Validmethode(std::string &methode);
        std::string FindHost(std::string HostLine);
        TypeTransf getTheBodyType();
        std::string getStartBoundary();
        std::string getEndofBoundary();
        std::string getTransferCoding();
        void setFinishReading(bool var);
        void    findLocationtobeUsed();

        long int &getClientMaxSizeBody();
    
        void findServer(Client &ClientData);
        // int CheckserverhostName(Client& ClientData);
        //GET parser function
        int CheckDirectory(Client &ClientData);
        
        void sethostName(std::string _hostName);
        void deleteMethode(Client &clientData);
        // POST parse Body
        int ParsePostHeaders();
        int parseBodytype();
        int parseBodyTypeBuffer(std::string &bufferedBody);
    
        std::string  getport();
        std::string gethostName();
        std::string getValue(std::string _Key);
        std::string getmethode();
        std::string getUri();
        std::string &getBody();
        std::map<std::string,std::string> &getHeaders( void );

        void reset( void );
        
        //parsing the request;
        void SetUri(std::string _Uri);
        int ParsingTheRequest(Client& ClientData);
        int parseUri(std::string &uri);
    
        // for debugging
        void printmap();
};

#endif