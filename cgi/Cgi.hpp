/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 09:43:59 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/11/13 09:56:46 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP

# define CGI_HPP

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <vector>
# include <iostream> 
# include "../utils/Client.hpp"
# include "../Request/Request.hpp"
# include "../utils/Client.hpp"
class Client;

class Cgi {
    public:
        Cgi( void );
        ~Cgi( void );
        int extractHeadrs(Client &clientData);
        int getFileOffset( void );
        void setCgiFullPath(std::string &_Path);
        void setExtension(std::string _extension);
        void setUpCgenv(std::vector<std::string> &_extension);
        void setDirectPath(std::string _path);
        std::vector<std::string> &getCgiEnv();
        int executeCgi(Client &clientData);
        void cgiExecution(Client &clientData);
        void setCgiPath(std::string _cgiPath);
        int CgiMonitore(Client &clientData);
        std::string &getCgiFileName();
        int &getFileResponse();
        void reset();
    private:
        time_t Cgi_timeout;
        int fileResponse;
        int pid;
        int fileOfsset;
        std::string cgiPath;
        std::string fileName;
        std::string direcpath;
        std::string cgiFullpath;
        std::string extension;
        std::vector<std::string> envCgi;
};

#endif