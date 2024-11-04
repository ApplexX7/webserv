/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 09:43:59 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/11/04 18:52:54 by mohilali         ###   ########.fr       */
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
        void setCgiFullPath(std::string &_Path);
        void setExtension(std::string _extension);
        void setUpCgenv(std::vector<std::string> &_extension);
        std::vector<std::string> &getCgiEnv();
        int executeCgi(Client &clientData);
        void cgiExecution(Client &clientData, int fd, char **env);
        int &getFileResponse();
    private:
        std::string cgiFullpath;
        int fileResponse;
        std::string extension;
        std::vector<std::string> envCgi;
};

#endif