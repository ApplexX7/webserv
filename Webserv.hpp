/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 08:14:22 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/11/10 13:26:24 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP

# define WEBSERV_HPP

# include <iostream>
# include <vector>
# include <map>
# include "parsing/Parser.hpp"
# include "utils/ServerNode.hpp"
# include "utils/ListNode.hpp"
# include "utils/Client.hpp"
# include <sys/poll.h>

class Webserv {
    private:
        ListNode* listHead;
        std::vector<ServerNode*> servers;
        std::vector<int> clientFds;
    
    public:
        Webserv( void );
        ~Webserv( void );

        void init( std::string );
        void listen( void );
        ServerNode *getServerByFd( int );

        class ServerException: public std::exception {
            private:
                std::string message;

            public:
                ServerException(std::string);

                virtual const char* what() const throw();

                virtual ~ServerException() throw();
        };
        
};

#endif