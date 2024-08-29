/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   charaOutput.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/08/29 23:22:33 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void  spinnerOut(void) {
  const std::vector<std::string>  spinnerChara = {"|", "¥", "-", "\\", " "};
  std::vector<std::string>::const_iterator  it;

  for (it = spinnerChara.begin(); it != spinnerChara.end(); it++) {
    std::cout << '\b' << *it;
    std::cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(120));
  }
  return ;
}