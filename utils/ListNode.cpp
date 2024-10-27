/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListNode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 14:15:55 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/10/22 20:52:51 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListNode.hpp"
#include <vector>

ListNode::ListNode( void ) {
    this->child = NULL;
    this->next = NULL;
};

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

void ListNode::setContent( std::string content ) {
    this->content = content;
}

ListNode* ListNode::getNext( void )
{
    return this->next;
}

ListNode* ListNode::getChild( void )
{
    return this->child;
}

bool ListNode::hasNext( void )
{
    return this->next != NULL;
}

bool ListNode::hasChild( void )
{
    return this->child != NULL;
}

std::vector<std::string> ListNode::getFields( void )
{
    return this->fields;
}

void ListNode::freeListNode( ListNode *head ) {
    if (!head)
        return ;
    ListNode::freeListNode(head->getChild());
    ListNode::freeListNode(head->getNext());
    delete head;
}