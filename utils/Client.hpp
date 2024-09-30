/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:11:02 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/30 15:46:36 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP

# define CLIENT_HPP

#include <iostream>

class Client {
    public:
        Client( void );
        ~Client( void );

        std::string getMessage( void );
        void setMessage( std::string );
        void appendMessage( std::string );

        int getFd( void );
        void setFd( int );

        

    private:
        std::string message;
        int fd;

        // Req
        // Res
};

#endif