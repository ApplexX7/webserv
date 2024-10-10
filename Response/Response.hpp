/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 11:14:38 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/10 10:26:19 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPONSE_HPP
#define REPONSE_HPP

#include <ctime>
#include "../Request/Request.hpp"
#include "../utils/Client.hpp"
#include <exception>
//status Response

//information Response
#define CONTINUE 100
#define SWITCHING_PRO 101
#define PROCESSING 102
#define EARLY_HINTS 103

// Successful response
#define SUCCREQUEST 200
#define CREATED 201
#define ACCEPTED 202
#define NOCONTENT 204

//client error response
#define BADREQUEST 400
#define UNAUTHORIZED 401
#define FORBIDDEN 403
#define NOTFOUND 404
#define NOTALLOWEDMETHOD 405
#define NOTACCEPTABLE 406
#define NOTINPLEMETED 500


#define SERVER webserve/1.1
  



class Client;

class Response {
    private:
        int StatusCode;
        std::string StatusLine;
        std::string FileName;
        std::map<std::string, std::string> ResponseMeth;
        std::map<std::string, std::string> MIMeType;
        std::string bodyResponse;
        Client *client;

        bool checkPath( std::string ) const;
        
    public:
        Response();
        Response(const Response &Obj);
        Response& operator=(const Response & Obj);
        ~Response();

        //seters and geters
        void setFileName(std::string);
        std::string getFileName( void );
        void setMap(std::string _name, std::string _Value);
        void setStatusLine(std::string _Status);
        std::string getStatusLine();
        std::map<std::string, std::string> getMap( void ) const;
        std::string getMIMeType(std::string _Key);
        void SetStatusCode(int _StatusCode);
        int GetStatusCode( void );

        Client* getClient( void );
        void setClient( Client * );

        // response handlers
        std::string createGetResponse( void );
};

#endif