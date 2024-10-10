/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethode.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 09:49:44 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/10 17:22:21 by mohilali         ###   ########.fr       */
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
		this->endofBoundary = "--" + tempStr.substr(tempStr.find("boundary=") + std::strlen("boundary=")) + "--";
	}
	else
		this->contentType = tempStr;
	std::istringstream ctSize(this->getValue("Content-Length"));
	if (ctSize >> lenght && ctSize.peek() == EOF){
		this->contentLenght = lenght;
	}
	else
		this->contentLenght = 0;

	this->TransferCoding = getValue("Transfer-Coding");
	return 0;
}

