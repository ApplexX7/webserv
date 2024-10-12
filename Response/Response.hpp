/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 11:14:38 by mohilali          #+#    #+#             */
/*   Updated: 2024/10/12 20:41:45 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPONSE_HPP
#define REPONSE_HPP

#include <ctime>
#include "../Request/Request.hpp"
#include <exception>

class Client;

class Response{
	private:
		int statusCode;
		std::string statusLine;
		std::string fileName;
		std::map<int, std::string> statusMaps;
		std::map<std::string, std::string> MIMeType;
		std::string bodyResponse;

		// for Post Body
		std::string finaleBody;
		std::string bhCtDisposition;
		std::string bhConetentType;
		std::string bhName;
		std::string bhFileName;
		std::string ContentType;
	public:
		Response();
		Response(const Response &Obj);
		Response& operator=(const Response & Obj);
		~Response();

		//seters and geters
		void setFileName(std::string);
		void setStatusMaps();
		std::string getFileName( void );
		void setMap(std::string _name, std::string _Value);
		void setStatusLine(std::string _Status);
		std::string getStatusLine();
		std::map<std::string, std::string> getMap( void ) const;
		std::string getMIMeType(std::string _Key);
		void SetStatusCode(int _StatusCode);
		int GetStatusCode( void );
		// Post Response;
		int postBodyResponse(Client &clientData);
		int parseChunckedType(std::string &body, Client & clientdata);
		int parseChunkedPart(std::string chunk, Client &clientdata);
		int parseBodyHeaders(std::string header, Client &clientdata);
		int openFile(Client &clientDatat);
		int writeChunkinfile(Client &clientData);
		int parseBoundarybody(std::string &body);
		int hexaToDecima(std::string hexa);
		// response handlers
		std::string createGetResponse( void );
};

#endif