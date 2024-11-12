/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 09:45:37 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/11/12 19:13:11 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"
#include "signal.h"

Cgi::Cgi( void ) {
	this->fileOfsset = 0;
	this->Cgi_timeout = 0;
	this->fileName = "";
	this->direcpath = "";
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
	char **env;
	char *args[3] = {
    	const_cast<char*>(this->cgiPath.c_str()),
    	const_cast<char*>(clientData.getRequest().getUri().erase(0, 1).c_str()),
    	NULL
	};
	if (chdir(this->direcpath.c_str()) == -1){
		exit (1);
	}
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
		env[i] =  new  char[this->envCgi[i].length()];
		std::strcpy(env[i], this->envCgi[i].c_str());
		if (!env[i]){
			exit(1);
		}
	}
	env[this->envCgi.size()] = 0;
	if (execve(this->cgiPath.c_str(), args , env) == -1){
		for (int i = 0; env[i] != NULL; i++){
			delete [] env[i];
		}
		delete [] env;
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
	this->fileOfsset = _offset;
	parseFile.close();
	return (0);
}

static std::string int_to_string(int num){
	std::ostringstream oss;
	oss << num;
	std::string str = oss.str();
	return (str);
}

int Cgi::CgiMonitore(Client &clientData){
	int status;
	if(std::time(NULL) - this->Cgi_timeout >= 10){
		if (this->pid != -1)
			kill(this->pid, SIGTERM);
		remove(this->fileName.c_str());
		close(this->fileResponse);
		this->thereIsOne = false;
		this->fileResponse = -1;
		clientData.getResponse().setStatusCode(408);
		clientData.responseReady = true;
		return (1);
	}
	if (waitpid(this->pid, &status, WNOHANG) == this->pid){
		clientData.responseReady = true;
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1){
			remove(this->fileName.c_str());
			clientData.getResponse().setStatusCode(500);
			close(this->fileResponse);
			this->fileResponse = -1;
		}
		else if (this->extractHeadrs(clientData)){
			remove(this->fileName.c_str());
			clientData.getResponse().setStatusCode(500);
			close(this->fileResponse);
			this->fileResponse = -1;
		}
		if (this->pid != -1)
			kill(this->pid, SIGTERM);
		return (1);
	}
	return (0);
}

int Cgi::executeCgi(Client &clientData) {
	std::srand(std::time(NULL));
	this->fileName = "/tmp/." +  clientData.getResponse().generateFileName() + std::to_string(std::rand()) + int_to_string(clientData.getFd());
	this->pid = fork();
	if (this->pid == -1){
		clientData.getResponse().setStatusCode(500);
		return (1);
	}
	if (this->pid == 0){
		signal(SIGTERM, SIG_DFL);
		this->cgiExecution(clientData);
		return (1);
	}
	this->Cgi_timeout = std::time(NULL);
	return (0);
}