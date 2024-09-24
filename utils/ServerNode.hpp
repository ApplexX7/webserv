/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNode.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 11:47:25 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/24 10:42:33 by wbelfatm         ###   ########.fr       */
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
        ServerNode( void );
        ServerNode( ListNode* );
        ServerNode( ServerNode& );
        ServerNode& operator=( ServerNode& );
        ~ServerNode( void );

        void addField( std::string, std::string );
        std::map<std::string, Field > getFields( void );
    
        void addLocationField( std::string, std::string, std::string);
        std::map<std::string, Location > getLocations( void );

        Field getField( std::string );
};

#endif