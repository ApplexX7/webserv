/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Field.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 09:40:03 by wbelfatm          #+#    #+#             */
/*   Updated: 2024/09/26 08:31:30 by wbelfatm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Field.hpp"

Field::Field( void ) {
}

void Field::addValue( std::string value ) {
    this->values.push_back(value);
}

std::vector<std::string> Field::getValues( void ) {
    return this->values;
}

void Field::updateValue( std::string newValue, int index ) {
    this->values[index] = newValue;
}