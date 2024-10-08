/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 08:14:22 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/10/07 10:03:02 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP

# define WEBSERV_HPP

# include <iostream>
# include <vector>
# include "parsing/Parser.hpp"
# include "utils/ServerNode.hpp"
# include "utils/ListNode.hpp"
# include "utils/Client.hpp"
# include <sys/poll.h>


class Webserv {
    private:
        ListNode* listHead;
        std::vector<ServerNode*> servers;
    
    public:
        Webserv( void );
        ~Webserv( void );

        void init( std::string );
        void listen( void );
        ServerNode *getServerByFd( int );
};

#endif