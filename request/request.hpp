/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:18:27 by mohilali          #+#    #+#             */
/*   Updated: 2024/09/24 16:31:10 by mohilali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST
#define REQUEST

#include <iostream>
#include <string>
#include <stdexcept>

class Request{
    private:
        std::string methode;
        std::string Uri;
        std::string Host;
    public:
        Request();
        Request(const Request &obj);
        Request& operator=(const Request &ope);
        ~Request();
        void Setmethode(std::string _metode);
        void SetHost(std::string _Host);
        std::string getHost();
        std::string getMethode();
        std::string getUri();
        void SetUri(std::string _Uri);
};

#endif