/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 11:14:38 by mohilali          #+#    #+#             */
/*   Updated: 2024/09/30 14:58:26 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPONSE_HPP
#define REPONSE_HPP

#include <ctime>
#include "Request.hpp"
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

class Response{
    private:
        std::string StatusLine;
        std::map<std::string, std::string> ResponseMeth;
        std::map<std::string, std::string> MIMeType;
    public:
        Response();
        Response(const Response &Obj);
        Response& operator=(const Response & Obj);
        ~Response();

        //seters and geters
        void setMap(std::string _name, std::string _Value);
        void setStatusLine(std::string _Status);
        std::string getStatusLine();
        std::map<std::string, std::string> getMap() const;
        std::string getMIMeType(std::string _Key);

        class ResponseException: public std::exception {
            private:
                std::string error;
            public:
                ResponseException(const std::string& msg);
                virtual const char *what() const throw();
                virtual ~ResponseException() throw();
        };
};

void  GetMethode(Request &Methode);

#endif