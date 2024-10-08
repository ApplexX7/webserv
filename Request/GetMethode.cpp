/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetMethode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:36:01 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/07 12:05:16 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

int Request::findLocation(Client &ClientData) {
    Request& request = ClientData.getRequest();
    ServerNode& ThisServer = request.getServerNode();
    for (std::map<std::string, Location>::iterator it = ThisServer.getLocations().begin();
         it != ThisServer.getLocations().end(); it++) {
        if (request.getUri().find(it->first) == 0) {
            this->LocationName = it->first;
            return 0;
        }
    }
    this->LocationName = "";
    return 0;
}

int Request::CheckDirectory(Client &ClientData){
	if (this->Server->getLocations().size() != 0){
		this->findLocation(ClientData);
		this->PathName = this->Server->getLocations()[this->LocationName].getField("root").getValues()[0] + this->Uri;
	}
	else
		//root if the location don't exist;
	if (access(this->PathName.c_str(), F_OK) != 0){
		ClientData.getResponse().SetStatusCode(404);
		return (0);
	}
	if (access(this->PathName.c_str(), R_OK) != 0){
		ClientData.getResponse().SetStatusCode(403);
		return (0);
	}
	return (0);
}