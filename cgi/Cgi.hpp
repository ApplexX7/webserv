/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 09:43:59 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/11/10 16:16:24 by mohilali         ###   ########.fr       */
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
        void setCgiFullPath(std::string &_Path);
        void setExtension(std::string _extension);
        void setUpCgenv(std::vector<std::string> &_extension);
        void setDirectPath(std::string _path);
        std::vector<std::string> &getCgiEnv();
        int executeCgi(Client &clientData);
        void cgiExecution(Client &clientData);
        void setCgiPath(std::string _cgiPath);
        std::string &getCgiFileName();
        int &getFileResponse();
        void reset();
    private:
        time_t Cgi_timeout;
        bool thereIsOne;
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