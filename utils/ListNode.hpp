/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListNode.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 14:09:18 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/07 14:56:13 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_NODE

# define LIST_NODE

#include <map>
#include <iostream>

class ListNode {

    private:
        ListNode *next;
        ListNode *child;
        std::string content;       

    public:
        ListNode( void );
        ListNode( std::string );
        ListNode( ListNode& );
        ListNode& operator=( ListNode& );
        ~ListNode( void );
        
        void addNext( ListNode* );
        void addChild( ListNode* );
        std::string getContent( void );
        ListNode *getNext( void );
        ListNode *getChild( void );
    

};

#endif