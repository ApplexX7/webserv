/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 08:06:05 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/24 11:55:55 by wbelfatm         ###   ########.fr       */
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

        // string utils
        static std::string strTrim( std::string );
        static std::vector<std::string> *strSplit( std::string );

        // general utils
        static ListNode *extractBlocks( std::string, int );
        static bool checkValidList( ListNode *, int );
        static bool checkValidContent( std::string );


        class ParsingException: public std::exception {
            private:
                std::string message;

            public:
                // Constructor accepts a const char* that is used to set
                // the exception message
                explicit ParsingException(std::string msg)
                    : message(msg)
                {
                }

                // Override the what() method to return our message
                virtual const char* what() const throw()
                {
                    return message.c_str();
                }

                virtual ~ParsingException() throw() {}
        };
        
};

#endif