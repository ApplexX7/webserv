/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 08:14:22 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/29 10:03:15 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP

# define WEBSERV_HPP

# include <iostream>
# include <vector>
# include "parsing/Parser.hpp"
# include "utils/ServerNode.hpp"
# include "utils/ListNode.hpp"


class Webserv {
    private:
        ListNode* listHead;
        std::vector<ServerNode*> servers;
    
    public:
        Webserv( void );
        ~Webserv( void );

        void init( std::string );
};

#endif