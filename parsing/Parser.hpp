/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 08:06:05 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/20 08:30:21 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP

# define PARSER_HPP

#include <iostream>
#include "../utils/ListNode.hpp"

class Parser {
    private:
        std::string _fileName;
        std::string _fileContent;

    public:
        Parser( void );
        Parser( std::string );
        Parser( Parser& );
        ~Parser( void );
        Parser& operator=( Parser& );

        void loadFile( void );
        std::string getContent( void );

        static ListNode *extractBlock( std::string, int );
        static std::string strTrim( std::string );
        static bool checkValid( ListNode * );
};

#endif