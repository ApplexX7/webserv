/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 11:19:17 by mohilali          #+#    #+#             */
/*   Updated: 2024/11/09 17:15:29 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "../parsing/Parser.hpp"

void initializeStatusTexts(std::map<int, std::string> &statusTexts)
{
	// 1xx
	statusTexts[CONTINUE] = "Continue";
	statusTexts[SWITCHING_PROTOCOLS] = "Switching Protocols";

	// 2xx
	statusTexts[SUCCESS] = "OK";
	statusTexts[CREATED] = "Created";
	statusTexts[ACCEPTED] = "Accepted";
	statusTexts[NO_CONTENT] = "No Content";
	statusTexts[PARTIAL_CONTENT] = "Partial Content";

	// 30x
	statusTexts[NOT_MODIFIED] = "Not Modified";

	// 4xx
	statusTexts[BAD_REQUEST] = "Bad Request";
	statusTexts[UNAUTHORIZED] = "Unauthorized";
	statusTexts[FORBIDDEN] = "Forbidden";
	statusTexts[NOT_FOUND] = "Not Found";
	statusTexts[METHOD_NOT_ALLOWED] = "Method Not Allowed";
	statusTexts[NOT_ACCEPTABLE] = "Not Acceptable";
	statusTexts[REQUEST_TIMEOUT] = "Request Timeout";
	statusTexts[PAYLOAD_TOO_LARGE] = "Payload too Large";
	statusTexts[URI_TOO_LONG] = "URI Too Long";
	statusTexts[UNSUPPORTED_MEDIA_TYPE] = "Unsupported Media Type";
	statusTexts[RANGE_NOT_SATISFIABLE] = "Range Not Satisfiable";
	statusTexts[UNPROCESSABLE_ENTITY] = "Unprocessable Entity";

	// 5xx
	statusTexts[INTERNAL_SERVER_ERROR] = "Internal Server Error";
	statusTexts[NOT_IMPLEMENTED] = "Not Implemented";
	statusTexts[HTTP_VERSION_NOT_SUPPORTED] = "HTTP Version Not Supported";
}

Response::Response()
{
	// initialize mime types
	this->contentType = "text/plain; charset=UTF-8";
	this->mimeTypes[".html"] = "text/html; charset=UTF-8";
	this->mimeTypes[".css"] = "text/css";
	this->mimeTypes[".js"] = "application/javascript";
	this->mimeTypes[".jpg"] = "image/jpeg";
	this->mimeTypes[".jpeg"] = "image/jpeg";
	this->mimeTypes[".png"] = "image/png";
	this->mimeTypes[".gif"] = "image/gif";
	this->mimeTypes[".json"] = "application/json";
	this->mimeTypes[".xml"] = "application/xml";
	this->mimeTypes[".txt"] = "text/plain";
	this->mimeTypes[".mp4"] = "video/mp4";
	this->mimeTypes[".pdf"] = "application/pdf";
	this->mimeTypes[".svg"] = "image/svg+xml";
	this->mimeTypes[".woff"] = "font/woff";
	this->mimeTypes[".woff2"] = "font/woff2";
	this->mimeTypes[".ttf"] = "font/ttf";
	this->mimeTypes[".eot"] = "application/vnd.ms-fontobject";
	this->mimeTypes[".otf"] = "font/otf";
	this->mimeTypes[".webp"] = "image/webp";
	this->mimeTypes[".bmp"] = "image/bmp";
	this->mimeTypes[".tiff"] = "image/tiff";
	this->mimeTypes[".ico"] = "image/x-icon";
	this->mimeTypes[".webm"] = "video/webm";
	this->mimeTypes[".ogg"] = "audio/ogg";
	this->mimeTypes[".mp3"] = "audio/mpeg";
	this->mimeTypes[".wav"] = "audio/wav";
	this->mimeTypes[".flac"] = "audio/flac";
	this->mimeTypes[".zip"] = "application/zip";
	this->mimeTypes[".tar"] = "application/x-tar";
	this->mimeTypes[".gz"] = "application/gzip";
	this->mimeTypes[".7z"] = "application/x-7z-compressed";
	this->mimeTypes[".rar"] = "application/x-rar-compressed";
	this->mimeTypes[".csv"] = "text/csv";
	this->mimeTypes[".md"] = "text/markdown";
	this->mimeTypes[".psd"] = "image/vnd.adobe.photoshop";
	this->mimeTypes[".epub"] = "application/epub+zip";
	this->mimeTypes[".apk"] = "application/vnd.android.package-archive";
	this->mimeTypes[".jsonld"] = "application/ld+json";
	this->mimeTypes[".xls"] = "application/vnd.ms-excel";
	this->mimeTypes[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	this->mimeTypes[".ppt"] = "application/vnd.ms-powerpoint";
	this->mimeTypes[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	this->mimeTypes[".doc"] = "application/msword";
	this->mimeTypes[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	this->mimeTypes[".json5"] = "application/json5";
	this->mimeTypes[".mkv"] = "video/x-matroska";
	this->mimeTypes[".flv"] = "video/x-flv";
	this->mimeTypes[".exe"] = "application/vnd.microsoft.portable-executable";
	this->mimeTypes[".iso"] = "application/x-iso9660-image";
	this->mimeTypes[".dmg"] = "application/x-apple-diskimage";
	this->mimeTypes[".bin"] = "application/octet-stream";
	this->mimeTypes[".mid"] = "audio/midi";
	this->mimeTypes[".midi"] = "audio/midi";
	this->mimeTypes[".3gp"] = "video/3gpp";
	this->mimeTypes[".3g2"] = "video/3gpp2";
	this->mimeTypes[".swf"] = "application/x-shockwave-flash";
	this->mimeTypes[".vtt"] = "text/vtt";
	this->mimeTypes[".ts"] = "video/mp2t";
	this->mimeTypes[".ics"] = "text/calendar";
	this->mimeTypes[".vcf"] = "text/vcard";

	// initialize encoding symbols
	this->encodingSymbols["%20"] = ' ';
	this->encodingSymbols["%21"] = '!';
	this->encodingSymbols["%22"] = '"';
	this->encodingSymbols["%23"] = '#';
	this->encodingSymbols["%24"] = '$';
	this->encodingSymbols["%25"] = '%';
	this->encodingSymbols["%26"] = '&';
	this->encodingSymbols["%27"] = '\'';
	this->encodingSymbols["%28"] = '(';
	this->encodingSymbols["%29"] = ')';
	this->encodingSymbols["%2A"] = '*';
	this->encodingSymbols["%2B"] = '+';
	this->encodingSymbols["%2C"] = ',';
	this->encodingSymbols["%2D"] = '-';
	this->encodingSymbols["%2E"] = '.';
	this->encodingSymbols["%2F"] = '/';
	this->encodingSymbols["%3A"] = ':';
	this->encodingSymbols["%3B"] = ';';
	this->encodingSymbols["%3C"] = '<';
	this->encodingSymbols["%3D"] = '=';
	this->encodingSymbols["%3E"] = '>';
	this->encodingSymbols["%3F"] = '?';
	this->encodingSymbols["%40"] = '@';
	this->encodingSymbols["%5B"] = '[';
	this->encodingSymbols["%5C"] = '\\';
	this->encodingSymbols["%5D"] = ']';
	this->encodingSymbols["%5E"] = '^';
	this->encodingSymbols["%5F"] = '_';
	this->encodingSymbols["%60"] = '`';
	this->encodingSymbols["%7B"] = '{';
	this->encodingSymbols["%7C"] = '|';
	this->encodingSymbols["%7D"] = '}';
	this->encodingSymbols["%7E"] = '~';

	// initialize defaults
	this->statusCode = SUCCESS;
	this->status = IDLE;
	initializeStatusTexts(this->statusTexts);
	this->location = NULL;
	this->body = "";
	this->contentLength = 0;
	this->bytesSent = 0;
	this->rangeStart = 0;
	this->isError = false;
	this->isBody = false;
	this->isRedir = false;
	this->redirUrl = "";
	this->chunkSize = 0;
	this->globalRead = 0;
	this->bufferBody = "";
	this->chunkedNotComplite = 0;
}

int Response::getStatusCode()
{
	return (this->statusCode);
}

void Response::setStatusCode(int statusCode)
{
	this->statusCode = statusCode;
}

void Response::setMap(std::string _name, std::string _Value)
{
	this->ResponseMeth[_name] = _Value;
}

Response &Response::operator=(const Response &Obj)
{
	if (this != &Obj)
	{
		this->ResponseMeth = Obj.ResponseMeth;
		this->mimeTypes = Obj.mimeTypes;
	}
	return (*this);
}

std::map<std::string, std::string> Response::getMap() const
{
	return (this->ResponseMeth);
}

Client *Response::getClient(void)
{
	return this->client;
}

void Response::setClient(Client *client)
{
	this->client = client;
}

void trimSlashes(std::string &path)
{
	int i = path.length() - 1;

	if (i <= 0)
		return;
	while (i >= 0 && path[i] == '/')
		i--;
	path = path.substr(0, i + 1);
}

std::string Response::getFullPath(std::string path)
{

	ServerNode &server = this->client->getParentServer();
	Location *location = NULL;
	std::string root;
	std::string fullPath;
	std::string index = "";
	std::vector<std::string> redirection;
	struct stat fileStat;
	bool autoIndex = true;

	location = this->getPathLocation(path);

	if (!location)
	{
		root = server.getField("root").getValues()[0];
		redirection = server.getField("return").getValues();
	}
	else
	{
		this->location = location;
		root = location->getField("root").getValues()[0];
		redirection = location->getField("return").getValues();
	}

	// trimSlashes(root);
	// trimSlashes(path);

	if (redirection.size())
	{
		this->isRedir = true;
		this->redirUrl = redirection[1];
		this->statusCode = std::atoi(redirection[0].data());
		return "";
	}

	if (location)
		this->checkPath();
	this->setPath(root);
	this->checkPath();

	fullPath = root + path;

	std::cout << "FULL PATH: " << fullPath << std::endl;

	this->setPath(fullPath);
	this->checkPath();

	stat(fullPath.data(), &fileStat);

	/*
		check file type (dir/regular)
			if dir
				check autoindex, if so, append index.html to path
				otherwise return dirs listing
			if file, nothing to be done
	*/

	if (S_ISDIR(fileStat.st_mode))
	{

		if (path[path.length() - 1] != '/')
			fullPath += "/";

		// is dir
		if (location)
		{
			// path matches a location
			autoIndex = location->getField("autoindex").getValues()[0] == "on";
			index = location->getField("index").getValues()[0];
		}

		else
		{
			autoIndex = server.getField("autoindex").getValues()[0] == "on";
			index = server.getField("index").getValues()[0];
		}

		if (index.length() == 0)
			index = "index.html";
		fullPath += index;
		try
		{
			this->setPath(fullPath);
			this->checkPath();
		}
		catch (Response::ResponseException e)
		{

			// index doesn't exist or not enough permissions
			if (autoIndex)
			{
				fullPath = root + path;
				this->setPath(fullPath);
			}
			else
			{
				this->setStatusCode(NOT_FOUND);
				throw Response::ResponseException("Not found");
			}
		}
	}

	if (this->client->getRequest().getIsACgi())
	{
		fullPath = this->client->getRequest().handleCgi->getCgiFileName();
		this->path = fullPath;
		std::cout << fullPath << std::endl;
	}

	stat(fullPath.data(), &fileStat);
	if (!S_ISDIR(fileStat.st_mode))
	{
		this->contentLength = fileStat.st_size;
		this->extractFileName();
	}
	else
		this->fileName = "";
	return fullPath;
}

