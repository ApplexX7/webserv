/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 08:04:58 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/10/19 10:17:55 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

void leaks() {
    // system("leaks webserv");
}

int main(int argc, char **argv)
{
    signal(SIGPIPE, SIG_IGN);
    
    atexit(leaks);

    if (argc != 2)
    {
        std::cout << "Usage: ./webserv <config_file>" << std::endl;
        return 1;
    }

    std::string configPath(argv[1]);

    try
    {
        Webserv webserv;
        webserv.init(configPath);
        webserv.listen();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Config file error: " << e.what() << '\n';
    }
}