/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 08:04:58 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/11/13 12:29:53 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

int main(int argc, char **argv)
{
    std::string path;
    signal(SIGPIPE, SIG_IGN);

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