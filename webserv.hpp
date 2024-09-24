/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 08:14:22 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/24 22:30:09 by wbelfatm         ###   ########.fr       */
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
        Webserv( std::string );
        ~Webserv( void );
};

#endif