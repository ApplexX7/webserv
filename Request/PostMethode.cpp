/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethode.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 09:49:44 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/19 18:12:42 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

int Request::ParsePostHeaders(){
	size_t lenght;
	std::string tempStr;
	tempStr = this->getValue("Content-Type");
	if (tempStr.find("boundary=") != std::string::npos){
		this->contentType = tempStr.substr(0, tempStr.find("boundary=") - 1);
		this->startofBoundary = "--" + tempStr.substr(tempStr.find("boundary=") + std::strlen("boundary="));
		this->endofBoundary = this->startofBoundary + "--";
	}
	else
		this->contentType = tempStr;
	std::istringstream ctSize(this->getValue("Content-Length"));
	if (ctSize >> lenght && ctSize.peek() == EOF){
		this->contentLenght = lenght;
	}
	else
		this->contentLenght = 0;
	this->TransferCoding = this->getValue("Transfer-Encoding");
	return (0);
}



//check  the end of teh body and the type of body
int Request::parseBodyTypeBuffer(std::string &bufferedBody){
	if (!this->TransferCoding.empty()){
		if (this->TransferCoding == "chunked"){
			if (bufferedBody.find("0\r\n") != std::string::npos){
				this->bodyType = ENCODING;
				return (1);
			}
			else if (this->bodybuffer.find("0\r\n") != std::string::npos){
				this->bodyType = ENCODING;
				return (1);
			}
		}
	}
	else if (!this->startofBoundary.empty()){
		if (bufferedBody.find(this->endofBoundary) != std::string::npos){
			this->bodyType = BOUNDARY;
			return (1);
		}
		else if (this->bodybuffer.find(this->endofBoundary) != std::string::npos){
			this->bodyType = BOUNDARY;
			return (1);
		}
	}
	else if (this->contentLenght){
		if ((size_t)this->contentLenght <= this->bodybuffer.size()){
			this->bodyType = FIXEDSIZE;
			return (1);
		}
	}
	return (0);
}

