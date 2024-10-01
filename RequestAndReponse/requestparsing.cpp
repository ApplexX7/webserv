/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestparsing.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:17:43 by mohilali          #+#    #+#             */
/*   Updated: 2024/09/30 15:20:27 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"


//request-line parsing

void requestline(std::string str, Request &Methode){
    if (std::count(str.begin(), str.end(), ' ') != 2){
          throw Response::ResponseException("HTTP/1.1 400 Bad Request\n");
    }
    if (str.find("GET") == 0)
        Methode.Setmethode("GET");
    else if (str.find("POST") == 0)
        Methode.Setmethode("POST");
    else if (str.find("DELETE") == 0)
        Methode.Setmethode("DELETE");
    else{
        throw Response::ResponseException("HTTP/1.1 501 Not Implemented\n");
    }
    //check path travel in URI
    int pos;
    pos = str.find_last_of(' ');
    if (str.substr(pos + 1, str.length()) != "HTTP/1.1"){
        throw Response::ResponseException("HTTP/1.1 505 HTTP Version Not Supported\n");
    }
    Methode.SetUri(str.substr(str.find(' ') + 1, pos - str.find(' ') -1 ));
}


/*
    Parse the header request
    and fill the map with the headers value
    betwen a header and another is \r\n
    the begging of the string str is the Host value tell the end of the headers
*/


// need it later
    // if (std::count(str.begin(), str.end(), ' ') != 1){
    //     std::cerr <<  "HTPP/1.1 400 Bad Request" << std::endl;
    //     exit(0);
    // }
    // str = str.erase(0,6);
    // if (str.empty()){
    //     std::cerr <<  "HTPP/1.1 400 Bad Request" << std::endl;
    //     exit(0);
    // }
    // if (str.find_first_of("@#!$%&^*+=_/") != std::string::npos){
    //     std::cerr <<  "HTPP/1.1 400 Bad Request" << std::endl;
    //     exit(0);
    // }
    // if (str[0] == '.' || str[0] == '-' || str[str.length() - 1] == '.' || str[str.length() - 1] == '-'){
    //     std::cerr <<  "HTPP/1.1 400 Bad Request" << std::endl;
    //     exit(0);
    // }
    // str.erase();

void herderparser(std::string str, Request &Methode){
    int pos;
    std::string line;
    std::string name;
    std::string value;
    std::istringstream strheader(str);

    while (std::getline(strheader, line)){
       if (line == "\r")
            break;
        pos = line.find(':');
        if ((size_t)pos == std::string::npos){
              throw Response::ResponseException("HTTP/1.1 505 HTTP Version Not Supported\n");
        }
        name = line.substr(0, pos);
        value = line.substr(pos + 1);
        // isspace should not be in the name
        int f = name.find(' ');
        int fi = name.find('\t');
        if ((size_t)f != std::string::npos || (size_t)fi != std::string::npos){
              throw Response::ResponseException("HTTP/1.1 400 Bad Request\n");
        }
        value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
        if (value.empty() || name.empty()){
            throw Response::ResponseException("HTTP/1.1 400 Bad Request\n");
        }
        Methode.setHeaders(name, value);
    }
    
}

void parsingbodyrequest(std::string str, Request &Methode){
    int pos;

    pos = str.find("\r\n\r\n");
    Methode.setBody(str.substr(0, pos));
}

// main for testing
int main(void) {
    int pos;
    std::string str;
    Request Methode;
    str = "GET /Users/mohilali/.brew/etc/nginx/nginx.conf HTTP/1.1\r\n"
            "Host: localhost:8080\r\n"
            "Connection: keep-alive\r\n"
            "Upgrade-Insecure-Requests: 1\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
            "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_3) AppleWebKit/604.5.6 (KHTML, like Gecko) Version/11.0.3 Safari/604.5.6\r\n"
            "Accept-Language: en-us\r\n"
            "DNT: 1\r\n"
            "Accept-Encoding: gzip, deflate\r\n"
            "\r\n"
            "Usually GET requests don\'t have a body\r\n"
            "But I don\'t care in this case :)";
    pos = str.find("\r\n");
    requestline(str.substr(0, pos), Methode);
    // check the head Host if its valid;
    pos = str.find("Host:");
    if (str.find_first_of("Host:") != std::string::npos){
        herderparser(str.substr(pos, str.length() - pos), Methode);
    }
    // body check;
    pos = str.find("\r\n\r\n");
    parsingbodyrequest(str.substr(pos + 4), Methode);
    std::cout << "REQUEST LINE: " << std::endl;
    std::cout << Methode.getMethode()<< "$" << std::endl;
    std::cout << Methode.getUri() << "$"<< std::endl;
    Methode.printmap();
    std::cout << "Body :" << std::endl;
    std::cout << Methode.getBody() << std::endl;

    if (Methode.getMethode() == "GET")
        GetMethode(Methode);
    return 0;
}
