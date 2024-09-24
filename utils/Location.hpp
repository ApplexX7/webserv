/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 09:30:53 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/24 10:09:10 by wbelfatm         ###   ########.fr       */
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
        ServerNode* server;

    public:
        Location( void );
        Location( ServerNode* );
        
        void setServer( ServerNode* server );
        std::map<std::string, Field > getFields( void );
        void addField( std::string, std::string );
};

#endif