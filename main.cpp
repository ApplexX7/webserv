/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 08:04:58 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/11/12 17:43:00 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

void leaks() {
    system("lsof -c webserv");
}

int main(int argc, char **argv)
{
    std::string path;
    signal(SIGPIPE, SIG_IGN);
    
    atexit(leaks);

    // if (argc != 2)
    // {
    //     std::cout << "Usage: ./webserv <config_file>" << std::endl;
    //     return 1;
    // }

    if (argc > 1)
        path = argv[1];
    else
        path = "./config/default.conf";

    std::string configPath(path);
    try
    {
        Webserv webserv;
        webserv.init(configPath);
        webserv.listen();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Config file error: " << e.what() << '\n';
    }
}