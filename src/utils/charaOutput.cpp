/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   charaOutput.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miyazawa.kai.0823 <miyazawa.kai.0823@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/12/13 14:31:22 by miyazawa.ka      ###   ########.fr       */
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
