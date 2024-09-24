/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestparsing.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:17:43 by mohilali          #+#    #+#             */
/*   Updated: 2024/09/24 17:31:06 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"


//request parsing

void requestline(std::string str, Request &Methode){
    if (std::count(str.begin(), str.end(), ' ') != 2){
        std::cerr <<  "HTPP/1.1 400 Bad Request" << std::endl;
        exit(0);
    }
    if (str.find("GET") == 0)
        Methode.Setmethode("GET");
    else if (str.find("POST") == 0)
        Methode.Setmethode("POST");
    else if (str.find("DELETE") == 0)
        Methode.Setmethode("DELETE");
    else{
        std::cerr << "501 Not Implemented\n";
        exit(0);
    }
    //check the Request-URI
    //Valid HTPP
    int pos;
    pos = str.find_last_of(' ');
    if (str.substr(pos + 1, str.length()) != "HTTP/1.1"){
        std::cerr << "505 HTTP version not Supported"<< std::endl;
        exit(0);
    }
    Methode.SetUri(str.substr(str.find(' ') + 1, pos - str.find(' ') -1 ));
}


void parsHeader(std::string str, Request &Methode){
    if (std::count(str.begin(), str.end(), ' ') != 1){
        std::cerr <<  "HTPP/1.1 400 Bad Request" << std::endl;
        exit(0);
    }
    str = str.erase(0,6);
    if (str.empty()){
        std::cerr <<  "HTPP/1.1 400 Bad Request" << std::endl;
        exit(0);
    }
    if (str.find_first_of("@#!$%&^*+=_/") != std::string::npos){
        std::cerr <<  "HTPP/1.1 400 Bad Request" << std::endl;
        exit(0);
    }
    if (str[0] == '.' || str[0] == '-' || str[str.length() - 1] == '.' || str[str.length() - 1] == '-'){
        std::cerr <<  "HTPP/1.1 400 Bad Request" << std::endl;
        exit(0);
    }
    Methode.SetHost(str);
}

// main for testing
int main(void) {
    int pos;
    std::string str;
    Request Methode;
    str = "GET /index.html HTTP/1.1\n"
          "Host: www.example.com\n"
          "Connection: keep-alive\n"
          "sec-ch-ua: \"Chromium\";v=\"116\", \"Not)A;Brand\";v=\"24\", \"Opera GX\";v=\"102\"\n"
          "sec-ch-ua-mobile: ?0\n"
          "sec-ch-ua-platform: \"Windows\"\n"
          "Upgrade-Insecure-Requests: 1\n"
          "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/116.0.0.0 Safari/537.36 OPR/102.0.0.0\n"
          "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\n"
          "Sec-Fetch-Site: none\n"
          "Sec-Fetch-Mode: navigate\n"
          "Sec-Fetch-User: ?1\n"
          "Sec-Fetch-Dest: document\n"
          "Accept-Encoding: gzip, deflate, br\n"
          "Accept-Language: en-US,en;q=0.9";
    pos = str.find_first_of('\n');
    requestline(str.substr(0, pos), Methode);
    // check the head Host if its valid;
    pos = str.find("Host:");
    if (str.find_first_of("Host:") != std::string::npos){
        parsHeader(str.substr(pos, str.find('\n', pos) - pos), Methode);
    }
    std::cout << Methode.getMethode()<< "$" << std::endl;
    std::cout << Methode.getUri() << "$"<< std::endl;
    std::cout << Methode.getHost() << "$"<< std::endl;
    return 0;
}
