/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethode.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 14:22:41 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/22 22:24:22 by mohilali         ###   ########.fr       */
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


int Response::writeChunkinfile(std::string content, Client &clientdata){
	if (this->openFile(clientdata)){
		return (1);
	}
	this->outFile.write(content.c_str(), content.size());
	if (!this->outFile){
		perror("erro");
		exit(1);
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


int Response::handle_partchunkedformdataWriting(Client &clientData){
	size_t pos = 0;
	int cout = 0;
	std::string headerString;
	std::string startbd = clientData.getRequest().getStartBoundary();
	std::string endbd = clientData.getRequest().getEndofBoundary();

	while (!this->finaleBody.empty()){
		cout++;
		pos = this->finaleBody.find(startbd);
		if (pos != std::string::npos){

			if (pos != 0 && pos < (this->finaleBody.find(endbd))){
				if (!this->checkforValidField()){
					this->writeChunkinfile(this->finaleBody.substr(0, pos - 2), clientData);
				}
				this->finaleBody = this->finaleBody.substr(pos);
				continue ;
			}
			this->finaleBody = this->finaleBody.substr(pos + 2);
			this->resetBodyHeader();
			this->closeFileafterWriting();
			pos = this->finaleBody.find(endbd);
			if (pos != std::string::npos){
				this->finaleBody = this->finaleBody.substr(0, pos - 2);
			}
			this->finaleBody = this->finaleBody.substr(startbd.length());
			pos = this->finaleBody.find("\r\n\r\n");
			if (pos != std::string::npos){
				headerString = this->finaleBody.substr(0, pos);
				this->parseBodyHeaders(headerString);
				this->finaleBody = this->finaleBody.substr(pos + 4);
			}
		}
		if (!this->checkforValidField()){
			this->writeChunkinfile(this->finaleBody,clientData);
			if ((this->finaleBody.find(startbd)) == std::string::npos){
				this->finaleBody.clear();
			}
		}
	}
	this->finaleBody.clear();
	return (0);
}

int Response::hexaToDecima(std::string hexa){
	int decimal;
	std::stringstream ss;

	ss << std::hex << hexa;
	ss >> decimal;
	return (decimal);
}

void	Response::extractSizeChunk(std::string &body){
	size_t pos = 0;
	std::string heaxastring;
	pos = body.find("\r\n");
	if (pos != std::string::npos){
		heaxastring = body.substr(0, pos);
		this->chunkSize = this->hexaToDecima(heaxastring);
		body.erase(0, pos + 2);
	}
}

int Response::parseChunckedType(Client &clientData){
	this->bufferBody += clientData.getMessage();
	if (this->chunkSize == 0){
		extractSizeChunk(this->bufferBody);
	}
	while (!this->bufferBody.empty() && this->chunkSize != 0){
		if (this->bufferBody.length() <= (size_t)this->chunkSize){
			this->finaleBody += this->bufferBody;
			this->chunkSize -= this->bufferBody.length();
			this->bufferBody.clear();
		}
		else{
			this->finaleBody += this->bufferBody.substr(0, this->chunkSize);
			this->bufferBody.erase(0, this->chunkSize);
			this->chunkSize = 0;
		}
	}
	this->handle_partchunkedformdataWriting(clientData);
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
				this->writeChunkinfile(this->finaleBody,clientData);
				this->finaleBody.clear();

			}
			else{
				this->finaleBody = body.substr(pos);
				this->writeChunkinfile(this->finaleBody,clientData);
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
			writeChunkinfile(this->finaleBody,clientData);
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
	this->writeChunkinfile(this->finaleBody ,clientData);
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
		if (!this->parseChunckedType(clientData) &&  clientData.getRequest().getFinishReading()){
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
