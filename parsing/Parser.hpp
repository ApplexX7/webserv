/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 08:06:05 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/10/22 20:43:21 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP

# define PARSER_HPP

#include <iostream>
#include "../utils/ListNode.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

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
        static std::vector<std::string> strSplit( std::string, char );
        static void *ft_memset(void *b, int c, size_t len);

        // general utils
        static ListNode *extractBlocks( std::string, int );
        static ListNode *parse( std::string );
        static bool checkValidList( ListNode *, int );
        static bool checkValidContent( std::string );

        // field validation
        static void validateField( std::string, std::vector<std::string> );
        static void validateBodySize( std::vector<std::string> );
        static void validateAllowedMethods( std::vector<std::string> );
        static void validateErrorPage( std::vector<std::string> );
        static void validateListen( std::vector<std::string> );
        static void validateRoot( std::vector<std::string> );
        static void validateAutoIndex( std::vector<std::string> );
        static void validateFileUpload( std::vector<std::string> );

        // check types
        static bool isNumber( std::string );

        class ParsingException: public std::exception {
            private:
                std::string message;

            public:
                ParsingException(std::string);

                virtual const char* what() const throw();

                virtual ~ParsingException() throw();
        };
        
};

#endif