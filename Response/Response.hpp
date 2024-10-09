/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 11:14:38 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/08 13:25:36 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPONSE_HPP
#define REPONSE_HPP

#include <ctime>
#include "../Request/Request.hpp"
#include <exception>

class Response{
    private:
        int statusCode;
        std::string statusLine;
        std::string fileName;
        std::map<int, std::string> statusMaps;
        std::map<std::string, std::string> MIMeType;
        std::string bodyResponse;
    public:
        Response();
        Response(const Response &Obj);
        Response& operator=(const Response & Obj);
        ~Response();

        //seters and geters
        void setFileName(std::string);
        void setStatusMaps();
        std::string getFileName( void );
        void setMap(std::string _name, std::string _Value);
        void setStatusLine(std::string _Status);
        std::string getStatusLine();
        std::map<std::string, std::string> getMap( void ) const;
        std::string getMIMeType(std::string _Key);
        void SetStatusCode(int _StatusCode);
        int GetStatusCode( void );
};

#endif