/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethode.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 14:22:41 by mohilali          #+#    #+#             */
/*   Updated: 2024/11/12 12:36:05 by mohilali         ###   ########.fr       */
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

static std::string int_to_string(int num){
	std::ostringstream oss;
	oss << num;
	std::string str = oss.str();
	return (str);
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
		this->bFullPath = PathLocation->getField("upload_store").getValues()[0] + "/" + this->bhFileName + this->getMimeType(this->bhConetentType);
	}
	else{
		if (clientData.getRequest().getserverNode().getFields()["upload_store"].getValues().size() > 0)
			this->bFullPath = clientData.getRequest().getserverNode().getFields()["upload_store"].getValues()[0] + "/" + this->bhFileName + this->getMimeType(this->contentType);
		else
			return (1);
	}
	if (this->bFullPath.empty()){
		return (2);
	}
	if (this->outFile.is_open()){
		return (0);
	}
	if (stat(this->bFullPath.c_str(), &buffer) == 0){
		this->statusCode = 409;
		return (3);
	}
	this->outFile.open(this->bFullPath, std::ios::binary);
	if (!this->outFile.good()){
		return (1);
	}
	return (0);
}


int Response::writeChunkinfile(std::string content, Client &clientdata){
	int status = this->openFile(clientdata);
	if (status == 1){
		this->statusCode = 400;
		return (1);
	}
	else if (status == 3)
		return (1);
	this->outFile.write(content.c_str(), content.size());
	if (!this->outFile){
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
	std::string headerString;
	std::string startBd = clientData.getRequest().getStartBoundary();
	std::string endBd = clientData.getRequest().getEndofBoundary();
	while (!this->finaleBody.empty()){
		pos = this->finaleBody.find(startBd);
		if (pos != std::string::npos){
			if (pos != 0 && pos < this->finaleBody.find(endBd)){
				std::string segment = this->finaleBody.substr(0, pos - 2);
				if (!this->checkforValidField()){
					if (this->writeChunkinfile(segment, clientData))
						return (1);
				}
				this->finaleBody = this->finaleBody.substr(pos);
				continue;
			}
			else{
				pos = this->finaleBody.find(endBd);
				if (pos != std::string::npos){
					this->finaleBody = this->finaleBody.substr(0, pos - 2);
					continue;
				}
				pos = this->finaleBody.find("\r\n\r\n");
				if (pos != std::string::npos){
					this->resetBodyHeader();
					this->closeFileafterWriting();
					std::string headerBh = this->finaleBody.substr(0, pos);
					this->parseBodyHeaders(headerBh);
					this->finaleBody = this->finaleBody.substr(pos + 4);
					continue;
				}
				else
					break;
			}
		}
		if (!this->checkforValidField()){
			if (checkforPartienltboundary(startBd, this->finaleBody) > 0 && !this->chunkedNotComplite)
			{
				this->chunkedNotComplite = 1;
				break;
			}
			this->chunkedNotComplite = 0;
			if (this->writeChunkinfile(this->finaleBody,clientData))
				return (1);
			if ((this->finaleBody.find(startBd)) == std::string::npos){
				this->finaleBody.clear();
			}
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
		clientData.getRequest().getClientMaxSizeBody() -= this->chunkSize;
		if (clientData.getRequest().getClientMaxSizeBody() < 0){
			remove(this->bFullPath.c_str());
			this->statusCode = 413;
			return (1);
		}
	}
	while (!this->bufferBody.empty() && this->chunkSize != 0){
		if (this->bufferBody.length() <= (size_t)this->chunkSize){
			this->finaleBody += this->bufferBody;
			this->chunkSize -= this->bufferBody.length();
			this->bufferBody.clear();
		}
		else{
			this->finaleBody += this->bufferBody.substr(0, this->chunkSize);
			this->bufferBody.erase(0, this->chunkSize + 2);
			this->chunkSize = 0;
		}
	}
	if (clientData.getRequest().getIsACgi()){
		this->cgiFile.write(this->finaleBody.c_str(), this->finaleBody.size());
		if (!this->cgiFile){
			this->statusCode = 500;
			return (1);
		}
		return (0);
	}
	if (this->handle_partchunkedformdataWriting(clientData))
		return (1);
	return (0);
}


int checkforPartienltboundary(const std::string &boundary, const std::string &body) {
    size_t max_len = std::min(boundary.length(), body.length());

    for (size_t i = 1; i <= max_len; ++i) {
        if (boundary.substr(0, i) == body.substr(body.length() - i)) {
            return i;
        }
    }
    return 0;
}

int checkforPartienltboundary(std::string &partielData, std::string &body){
	size_t pos = 0;
	std::string findstring = partielData.substr(0, 3);
	pos = body.find(findstring);
	if (pos != std::string::npos)
		return (1);
	return (0);
}

int Response::parseBoundarys(std::string &body, Client &clientData){
	clientData.getRequest().getClientMaxSizeBody() -= body.length();
	if (clientData.getRequest().getClientMaxSizeBody() < 0){
		remove(this->bFullPath.c_str());
		this->statusCode = 413;
		return (1);
	}
	this->finaleBody += body;
	if (this->handle_partchunkedformdataWriting(clientData))
		return (1);
	return (0);
}

int Response::parseContentLenght(Client &clientData, std::string &body){
	size_t pos = 0;
	this->bhConetentType = clientData.getRequest().getValue("Content-Type");
	this->bhFileName = "/tmp/." + this->generateFileName() + int_to_string(clientData.getFd());
	pos = body.find("\r\n\r\n");
	if (pos != std::string::npos){
		this->finaleBody = body.substr(0, pos);
	}
	else
		this->finaleBody = body;
	std::ofstream outputfile(this->bhFileName);
	if (!outputfile.is_open()){
		return (1);
	}
	outputfile.write(this->finaleBody.c_str(), this->finaleBody.length());
	if (!outputfile){
		outputfile.close();
		return (1);
	}
	outputfile.close();
	return (0);
}


int Response::handleCgiPost(Client &clientData){
	std::srand(time(NULL));
	if (this->cgInputfile.empty()){
		this->cgInputfile = "/tmp/." + this->generateFileName() + std::to_string(std::rand()) + int_to_string(clientData.getFd());
		this->cgiFile.open(this->cgInputfile, std::ios::binary);
		if (!this->cgiFile.is_open()){
			return (1);
		}
	}
	if (clientData.getRequest().getTheBodyType() == ENCODING){
		if (this->parseChunckedType(clientData)){
			return (1);
		}
		return (0);
	}
	if (clientData.getMessage().empty()){
		this->cgiFile.write("\n", 1);
	}
	else{
		clientData.getRequest().getClientMaxSizeBody() -= clientData.getMessage().length();
		if (clientData.getRequest().getClientMaxSizeBody() < 0){
			remove(this->bFullPath.c_str());
			this->statusCode = 413;
			return (1);
		}
		this->cgiFile.write(clientData.getMessage().c_str(), clientData.getMessage().length());
		this->cgiFile.write("\n", 1);
		clientData.getMessage().clear();
	}
	if (this->cgiFile.fail()){
		this->statusCode = 500;
		return (1);
	}
	return (0);
}

int  Response::postBodyResponse(Client &clientData){
	if (clientData.getRequest().getIsACgi()){
		if (this->handleCgiPost(clientData)){
			clientData.responseReady = true;
			return (1);
		}
		return (0);
	}
	if (clientData.getRequest().getTheBodyType() == ENCODING){
		if (!this->parseChunckedType(clientData)){
			if (clientData.getRequest().getFinishReading()){
				clientData.responseReady = true;
				this->statusCode = 201;
			}
		}
		else{
			clientData.responseReady = true;
			return (1);
		}
	}
	else if (clientData.getRequest().getTheBodyType() == BOUNDARY){
		if (!this->parseBoundarys(clientData.getRequest().getBody(), clientData)){
			if (clientData.getRequest().getFinishReading()){
				this->statusCode = 201;
				clientData.responseReady = true;
			}
		}
		else{
			clientData.responseReady = true;
			return (1);
		}
	}
	else if (clientData.getRequest().getTheBodyType() ==  FIXEDSIZE){
		if ((long int)clientData.getRequest().getContentLenght() > clientData.getRequest().getClientMaxSizeBody()){
			clientData.responseReady = true;
			this->statusCode = 413;
			return (1);
		}
		if ((size_t)clientData.getRequest().getContentLenght() != clientData.getRequest().getBody().size()){
			clientData.responseReady = true;
			this->statusCode = 400;
			return (1);
		}
		else{
			if (clientData.getRequest().getContentLenght() == 0){
				clientData.responseReady = true;
				this->statusCode = 201;
			}
			if (!this->parseContentLenght(clientData, clientData.getMessage())){
				clientData.responseReady = true;
				this->statusCode = 201;
			}
			else{
				clientData.responseReady = true;
				this->statusCode = 500;
				return (1);
			}
		}
	}
	else if (clientData.getRequest().getTheBodyType() == NONE){
		this->statusCode = 406;
		clientData.responseReady = true;
		clientData.getRequest().setFinishReading(true);
	}
	return (0);
}
