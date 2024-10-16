/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 11:14:38 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/16 11:16:32 by wbelfatm         ###   ########.fr       */
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
#include <fstream>


# define CHUNK_SIZE 1024

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
#define PARTIAL_CONTENT 206

// 30x
#define NOT_MODIFIED 304

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
        std::string fileName;
        std::map<std::string, std::string> ResponseMeth;
        std::map<std::string, std::string> mimeTypes;
        std::string body;
        std::string contentType;
        unsigned long contentLength;
        unsigned long bytesSent;
        Location *location;
        unsigned long rangeStart;
        bool isError;

        std::string path;

        Client *client;

        // for Post Body
        std::ofstream outFile;
        std::string bFullPath;
        std::string finaleBody;
        std::string bhCtDisposition;
        std::string bhConetentType;
        std::string bhName;
        std::string bhFileName;
        std::string ContentType;

        bool checkPath( void );
        std::string getFullPath( std::string );

    public:
        Response();
        Response(const Response &Obj);
        Response& operator=(const Response & Obj);
        ~Response();

        //seters and geters
        void setStatusMaps();
        void setFileName( std::string );
        void extractFileName( void );
        std::string getFileName( void );

        std::string getBody( void ) const;
        void setBody( std::string );
        
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
        bool checkAllowedMethod( std::string );
        void reset( void );
        void extractRange( void );

        // response handlers
        std::string constructHeader( void );
        std::string createGetResponse( void );
        std::string getErrorResponse( void );
        Location* getPathLocation( std::string );

        // Post Response;
        int postBodyResponse(Client &clientData);
        int parseBoundaryPart(std::string body, Client & clientDatat);
        int parseChunckedType(std::string &body, Client & clientdata);
        int parseChunkedPart(std::string chunk, Client &clientdata);
        int parseBodyHeaders(std::string header);
        int openFile(Client &clientDatat);
        int checkforValidField();
        int writeChunkinfile(Client &clientData);
        int parseBoundarybody(std::string &body);
        int hexaToDecima(std::string hexa);

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