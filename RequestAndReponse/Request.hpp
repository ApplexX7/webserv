/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:18:27 by mohilali          #+#    #+#             */
/*   Updated: 2024/09/30 12:00:29 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST
#define REQUEST

#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cctype>
#include <string>
#include <stdexcept>
#include <fstream>
#include <map>



class Request{
    private:
        std::string methode;
        std::string Uri;
        std::map<std::string, std::string> headers;
        std::string body;
    public:
        Request();
        Request(const Request &obj);
        Request& operator=(const Request &ope);
        ~Request();
        void setBody(std::string _Body);
        void setHeaders(std::string &name, std::string &value);
        void Setmethode(std::string _metode);
        std::string getValue(std::string _Key);
        std::string getMethode();
        std::string getUri();
        std::string getBody();
        // for debugging
        void printmap();
        void SetUri(std::string _Uri);
};

#endif