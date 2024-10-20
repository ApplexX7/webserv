/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethode.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 14:22:41 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/20 21:08:00 by mohilali         ###   ########.fr       */
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
	if (this->outFile.is_open()){
		return (0);
	}
	if (stat(this->bFullPath.c_str(), &buffer) == 0){
		this->statusCode = 409;
		return (1);
	}
	std::cout <<this->bFullPath << std::endl;
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
	int byteRead = 0;
	std::string line;
	std::stringstream ss(body);
	std::string startBoundary = clientdata.getRequest().getStartBoundary() + "\r\n";
	std::string endBoundary = clientdata.getRequest().getEndofBoundary();
	while (true){
		if (!this->chunkSize){
			if (!std::getline(ss, line) || line.empty())
				break;
			this->chunkSize = this->hexaToDecima(line);
			if (!this->chunkSize)
				break;
		}
		std::string buffer(this->chunkSize, '\0');
		byteRead = ss.read(&buffer[0], this->chunkSize).gcount();
		if (byteRead <= 0){
			break;
		}
		if (byteRead < (int)this->chunkSize){
			this->chunkSize -= byteRead;
			buffer.resize(byteRead);
		}
		else
			this->chunkSize = 0;
		pos = buffer.find(startBoundary);
		if (pos != std::string::npos){
			if (pos != 0){
				this->finaleBody += body.substr(0, pos - 2);
				if (!this->finaleBody.empty()){
					if (this->checkforValidField())
						;
					else {
						this->writeChunkinfile(clientdata);
					}
					this->closeFileafterWriting();
					this->resetBodyHeader();
					this->finaleBody.clear();
				}
			}
			size_t headerEnd = buffer.find("\r\n\r\n", pos);
			this->parseBodyHeaders(buffer.substr(pos + startBoundary.length(), headerEnd - pos - startBoundary.length()));
			buffer = buffer.substr(headerEnd + 4);
		}
		this->finaleBody += buffer;
		pos = this->finaleBody.find(endBoundary);
		if (pos != std::string::npos){
			this->finaleBody = this->finaleBody.substr(0, pos - 2);
			break;
		}
		if (this->chunkSize == 0 && !this->finaleBody.empty()){
			if (this->checkforValidField())
					;
			else {
				this->writeChunkinfile(clientdata);
			}
			this->finaleBody.clear();
		}
		if (ss.eof())
			break;
	}
	if (!this->finaleBody.empty()){
		pos = this->finaleBody.find(endBoundary);
		if (pos != std::string::npos){
			this->finaleBody = this->finaleBody.substr(0, pos - 2);
		}
		if (this->checkforValidField())
			;
		else {
			this->writeChunkinfile(clientdata);
		}
		this->finaleBody.clear();
	}
	return (0);
}


int Response::parseBoundarys(std::string &body, Client &clientData){
	size_t pos = 0;
	size_t secpos = 0;
	bool complete = false;
	std::string startBd = clientData.getRequest().getStartBoundary() + "\r\n";
	std::string endBd = clientData.getRequest().getEndofBoundary();

	while (!complete){
		pos = body.find(startBd, pos);
		if (pos != std::string::npos){
			pos =  startBd.length() + pos + 2;
			if (this->finaleBody.empty()){
				this->closeFileafterWriting();
				this->finaleBody.clear();
			}
			secpos = body.find("\r\n\r\n", pos) + 4;
			this->parseBodyHeaders(body.substr(pos , secpos - pos));
			pos = secpos;
			size_t endbb = body.find(endBd, pos);
			secpos = body.find(startBd, pos);

            if (endbb != std::string::npos && endbb <= secpos) {
                secpos = endbb;
                complete = true;
            }
			if (secpos != std::string::npos){
				this->finaleBody = body.substr(pos, secpos - pos - 2);
				this->writeChunkinfile(clientData);
				this->finaleBody.clear();

			}
			else{
				this->finaleBody = body.substr(pos);
				this->writeChunkinfile(clientData);
				this->finaleBody.clear();
				complete = true;
			}
		}
		else{
			size_t endpos = body.find(endBd);
			if (endpos != std::string::npos){
				this->finaleBody = body.substr(0,endpos - 2);
			}
			else
				this->finaleBody = body;
			writeChunkinfile(clientData);
			this->finaleBody.clear();
			complete = true;
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
	// std::cout << clientData.getRequest().getBody() <<std::endl;
	if (clientData.getRequest().getTheBodyType() == ENCODING){
		std::cout << clientData.getRequest().getBody() << std::endl;
		if (!this->parseChunckedType(clientData.getRequest().getBody(), clientData) &&  clientData.getRequest().getFinishReading()){
			clientData.responseReady = true;
			this->statusCode = 201;
		}
		else
			return (1);
	}
	else if (clientData.getRequest().getTheBodyType() == BOUNDARY){
		if (!this->parseBoundarys(clientData.getRequest().getBody(), clientData) && clientData.getRequest().getFinishReading()){
			clientData.responseReady = true;
		}
	}
	else if (clientData.getRequest().getTheBodyType() ==  FIXEDSIZE){
		if ((size_t)clientData.getRequest().getContentLenght() != clientData.getRequest().getBody().size()){
			clientData.responseReady = true;
			this->statusCode = 400;
			return (1);
		}
		else
			parseContentLenght(clientData, clientData.getMessage());
	}
	else if (clientData.getRequest().getTheBodyType() == NONE){
		clientData.responseReady = true;
		clientData.getRequest().setFinishReading(true);
	}
	return (0);
}