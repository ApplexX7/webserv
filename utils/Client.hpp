/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:11:02 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/10/08 13:40:44 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP

# define CLIENT_HPP

class Request;
#include <iostream>
#include "ServerNode.hpp"
#include "../Request/Request.hpp"
#include "../Response/Response.hpp"

class Response;

class Client {
    public:
        Client( std::vector<ServerNode*>& );
        Client( std::vector<ServerNode*>&, std::string, int );
        ~Client( void );

        std::string getMessage( void );
        void setMessage( std::string );
        void appendMessage( std::string );

        void setListen( std::string );
        std::string getListen( void );

        int getFd( void );
        void setFd( int );

        bool isResponseReady( void );
        bool isKeepAlive( void );

        std::vector<ServerNode*> &getServers( void );

        void findParentServer( void );
        ServerNode& getParentServer( void );

        Request &getRequest( void );
        Response &getResponse( void );

    private:
        std::vector<ServerNode*> &servers;
        ServerNode* parentServer;
        std::string message;
        std::string listen;
        int fd;
        bool keepAlive;
        bool responseReady;

        // Req
        Request *requestMessage;
    
        // Res 
        Response *responseMessage;
};

#endif