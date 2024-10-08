/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetMethode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:36:01 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/08 11:10:01 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

int Request::CheckDirectory(Client &ClientData){
	if (this->server->getLocations().size() != 0){
		this->findLocationtobeUsed();
		this->pathName = this->server->getLocations()[this->locationName].getField("root").getValues()[0] + this->Uri;
	}
	else
		//root if the location don't exist;
	if (access(this->pathName.c_str(), F_OK) != 0){
		ClientData.getResponse().SetStatusCode(404);
		return (0);
	}
	if (access(this->pathName.c_str(), R_OK) != 0){
		ClientData.getResponse().SetStatusCode(403);
		return (0);
	}
	return (0);
}