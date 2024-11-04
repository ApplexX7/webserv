/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 09:45:37 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/11/04 19:03:02 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

Cgi::Cgi( void ) {};
Cgi::~Cgi( void ) {};

int &Cgi::getFileResponse(){
	return (this->fileResponse);
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

void Cgi::cgiExecution(Client &clientData, int fd, char **env){
	const char *path;
	char **args;
	std::string outfile = "/tmp/" + clientData.getResponse().generateFileName();
	std::string pathCgifile = clientData.getRequest().getServerLocation()->getCgiPath(this->extension);
	path = pathCgifile.c_str();
	args = (char **)malloc(sizeof(char *) * 2);
	if (!args)
		exit (1);
	args[0] = strdup(path);
	if (!args[1])
		exit (1);
	args[1] = NULL;
	if (clientData.getRequest().getmethode() == "POST"){
		if (dup2(STDIN_FILENO, fd) == -1){
			exit (1);
		}
		close(fd);
	}
	if (dup2(STDOUT_FILENO, this->fileResponse) == -1){
		exit (1);
	}
	if (execve(path, args, env) == -1){
		for (int i = 0; env[i] != NULL; i++){
			free(env[i]);
		}
		free(args);
		exit (1);
	}
}

int Cgi::executeCgi(Client &clientData) {
	char **env;
	int pid;
	int status;
	std::string filename;

	filename = clientData.getResponse().generateFileName();
	this->fileResponse = open(filename.c_str(), O_WRONLY);
	if (this->fileResponse == -1){
		clientData.getResponse().setStatusCode(500);
		return (1);
	}
	env = (char **)malloc(sizeof(char *) * this->envCgi.size());
	if (!env){
		clientData.getResponse().setStatusCode(500);
		return (1);
	}
	for (size_t i = 0; i < this->envCgi.size(); i++){
		env[i] = strdup(this->envCgi[i].c_str());
		if (!env[i]){
			clientData.getResponse().setStatusCode(500);
			return (1);
		}
	}
	env[this->envCgi.size()] = 0;
	pid = fork();
	if (pid == -1){
		clientData.getResponse().setStatusCode(500);
		return (1);
	}
	if (pid == 0){
		this->cgiExecution(clientData, 0,env);
		clientData.getResponse().setStatusCode(500);
		return (1);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) == 1){
		clientData.getResponse().setStatusCode(500);
		return (1);
	}
	if (lseek(this->fileResponse, 0, SEEK_SET) == -1){
		unlink(filename.c_str());
		clientData.getResponse().setStatusCode(500);
		return (1);
	}
	unlink(filename.c_str());
	return (0);
}