/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 11:14:38 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/11 12:23:15 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPONSE_HPP
#define REPONSE_HPP

#include <ctime>
#include "../Request/Request.hpp"
#include "../utils/Client.hpp"
#include <exception>
#include <sys/stat.h>
#include <dirent.h>
//status Response

//information Response
#define CONTINUE 100
#define SWITCHING_PROTOCOLS 101
#define PROCESSING 102
#define EARLY_HINTS 103

// Successful response
#define SUCCESS 200
#define CREATED 201
#define ACCEPTED 202
#define NO_CONTENT 204

//client error response
#define BAD_REQUEST 400
#define UNAUTHORIZED 401
#define FORBIDDEN 403
#define NOT_FOUND 404
#define METHOD_NOT_ALLOWED 405
#define NOT_ACCEPTABLE 406
#define REQUEST_TIMEOUT 408

// server errors
#define INTERNAL_SERVER_ERROR 500


#define SERVER webserve/1.1
  
typedef enum e_response_status {
    IDLE,
    ONGOING,
    FINISHED
} t_response_status;

class Client;

class Response {
    private:
        int statusCode;
        std::map<int, std::string> statusTexts;
        t_response_status status;
        std::ifstream file;
        std::string StatusLine;
        std::string fileName;
        std::map<std::string, std::string> ResponseMeth;
        std::map<std::string, std::string> mimeTypes;
        std::string body;
        std::string contentType;
        int contentLength;
        Location *location;

        std::string path;

        Client *client;

        bool checkPath( void );
        std::string getFullPath( std::string );

    public:
        Response();
        Response(const Response &Obj);
        Response& operator=(const Response & Obj);
        ~Response();

        //seters and geters
        void setFileName( std::string );
        void extractFileName( void );
        std::string getFileName( void );

        std::string getBody( void ) const;
        void setBody( std::string );
        
        std::string getStatusLine();
        void setStatusLine(std::string _Status);
        
        std::map<std::string, std::string> getMap( void ) const;
        void setMap(std::string _name, std::string _Value);
        
        std::string getMimeType(std::string _Key);
        
        int getStatusCode( void );
        void setStatusCode( int );
        
        t_response_status getStatus( void ) const;
        void setStatus( t_response_status );

        Client* getClient( void );
        void setClient( Client * );

        std::string getPath( void ) const;
        void setPath( std::string );
        
        std::string getContentType( void ) const;
        void setContentType( std::string );

        // helpers
        std::string getStatusText( void );
        std::string getFileChunk( void );

        // response handlers
        std::string constructHeader( void );
        std::string createGetResponse( void );

        // exceptions
        class ResponseException: public std::exception {
            private:
                std::string message;

            public:
                ResponseException(std::string);

                virtual const char* what() const throw();

                virtual ~ResponseException() throw();
        };
};

#endif