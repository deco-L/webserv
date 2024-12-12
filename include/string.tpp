/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.tpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/03 16:07:40 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

namespace mylib{
  template <typename T>
  std::string to_string( const T& n ) {
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
  }

  template <typename T>
  std::string nbrToS(T nbr) {
    std::stringstream nbrToS;

    nbrToS << nbr;
    if (nbrToS.fail())
      return ("");
    return (nbrToS.str());
  }
}