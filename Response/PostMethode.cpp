/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethode.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 14:22:41 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/19 18:39:43 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"


std::string Response::generateFileName(){
	char buffer[100];
	time_t rawtime;
	time(&rawtime);

	struct tm*  Info = gmtime(&rawtime);
	std::strftime(buffer, 100, "%a%d%b%Y%H:%M%S", Info);
	return ((std::string) buffer);
}

int Response::closeFileafterWriting(){
	if (this->outFile.is_open()){
		this->outFile.close();
		return (0);
	}
	return (0);
}

int Response::openFile(Client &clientData){
	struct stat buffer;
	Location *PathLocation = clientData.getRequest().getServerLocation();
	if (PathLocation != NULL){
		this->bFullPath = PathLocation->getField("root").getValues()[0] + "/" + this->bhFileName + this->getMimeType(this->bhConetentType);
	}
	else{
		this->bFullPath = clientData.getRequest().getserverNode().getFields()["root"].getValues()[0] + "/" + this->bhFileName + this->getMimeType(this->contentType);
	}
	if (stat(this->bFullPath.c_str(), &buffer) == 0){
		this->statusCode = 409;
		return (1);
	}
	this->outFile.open(this->bFullPath, std::ios::binary);
	if (!this->outFile.good()){
		std::cerr << "Error: Failed to create the file"<< std::endl;
		return (1);
	}
	return (0);
}


int Response::writeChunkinfile(Client &clientdata){
	if (this->openFile(clientdata)){
		return (1);
	}
	this->outFile.write(this->finaleBody.c_str(), this->finaleBody.size());
	if (!this->outFile){
		std::cerr << "Error: failed to write in file"<< std::endl;
		return (1);
	}
	return (0);
}


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

int Response::checkforValidField(){
	if (this->bhFileName.empty()){
		return (1);	
	}
	return (0);
}

void Response::resetBodyHeader( void ){
	this->bFullPath.clear();
	this->bhConetentType.clear();
	this->bhCtDisposition.clear();
	this->bhName.clear();
}

int Response::parseChunckedType(std::string &body, Client &clientdata){
	size_t pos = 0;
	int byteread = 0;
	std::string line;
	std::stringstream ss(body);
	std::string startBoundary = clientdata.getRequest().getStartBoundary();
	std::string endBoundary = clientdata.getRequest().getEndofBoundary();
	size_t sizeofchunk;
	this->bhFileName = this->generateFileName();

	while (getline(ss, line)){
		sizeofchunk = this->hexaToDecima(line);
		if (!sizeofchunk){
			break;
		}
		std::string buffer(sizeofchunk, '\0');
		byteread = ss.read(&buffer[0], sizeofchunk + 1).gcount();
		if (byteread <= 0)
			break;
		if ((pos = buffer.find(startBoundary, pos)) != std::string::npos){
			if (!this->finaleBody.empty()){
				this->finaleBody.resize(this->finaleBody.length() - 2);
				if (this->checkforValidField())
					;
				else {
					this->writeChunkinfile(clientdata);
				}
				this->resetBodyHeader();
				this->finaleBody.clear();
			}
			this->parseBodyHeaders(buffer.substr(startBoundary.length() + 2, buffer.find("\r\n\r\n")));
			
			buffer.erase(pos, buffer.find("\r\n\r\n") + 4);
			if ((pos = buffer.find(endBoundary) != std::string::npos)){
				buffer = buffer.substr(0, buffer.find(endBoundary) - 2);
				this->finaleBody += buffer;
				break;
			}
		}
		this->finaleBody += buffer;
	}
	if (!this->finaleBody.empty()){
		if (this->checkforValidField())
			;
		else {
			this->writeChunkinfile(clientdata);
		}
	}
	return (0);
}


int Response::parseBoundarys(std::string &body, Client &clientData){
	size_t pos = 0;
	size_t secpos = 0;
	bool complite = false;
	std::string startBd = clientData.getRequest().getStartBoundary();
	std::string endBd = clientData.getRequest().getEndofBoundary();

	while (!complite){
		pos = body.find(startBd, pos);
		if (pos != std::string::npos){
			pos =  startBd.length() + pos;
			if (!this->finaleBody.empty()){
				this->closeFileafterWriting();
				this->finaleBody.clear();
			}
			secpos = body.find("\r\n\r\n", pos) + 4;
			this->parseBodyHeaders(body.substr(pos , secpos - pos));
			pos = secpos;
			if ((secpos = (body.find(startBd, pos))) != std::string::npos || (secpos = body.find(endBd, pos)) != std::string::npos){
				if (body.find(endBd, pos) == secpos){
					complite = true;
				}
				this->finaleBody = body.substr(pos, secpos - pos - 2);
				this->writeChunkinfile(clientData);
			}
		}
		else{
			complite = true;
		}
	}
	return (0);
}


int Response::parseContentLenght(Client &clientData, std::string &body){
	size_t pos = 0;
	this->bhFileName = this->generateFileName();
	this->bhConetentType = clientData.getRequest().getValue("Content-Type");
	pos = body.find("\r\n\r\n");
	if (pos != std::string::npos){
		this->finaleBody = body.substr(0, pos);
	}
	else
		this->finaleBody = body;
	this->writeChunkinfile(clientData);
	this->closeFileafterWriting();
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
	if (clientData.getRequest().getTheBodyType() == ENCODING){
		if (this->parseChunckedType(clientData.getRequest().getBody(), clientData))
			this->statusCode = 201;
		else
			return (1);
	}
	else if (clientData.getRequest().getTheBodyType() == BOUNDARY){
		this->parseBoundarys(clientData.getRequest().getBody(), clientData);
	}
	else if (clientData.getRequest().getTheBodyType() ==  FIXEDSIZE){
		if ((size_t)clientData.getRequest().getContentLenght() != clientData.getRequest().getBody().size()){
			this->statusCode = 400;
			return (1);
		}
		else
			parseContentLenght(clientData, clientData.getMessage());
	}
	return (0);
}