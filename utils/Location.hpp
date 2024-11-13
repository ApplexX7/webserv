/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 09:30:53 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/11/13 10:59:25 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP

# define LOCATION_HPP

#include "Field.hpp"
#include "ServerNode.hpp"
#include <map>

class ServerNode;

class Location {
    private:
        std::map<std::string, Field > fields;
        std::map<std::string, std::string > cgiPaths;
        unsigned long long maxSize;
        
        ServerNode* server;

    public:
        Location( void );
        Location( ServerNode* );
        
        void setServer( ServerNode* server );
        std::map<std::string, Field > getFields( void );
        void addField( std::string, std::string );
        Field &getField( std::string );
        void addCgiPath( std::string, std::string );
        std::string getCgiPath( std::string );
        bool checkLocationFieldExists( std::string );

        void setMaxBodySize( void );
        unsigned long long getMaxBodySize( void ) const;
};

#endif