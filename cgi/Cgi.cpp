/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 09:45:37 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/11/05 18:14:03 by mohilali         ###   ########.fr       */
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

void Cgi::setDirectPath(std::string _path){
	this->direcpath = _path;
}

void Cgi::cgiExecution(Client &clientData){
	const char *path;
	char **args;
	char **env;
	std::string pathCgifile = clientData.getRequest().getServerLocation()->getCgiPath(this->extension);

	if (chdir(this->direcpath.c_str()) == -1){
		exit (1);
	}
	path = pathCgifile.c_str();
	args = (char **) new char *[3];
	if (!args)
		exit (1);
	args[0] = strdup(path);
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
	if (dup2(this->fileResponse, STDOUT_FILENO) == -1){
		exit (1);
	}
	close(this->fileResponse);
	env = (char **) new char *[this->envCgi.size()];
	if (!env){
		clientData.getResponse().setStatusCode(500);
		exit (1);
	}
	for (size_t i = 0; i < this->envCgi.size(); i++){
		env[i] = strdup(this->envCgi[i].c_str());
		if (!env[i]){
			exit(1);
		}
	}
	env[this->envCgi.size()] = 0;
	if (execve(path, args, env) == -1){
		for (int i = 0; env[i] != NULL; i++){
			delete [] env[i];
		}
		delete [] env;
		delete [] args;
		exit (1);
	}
}

int Cgi::executeCgi(Client &clientData) {
	int pid;
	int status;
	std::string filename;

	filename = "/tmp/" + clientData.getResponse().generateFileName() + ".txt";
	this->fileResponse = open(filename.c_str(),O_CREAT | O_RDWR, 0644);
	if (this->fileResponse == -1){
		clientData.getResponse().setStatusCode(500);
		return (1);
	}
	pid = fork();
	if (pid == -1){
		clientData.getResponse().setStatusCode(500);
		return (1);
	}
	if (pid == 0){
		this->cgiExecution(clientData);
		clientData.getResponse().setStatusCode(500);
		return (1);
	}
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) == 1){
		clientData.getResponse().setStatusCode(500);
		perror("Errro in child process:: ");
		return (1);
	}
	if (lseek(this->fileResponse, 0, SEEK_SET) == -1){
		remove(filename.c_str());
		clientData.getResponse().setStatusCode(500);
		return (1);
	}
	remove(filename.c_str());
	return (0);
}