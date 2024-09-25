/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 08:04:58 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/25 10:04:29 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"



int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: ./webserv <config_file>" << std::endl;
        return 1;
    }

    std::string configPath(argv[1]);

    try
    {
        Webserv webserv(configPath);
        // (void) webserv;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Config file error: " << e.what() << '\n';
    }

    // while (1);
}