bool Response::checkPath(void)
{
	if (access(this->path.data(), F_OK) != 0)
	{
		// doesn't exist
		this->statusCode = NOT_FOUND;
		throw ResponseException("Page Not Found");
	}

	if (access(this->path.data(), R_OK) != 0)
	{
		this->statusCode = FORBIDDEN;
		throw ResponseException("Permission Denied");
	}

	return true;
}

std::string Response::getPath(void) const
{
	return this->path;
}

void Response::setPath(std::string path)
{
	this->path = path;
}

t_response_status Response::getStatus(void) const
{
	return this->status;
}

void Response::setStatus(t_response_status status)
{
	this->status = status;
}

std::string Response::getBody(void) const
{
	return this->body;
}

void Response::setBody(std::string body)
{
	this->body = body;
}

std::string Response::getContentType(void) const
{
	return this->contentType;
}

void Response::setContentType(std::string contentType)
{
	this->contentType = contentType;
}

std::string Response::getStatusText(void)
{
	return this->statusTexts[this->statusCode];
}

void Response::extractFileName(void)
{
	std::string path = this->path;
	std::string extension;
	int start = path.length() - 1;

	for (int i = start; i >= 0; i--)
	{
		if (path[i] == '/')
		{
			start = i + 1;
			break;
		}
	}
	this->fileName = path.substr(start, path.length());
	start = this->fileName.length() - 1;
	for (int i = start; i >= 0; i--)
	{
		if (this->fileName[i] == '.')
		{
			start = i;
			break ;
		}
	}
	extension = this->fileName.substr(start, this->fileName.length());
	if (!this->client->getRequest().getIsACgi())
	{
		if (this->mimeTypes.find(extension) != this->mimeTypes.end())
			this->contentType = this->mimeTypes[extension];
		else
			this->contentType = "application/octet-stream";
	}
	else
	{
		// todo: read from cgi fd
		
	}
}

