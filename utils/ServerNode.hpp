/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNode.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 11:47:25 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/20 12:10:24 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ListNode.hpp"

#include <iostream>
#include <vector>
#include <map>

class ServerNode {

    private:
        std::map<std::string, std::vector<std::string>> fields;
        std::map<std::string, std::vector<std::string>> locations;

    public:
        ServerNode( void );
        ServerNode( ListNode* );
        ServerNode( ServerNode& );
        ServerNode& operator=( ServerNode& );
        ~ServerNode( void );
    
    
};