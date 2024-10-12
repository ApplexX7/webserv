/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethode.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 14:22:41 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/12 22:51:11 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

int Response::parseBodyHeaders(std::string header){
    size_t pos = 0;
    size_t secpos = 0;
    pos = header.find("Content-Disposition:");
    if (pos != std::string::npos){
        pos += std::strlen("Content-Disposition:");
        secpos = header.find(";", pos);
        if (secpos != std::string::npos){
            this->bhCtDisposition = header.substr(pos, secpos - pos);
            this->bhCtDisposition.erase(remove(this->bhCtDisposition.begin(), this->bhCtDisposition.end(), ' '), this->bhCtDisposition.end());
        }
    }
    pos = header.find("name=\"");
    if (pos != std::string::npos){
        pos += std::strlen("name=\"");
        secpos = header.find("\"", pos);
        if (secpos != std::string::npos){
            this->bhName = header.substr(pos, secpos - pos);
        }
    }
    pos = header.find("filename=\"");
    if (pos != std::string::npos){
        pos += std::strlen("filename=\"");
        secpos = header.find("\"\r\n");
        if (secpos != std::string::npos){
            this->bhFileName = header.substr(pos, secpos - pos);
        }
    }
    pos = header.find("Content-Type:");
    if (pos != std::string::npos){
        pos += std::strlen("Content-Type:");
        secpos = header.find("\r\n");
        if (secpos != std::string::npos){
            this->bhConetentType = header.substr(pos, secpos - pos);
            this->bhConetentType.erase(remove(this->bhConetentType.begin(), this->bhConetentType.end(), ' '), this->bhConetentType.end());
        }
    }
    std::cout  << "****************************************" << std::endl;
    std::cout << this->bhCtDisposition << std::endl;
    std::cout << this->bhName << std::endl;
    std::cout << this->bhFileName << std::endl;
    std::cout << this->bhConetentType << std::endl;
    std::cout  << "****************************************" << std::endl;
    return (0);
}

int Response::hexaToDecima(std::string hexa){
    int decimal;
    std::stringstream ss;

    ss << std::hex << hexa;
    ss >> decimal;
    return (decimal);
}

int Response::parseChunkedPart(std::string chunk, Client &clientdata){
    size_t pos = 0;
    size_t secpos = 0;
    size_t sizeofchunk;
    std::string buffer;

    (void)clientdata;
    while (true){
        pos = chunk.find("\r\n", secpos + 2);
        if (pos == std::string::npos){
            break;
        }
        sizeofchunk =  this->hexaToDecima(chunk.substr(secpos, pos - secpos));
        if (sizeofchunk == 0){
            break;
        }
        buffer += chunk.substr(pos + 2, sizeofchunk);
        secpos = sizeofchunk + 2;
    }
    return (0);
}


int Response::parseChunckedType(std::string &body, Client &clientdata){
    size_t secpos = 0;
    size_t pos = 0;
    std::string bufferdata;
    std::vector <std::string> chunked;
    if (body.find(clientdata.getRequest().getStartBoundary()) != std::string::npos) {
        while (pos != std::string::npos) {
            secpos = body.find(clientdata.getRequest().getStartBoundary(), pos + clientdata.getRequest().getStartBoundary().length());
            if (secpos != std::string::npos) {
            std::string chunk = body.substr(pos + clientdata.getRequest().getStartBoundary().length(),
                secpos - (pos + clientdata.getRequest().getStartBoundary().length()));
            if (!chunk.empty() && chunk != clientdata.getRequest().getEndofBoundary()) {
                chunked.push_back(chunk);
            }
            pos = secpos;
            } else {
                break;
            }
        }
        // parse the Chunk Part;
        for (size_t i = 0; i < chunked.size(); i++){
            pos = chunked[i].find("\r\n\r\n");
            if (pos == std::string::npos){
                return (0);
            }
            // parse the headers in every chunk;
            this->parseBodyHeaders(chunked[i].substr(2, pos));
            if (this->parseChunkedPart(chunked[i].substr(pos + 4), clientdata))
                return (1);
        }
    }
    else {// if there is only chunked without boundary
        this->parseChunkedPart(body, clientdata);
    }
    return (0);
}

int  Response::postBodyResponse(Client &clientData){
    // if (0){
    //     std::cout <<"hello\n" << std::endl;
    //     // check for the file dir;
    //     return (1);
    // }
    // else{
    //     // use the root for it
    //     return (1);
    // }
    // check if body going to be excute in Cgi
    if (clientData.getRequest().getTheBodyType() == BOUNDARY){
         this->parseChunckedType(clientData.getRequest().getBody(), clientData);
        // if we had boundary
    }
    else if (clientData.getRequest().getTheBodyType() == ENCODING){
        this->parseChunckedType(clientData.getRequest().getBody(), clientData);
        // parsing the body for the chunk size;
    }
    else if (clientData.getRequest().getTheBodyType() ==  FIXEDSIZE){
        // parsing the body for the Fizxed lenght
    }
    return (0);
}