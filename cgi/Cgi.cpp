/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 09:45:37 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/10/02 09:54:02 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

Cgi::Cgi( void ) {};
Cgi::~Cgi( void ) {};

std::string Cgi::executeCgi( std::string req ) {
    
    // int fds[2];

    // pipe(fds);
    // pid_t pid = fork();

    // if (pid) {
    //     close(fds[0]);
        
    //     dup2(fds[1], STDOUT_FILENO);

    //     write(0, req.data(), req.length());
        
        
    // }
    // else {
        
    // }

    return "";
}