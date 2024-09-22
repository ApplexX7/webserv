/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNode.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 11:47:25 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/22 09:59:16 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ListNode.hpp"
#include "Field.hpp"

#include <iostream>
#include <vector>
#include <map>

class ServerNode {

    private:
        std::map<std::string, Field > fields;
        std::map<std::string, std::map<std::string, Field > > locations;

    public:
        ServerNode( void );
        ServerNode( ListNode* );
        ServerNode( ServerNode& );
        ServerNode& operator=( ServerNode& );
        ~ServerNode( void );

        void addField( std::string, std::string );
        std::map<std::string, Field > getFields( void );
    
        void addLocationField( std::string, std::string, std::string);
        std::map<std::string, std::map<std::string, Field > > getLocations( void );
    
};