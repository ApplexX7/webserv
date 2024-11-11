/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 09:45:37 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/11/11 18:58:01 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"
#include "signal.h"

Cgi::Cgi( void ) {
	this->thereIsOne = false;
}
Cgi::~Cgi( void ) {};

int &Cgi::getFileResponse(){
	return (this->fileResponse);
}

int Cgi::getFileOffset( void ){
	return (this->fileOfsset);
}

void Cgi::reset(){
	this->fileOfsset = 0;
	this->Cgi_timeout = 0;
	this->thereIsOne = false;
	this->fileName = "";
	this->direcpath = "";
}

std::string &Cgi::getCgiFileName(){
	return (this->fileName);
}

std::vector<std::string> &Cgi::getCgiEnv(){
	return (this->envCgi);
}

void Cgi::setUpCgenv(std::vector<std::string> &_env){
	this->envCgi = _env;
}

void Cgi::setCgiFullPath(std::string &_Path){
	this->cgiFullpath = _Path;
}

void Cgi::setExtension(std::string _extension){
	this->extension = _extension;
}

void Cgi::setDirectPath(std::string _path){
	this->direcpath = _path;
}

void Cgi::setCgiPath(std::string _CgiPath){
	this->cgiPath = _CgiPath;
}

void Cgi::cgiExecution(Client &clientData){
	char **args;
	char **env;
	
	if (chdir(this->direcpath.c_str()) == -1){
		exit (1);
	}
	args = (char **) new char *[3];
	if (!args){
		exit (1);
	}
	args[0] = strdup(this->cgiPath.c_str());
	std::string path3 = clientData.getRequest().getUri().erase(0, 1);
	args[1] = strdup(path3.c_str());
	if (!args[1])
		exit (1);
	args[2] = NULL;
	if (clientData.getRequest().getmethode() == "POST"){
		int fd_input = open(clientData.getResponse().getCginputFile().c_str(), O_RDONLY);
		if (fd_input == -1){
			exit(1);
		}
		if (dup2(fd_input, STDIN_FILENO) == -1){
			exit (1);
		}
		close(fd_input);
	}
	this->fileResponse = open(this->fileName.c_str(),O_CREAT | O_RDWR, 0644);
	if (this->fileResponse == -1){
		exit (1);
	}
	if (dup2(this->fileResponse, STDOUT_FILENO) == -1){
		exit (1);
	}
	close(this->fileResponse);
	env = (char **) new char *[this->envCgi.size() + 1];
	if (!env){
		exit (1);
	}
	for (size_t i = 0; i < this->envCgi.size(); i++){
		env[i] = strdup(this->envCgi[i].c_str());
		if (!env[i]){
			exit(1);
		}
	}
	env[this->envCgi.size()] = 0;
	if (execve(this->cgiPath.c_str(), args , env) == -1){
		perror("Erorrr :");
		for (int i = 0; env[i] != NULL; i++){
			delete [] env[i];
		}
		delete [] env;
		delete [] args;
		exit (1);
	}
}

int Cgi::extractHeadrs(Client &clientData){
	size_t pos = 0;
	int _offset = 0;
	std::string line;
	std::string name;
	std::string Value;
	std::ifstream parseFile(this->fileName);

	while (std::getline(parseFile, line)){
		if (line.empty())
			break;
		if (line == "/r"){
			_offset += 4;
			break;
		}
		size_t secpos = line.find("\r");
		if (secpos == std::string::npos)
		{
			_offset = 0;
			break;
		}
		pos = line.find(':');
		_offset += line.length() + 1;
		if (pos == std::string::npos){
			break;
		}
		name = line.substr(0, pos);
		Value = line.substr(pos + 1);
		size_t spa = name.find(' ');
		size_t tab = name.find('\t');
		if (spa != std::string::npos || tab != std::string::npos){
			break;
		}
		Value.erase(std::remove_if(Value.begin(),Value.end(), ::isspace), Value.end());
		clientData.getResponse().setCgiHeaders(name, Value);
	}
	if (clientData.getResponse().getCgiHeaderValue("Content-Type").empty()){
		clientData.getResponse().setCgiHeaders("Content-Type", "text/plain");
	}
	if (clientData.getResponse().getCgiHeaderValue("Content-Length").empty()){
		size_t size;
		parseFile.seekg(0, std::ios::end);
		size = parseFile.tellg();
		std::cout << "size : " << size << std::endl;
		clientData.getResponse().setCgiHeaders("Content-Length", std::to_string(size - _offset));
	}
	this->fileOfsset = _offset;
	parseFile.close();
	return (0);
}


int Cgi::executeCgi(Client &clientData) {
	int status;

	std::srand(std::time(NULL));
	if (!this->thereIsOne){
		this->fileName = "/tmp/." +  clientData.getResponse().generateFileName() + std::to_string(std::rand());
		this->pid = fork();
		if (this->pid == -1){
			clientData.getResponse().setStatusCode(500);
			return (1);
		}
		if (this->pid == 0){
			this->cgiExecution(clientData);
			return (1);
		}
		this->Cgi_timeout = std::time(NULL);
		this->thereIsOne = true;
	}
	if(std::time(NULL) - this->Cgi_timeout >= 10){
		std::cout << "Time Out" << std::endl;
		kill(pid, SIGTERM);
		remove(this->fileName.c_str());
		close(this->fileResponse);
		this->thereIsOne = false;
		this->fileResponse = -1;
		clientData.getResponse().setStatusCode(408);
		return (1);
	}
	if (waitpid(this->pid, &status, WNOHANG) != 0){
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1){
			remove(this->fileName.c_str());
			clientData.getResponse().setStatusCode(505);
			close(this->fileResponse);
			this->fileResponse = -1;
			this->thereIsOne = false;
		}
		else if (this->extractHeadrs(clientData)){
			remove(this->fileName.c_str());
			clientData.getResponse().setStatusCode(500);
			close(this->fileResponse);
			this->fileResponse = -1;
			this->thereIsOne = false;
		}
		close(this->fileResponse);
		this->thereIsOne = false;
		return (1);
	}
	close(this->fileResponse);
	return (0);
}