/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   charaOutput.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/08 20:11:03 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


namespace mylib {
  void  spinnerOut(void) {
    const std::string spinnerArray[] = {"|", "¥", "-", "\\", " "};
    const std::vector<std::string> spinnerChara(spinnerArray, spinnerArray + sizeof(spinnerArray) / sizeof(spinnerArray[0]));
    std::vector<std::string>::const_iterator  it;

    for (it = spinnerChara.begin(); it != spinnerChara.end(); it++) {
      std::cout << '\b' << *it;
      std::cout.flush();
      struct timeval tv;
      tv.tv_sec = 0;
      tv.tv_usec = 120000;
      select(0, NULL, NULL, NULL, &tv);
    }
    return ;
  }
}
