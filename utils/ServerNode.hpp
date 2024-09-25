/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNode.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 11:47:25 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/25 17:19:08 by wbelfatm         ###   ########.fr       */
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

class Location;

class ServerNode {

    private:
        std::map<std::string, Field > fields;
        std::map<std::string,  Location> locations;

    public:
        std::vector<std::string> allowedFields;
        std::vector<std::string> allowedLocationFields;
        
        ServerNode( void );
        ServerNode( ListNode* );
        ServerNode( ServerNode& );
        ServerNode& operator=( ServerNode& );
        ~ServerNode( void );

        void initializeServer( ListNode* );

        void addField( std::string, std::string );
        std::map<std::string, Field > getFields( void );
    
        void addLocationField( std::string, std::string, std::string);
        std::map<std::string, Location > getLocations( void );

        Field getField( std::string );
        bool fieldExists( std::string );
        bool locationExists( std::string );
        bool locationFieldExists( std::string, std::string );
};

#endif