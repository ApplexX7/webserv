/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListNode.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 14:09:18 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/13 09:28:04 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_NODE

# define LIST_NODE

#include <map>
#include <iostream>
#include <vector>

class ListNode {

    private:
        std::vector<std::string> fields;
        std::string content;
        ListNode *next;
        ListNode *child;

    public:
        ListNode( void );
        ListNode( std::string );
        ListNode( ListNode& );
        ListNode& operator=( ListNode& );
        ~ListNode( void );
        
        void addNext( ListNode* );
        void addChild( ListNode* );
        void addField( std::string );
        std::string getContent( void );
        ListNode *getNext( void );
        ListNode *getChild( void );
        std::vector<std::string> getFields( void );
};

#endif