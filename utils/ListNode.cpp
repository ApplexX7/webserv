/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListNode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 14:15:55 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/13 09:28:09 by wbelfatm         ###   ########.fr       */
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
    ListNode* tmp = this;

    while (tmp->next)
        tmp = tmp->next;
    tmp->next = next;
}

void ListNode::addChild( ListNode* child )
{
    if (!this->child)
    {
        this->child = child;
        return ;
    }
    
    this->child->addNext(child);
}

void ListNode::addField( std::string field )
{
    this->fields.push_back(field);
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

std::vector<std::string> ListNode::getFields( void )
{
    return this->fields;
}