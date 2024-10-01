/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:11:02 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/10/01 09:11:03 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP

# define CLIENT_HPP

#include <iostream>
#include "ServerNode.hpp"

class Client {
    public:
        Client( std::vector<ServerNode*>& );
        Client( std::vector<ServerNode*>&, std::string, int );
        ~Client( void );

        std::string getMessage( void );
        void setMessage( std::string );
        void appendMessage( std::string );

        int getFd( void );
        void setFd( int );

        bool isResponseReady( void );
        bool isKeepAlive( void );

        std::vector<ServerNode*> &getServers( void );

    private:
        std::vector<ServerNode*> &servers;
        std::string message;
        int fd;
        bool keepAlive;
        bool responseReady;

        // Req
        // Res
};

#endif