/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 08:06:07 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/05 08:12:59 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser( void ) {};

Parser::Parser( Parser& copy ){
    (void) copy;
};

Parser::~Parser( void ) {};

Parser& Parser::operator=( Parser& rhs ){
    return rhs;
};