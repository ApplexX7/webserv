/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 09:43:59 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/11/05 17:44:17 by mohilali         ###   ########.fr       */
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
        void setDirectPath(std::string _path);
        std::vector<std::string> &getCgiEnv();
        int executeCgi(Client &clientData);
        void cgiExecution(Client &clientData);
        int &getFileResponse();
    private:
        std::string direcpath;
        std::string cgiFullpath;
        int fileResponse;
        std::string extension;
        std::vector<std::string> envCgi;
};

#endif