/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetMethode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:34:38 by mohilali          #+#    #+#             */
/*   Updated: 2024/09/30 15:12:06 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"

//Anny exit is going to change to a response, we just testing with it now

std::string getDateReaponse(){
	char buffer[100];
	time_t rawtime;
	time(&rawtime);

	struct tm*  Info = gmtime(&rawtime);
	std::strftime(buffer, 100, "%a %d %b %Y %H:%M%S GMT", Info);
	return ((std::string) buffer);
}

std::string getMIMEtype(std::string type, Response &Prepa){
	int pos;
	std::string str;

	pos = type.find('.');
	str = type.substr(pos);

	str = Prepa.getMIMeType(str);
	if (str.empty()){
		std::cerr << "Error not supported file type" << std::endl;
		return "";
	}
	return (str);
}


// std::string getBodyContent(){
// }

void FResponse(Response &Prepa, Request &Methode){
	std::map<std::string, std::string>::iterator it;
	std::map<std::string, std::string> ResponseTable;
	std::string responseString;

	ResponseTable = Prepa.getMap();
	responseString += Prepa.getStatusLine();
	responseString += "\r\n";

	for (it = ResponseTable.begin(); it != ResponseTable.end(); it++){
		responseString += it->first + ": " + it->second + "\r\n";
	}
	responseString += "\r\n";

	std::cout << "\n\n\n\n" << "Get Response : \n" << responseString;
}


void generateresponse(Response &Prepa, Request &Methode, struct stat file){
	time_t lastModifaction;
	std::ostringstream oss;
	Prepa.setMap("Date", getDateReaponse());
	Prepa.setMap("Connection", Methode.getValue("Connection"));
	lastModifaction = file.st_mtime;
	Prepa.setMap("Last-Modified", strtok(ctime(&lastModifaction), "\n"));
	Prepa.setMap("Content-Type", getMIMEtype(Methode.getUri(), Prepa));
	Prepa.setMap("Server", "Webserv/1.1");
	oss << file.st_size;
	Prepa.setMap("Content-Lenght", oss.str());
}

// std::string Getresponse(Response Prepa,Request &Methode){
// 	std::string body;
// 	body = getBodyContent(Methode);
// }
void  GetMethode(Request &Methode){
	struct stat file;
	std::string line;
	std::string ResponseBody;
	Response Preparingrequest;

	if (Methode.getUri() == "/"){
		//get the default page
	}
	else{
		if (stat(Methode.getUri().c_str(), &file) != 0){
			throw Response::ResponseException("HTTP/1.1 404 Not found\n");
		}
		if (access(Methode.getUri().c_str(), R_OK) != 0){
			throw Response::ResponseException("HTTP/1.1 403 Forbidden\n");
		}
		Preparingrequest.setStatusLine("HTTP1.1 200 OK");
		generateresponse(Preparingrequest, Methode, file);
		FResponse(Preparingrequest, Methode);
	}
}
