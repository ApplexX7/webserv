/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 09:43:59 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/10/02 09:50:45 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP

# define CGI_HPP

# include "unistd.h"
# include <iostream> 

class Cgi {
    public:
        Cgi( void );
        ~Cgi( void );

        std::string executeCgi( std::string );
};

#endif