/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNode.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 11:47:25 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/11/12 12:08:46 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_NODE_HPP

# define SERVER_NODE_HPP

#include "ListNode.hpp"
#include "Field.hpp"
#include "Location.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

class Location;

class ServerNode {

    private:
        std::map<std::string, Field > fields;
        std::map<std::string,  Location> locations;
        int fd;
        unsigned long long maxSize;

    public:
        std::map<std::string, std::string > cgiPaths;
        std::vector<std::string> allowedFields;
        std::vector<std::string> allowedLocationFields;
        
        ServerNode( void );
        ServerNode( ListNode* );
        ServerNode( ServerNode& );
        ServerNode& operator=( ServerNode& );
        ~ServerNode( void );

        void initializeServer( ListNode* );
        void initializeLocation( ListNode* );

        void addField( std::string, std::string );
        std::map<std::string, Field > getFields( void );
    
        void addLocationField( std::string, std::string, std::string);
        void addLocationCgi( std::string, std::string, std::string);
        void addCgi( std::string, std::string);
        std::map<std::string, Location>& getLocations( void );

        Field& getField( std::string );
        bool fieldExists( std::string );
        bool locationExists( std::string );
        bool locationFieldExists( std::string, std::string );

        int generateServerFd( void );
        int getFd( void );
        void setFd( int );
        void setMaxBodySize( void );
        unsigned long long getMaxBodySize( void ) const;

        std::string getListenField( void );

        class SocketException: public std::exception {
            private:
                std::string message;

            public:
                SocketException(std::string);

                virtual const char* what() const throw();

                virtual ~SocketException() throw();
        };
};

#endif