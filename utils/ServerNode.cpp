/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 11:47:25 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/20 12:12:31 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerNode.hpp"

// canonical form

ServerNode::ServerNode( void ) {};

ServerNode::ServerNode( ListNode *server ) {
    // loop through the server fields, get the first word as key and others as value

    // loop through children set second word as key and all other fields as value
};

ServerNode::ServerNode( ServerNode& cpy ) {
    (void) cpy;
};

ServerNode& ServerNode::operator=( ServerNode& rhs ) {
    return rhs;
    
};

ServerNode::~ServerNode( void ) {};