std::string Response::getDirectoryLinks(std::string path, std::string uri)
{
	DIR *dir = opendir(path.data());
	struct dirent *entry;
	std::string tmp;

	std::string head = "\
			<head> \
				<style> \
					*, *::before, *::after {\
						box-sizing: border-box; \
						font-family: helvetica, arial, non-sans; \
					}\
					\
					body { \
						padding: 0; \
						margin: 0; \
						background-color: black; \
						color: white; \
					} \
					a, a:visited, a:active { \
						color: white; \
						text-decoration: none; \
						width: 100%; \
						cursor: pointer; \
						display: block \
					} \
					\
					div { \
						width: 100%; \
						min-width: 100vw; \
						height: 100%; \
						min-height: 100hw; \
						padding: 16px 32px; \
					} \
					\
					ul { \
						list-style: none; \
						padding-left: 0; \
					} \
					\
					li { \
						padding: 8px; \
						width: 100%; \
						border-radius: 4px; \
						transition: all 0.2s; \
						border-bottom: 1px gray solid; \
					} \
					\
					li:hover { \
						background-color: gray; \
					} \
				</style> \
			</head> \
	";

	std::string res = " \
	<div>\
	<h1> Index of " + uri +
					  "</h1>\
	<ul>\
	";
	std::string closing = "\
	</ul>\
	</div>\
	";

	std::string name;

	if (!dir)
		return "";
	entry = readdir(dir);
	while (entry)
	{
		tmp = entry->d_name;
		if (tmp == ".." || tmp[0] != '.')
		{
			name = tmp;
			this->encodeUri(tmp);
			if (entry->d_type != DT_REG)
			{
				tmp += "/";
				name += "/";
				res += "<li><a href=\"" + tmp + "\" style=\"color: deepskyblue; font-weight: bold; \">";
			}
			else
				res += "<li><a href=\"" + tmp + "\">";
			res += name;
			res += "</a></li>";
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return head + res + closing;
}

std::string getDateResponse()
{
	char buffer[100];
	time_t rawtime;
	time(&rawtime);

	struct tm *Info = gmtime(&rawtime);
	std::strftime(buffer, 100, "%a %d %b %Y %H:%M:%S GMT", Info);
	return ((std::string)buffer);
}

std::string Response::constructHeader(void)
{
	std::string header = "HTTP/1.1 ";

	header += std::to_string(this->statusCode) + " " + this->getStatusText() + "\r\n";

	header += "Content-Type: " + this->contentType + "\r\n";
	header += "Connection: keep-alive\r\n";
	if (this->isRedir)
	{
		header += "Location: " + this->redirUrl + "\r\n";
	}
	if (this->statusCode == PARTIAL_CONTENT)
	{
		unsigned long totalSize = this->rangeStart + this->contentLength;
		header += "Content-Range: bytes " + std::to_string(this->rangeStart) + "-" + std::to_string(totalSize - 1) + "/" + std::to_string(totalSize) + "\r\n";
	}
	header += "Content-Length: " + std::to_string(this->contentLength) + "\r\n";
	header += "Accept-Ranges: bytes\r\n";
	header += "Date: " + getDateResponse() + "\r\n";
	header += "Server: webserv/1.1\r\n";
	header += "\r\n";

	if (this->contentLength == 0)
	{
		this->status = FINISHED;
	}

	return header;
}

std::string Response::getFileChunk(void)
{
	std::vector<char> buff(CHUNK_SIZE + 1);

	if (!this->file.is_open())
	{
		this->file.open(this->path, std::ios::binary);
		if (!this->file.is_open())
		{
			this->status = FINISHED;
			throw ResponseException("Couldn't open file");
		}
		this->file.seekg(this->rangeStart, std::ios::beg);
	}

	file.read(buff.data(), CHUNK_SIZE);

	std::streamsize bytesRead = this->file.gcount();
	if (bytesRead < 0)
	{
		this->status = FINISHED;
		throw ResponseException("Couldn't read from file");
	}

	std::string chunk(buff.data(), bytesRead);

	this->bytesSent += bytesRead;

	if (this->bytesSent >= this->contentLength)
		this->status = FINISHED;
	return chunk;
}

bool Response::checkAllowedMethod(std::string path)
{
	ServerNode &server = this->client->getParentServer();
	std::map<std::string, Location> locations = server.getLocations();
	Location *location = NULL;
	std::string method = this->client->getRequest().getmethode();
	std::vector<std::string> allowedMethods;

	location = this->getPathLocation(path);

	if (!location)
		return true;

	allowedMethods = location->getField("limit_except").getValues();

	if (allowedMethods.size() == 0)
		return true;
	if (std::find(allowedMethods.begin(), allowedMethods.end(), method) == allowedMethods.end())
	{
		this->statusCode = METHOD_NOT_ALLOWED;
		throw ResponseException("Method not allowed");
	}
	return true;
}

void Response::reset(void)
{
	this->contentLength = 0;
	this->bytesSent = 0;
	this->rangeStart = 0;
	this->contentType = "text/plain; charset=UTF-8";
	this->fileName = "";
	this->statusCode = SUCCESS;
	this->status = IDLE;
	this->file.close();
	this->path = "";
	this->isError = false;
	this->isBody = false;
	this->isRedir = false;
	this->redirUrl = "";
}

void Response::extractRange(void)
{
	std::string rangeHeader = this->client->getRequest().getValue("Range");
	std::string range;
	int end = rangeHeader.length() - 1;
	int start = end;

	if (rangeHeader.length() == 0)
		return;

	for (int i = 0; i < (int)rangeHeader.length(); i++)
	{
		if (std::isdigit(rangeHeader[i]))
		{
			start = i;
			break;
		}
	}

	if (start != end)
	{
		range = rangeHeader.substr(start, end - 1);

		range.erase(range.length() - 1);

		this->rangeStart = std::atoll(range.data());
	}
	this->statusCode = PARTIAL_CONTENT;
	if (this->contentLength < this->rangeStart)
	{
		// todo: to check
		exit(0);
	}
	this->contentLength -= this->rangeStart;
}

std::string Response::constructErrorBody(void)
{
	std::string html = "<div style=\"text-align: center; \" >\
		<h1>" + std::to_string(this->statusCode) +
					   " " + this->getStatusText() + "</h1>\
		<hr /> \
		<bold>webserv 1.1</bold> \
		</div>";
	this->contentType = this->mimeTypes[".html"];
	return html;
}

void Response::decodeUri(std::string &path)
{
	std::map<std::string, std::string>::iterator it;
	size_t pos = 0;

	for (it = this->encodingSymbols.begin(); it != this->encodingSymbols.end(); it++)
	{
		while ((pos = path.find(it->first, pos)) != path.npos)
		{
			path.replace(pos, 3, it->second);
			pos += 1;
		}
		pos = 0;
	}
}

void Response::encodeUri(std::string &path)
{
	std::map<std::string, std::string>::iterator it;
	size_t pos = 0;

	while ((pos = path.find("%", pos)) != path.npos)
	{
		path.replace(pos, 1, "%25");
		pos += 3;
	}
	pos = 0;

	for (it = this->encodingSymbols.begin(); it != this->encodingSymbols.end(); it++)
	{
		if (it->second != "%")
		{
			while ((pos = path.find(it->second, pos)) != path.npos)
			{
				path.replace(pos, it->second.length(), it->first);
				pos += it->first.length();
			}
			pos = 0;
		}
	}
}

std::string Response::createGetResponse(void)
{
	std::string path = this->client->getRequest().getUri();
	std::string chunk;

	this->decodeUri(path);

	try
	{
		/*
			only check full path if res on IDLE
			if not on IDLE, read file and send
		*/

		if (this->status == IDLE)
		{
			this->getFullPath(path);

			if (this->isRedir)
			{
				this->status = FINISHED;
				return constructHeader();
			}

			// empty filename means it's a dir
			if (this->fileName == "")
			{
				this->body = this->getDirectoryLinks(this->path, path);
				this->contentType = this->mimeTypes[".html"];
				this->contentLength = body.length();
				this->statusCode = SUCCESS;
				this->status = ONGOING;
				this->isBody = true;
				return constructHeader();
			}
			else
			{
				// this is a file, set status to ONGOING to start sending chunks
				this->status = ONGOING;
				this->body = "";

				// extract range from header
				this->extractRange();
			}
		}
		else
		{
			// read from body or file
			if (this->isBody)
			{
				chunk = this->body.substr(0, CHUNK_SIZE);
				if (this->body.length() >= CHUNK_SIZE)
					this->body = this->body.substr(CHUNK_SIZE, this->body.length());
				else
					this->status = FINISHED;
			}
			else
				chunk = this->getFileChunk();

			return chunk;
		}
	}
	catch (ResponseException e)
	{
		if (this->statusCode < 400)
			this->statusCode = INTERNAL_SERVER_ERROR;
			
		// std::cout << "Something went wrong with response: " << e.what() << " PATH : " << this->path << std::endl;
		
		if (!this->isError)
		{
			this->status = IDLE;
			return this->getErrorResponse();
		}
		else
		{
			this->status = FINISHED;
			this->body = this->constructErrorBody();
			this->contentLength = this->body.length();
			return this->constructHeader() + this->body;
		}
	}
	return this->constructHeader();
}

Location *Response::getPathLocation(std::string path)
{
	ServerNode &server = this->client->getParentServer();
	std::map<std::string, Location> &locations = server.getLocations();
	std::map<std::string, Location>::iterator it;
	Location *location = NULL;
	std::string lastMatch = "";
	std::string root;
	std::string rest;
	size_t found;

	for (it = locations.begin(); it != locations.end(); it++)
	{
		found = path.find(it->first);
		if (found != path.npos && found == 0)
		{
			rest = path.substr(found + it->first.length());
			// location match
			if (it->first.length() > lastMatch.length() && (rest.length() == 0 || rest[0] == '/' || it->first[it->first.length() - 1] == '/'))
			{
				location = &it->second;
				lastMatch = it->first;
			}
		}
	}
	if (location)
	{
		// check location permission
		root = location->getField("root").getValues()[0];
		this->setPath(root + "/" + lastMatch);
	}
	return location;
}

std::string Response::getCginputFile(void)
{
	return (this->cgInputfile);
}

std::string Response::getErrorResponse(void)
{
	/*
		steps:
			- look for error directive value
			- look for corresponding location
			- if no location found, use my default
	*/

	std::vector<std::string> errorField;
	std::vector<std::string> errorPages;
	std::string path = "";
	bool found = false;

	if (this->location)
		errorPages = this->location->getField("error_page").getValues();
	errorField = this->client->getParentServer().getField("error_page").getValues();
	errorPages.insert(errorPages.end(), errorField.begin(), errorField.end());

	for (int i = 0; i < (int)errorPages.size(); i++)
	{
		if (Parser::isNumber(errorPages[i]))
		{
			if (std::atoi(errorPages[i].data()) == this->statusCode)
				found = true;
		}
		else if (found)
		{
			path = errorPages[i];
			break ;
		}
	}
	if (path != "")
		this->client->getRequest().SetUri(path);
	this->isError = true;

	this->contentType = this->mimeTypes[".html"];

	if (this->client->getRequest().getmethode() == "GET" || path != "")
		return this->createGetResponse();

	this->status = FINISHED;
	this->body = this->constructErrorBody();
	this->contentLength = this->body.length();
	return this->constructHeader() + this->body;
}

std::string Response::generateResponse(void)
{
	if (this->statusCode >= 400)
		return this->getErrorResponse();

	if (this->client->getRequest().getmethode() == "GET")
		return this->createGetResponse();

	this->status = FINISHED;
	return this->constructHeader();
}

Response::~Response() {}

const char *Response::ResponseException::what() const throw()
{
	return message.c_str();
};

Response::ResponseException::ResponseException(std::string msg) : message(msg) {};

Response::ResponseException::~ResponseException() throw() {};