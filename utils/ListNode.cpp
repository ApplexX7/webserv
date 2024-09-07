/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListNode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 14:15:55 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/07 15:29:43 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListNode.hpp"

ListNode::ListNode( void ) {};

ListNode::ListNode( std::string content )
{
    this->content = content;
    this->child = NULL;
    this->next = NULL;
}

ListNode::ListNode( ListNode& cpy ) {
    (void) cpy;
};

ListNode& ListNode::operator=( ListNode& rhs ) {
    return rhs;
    
};

ListNode::~ListNode( void ) {};

void ListNode::addNext( ListNode* next )
{
    this->next = next;
}

void ListNode::addChild( ListNode* child )
{
    // std::cout << "Child added: " + child->getContent() << std::endl;
    this->child = child;
}

std::string ListNode::getContent( void )
{
    return this->content;
}

ListNode* ListNode::getNext( void )
{
    return this->next;
}

ListNode* ListNode::getChild( void )
{
    return this->child;
}