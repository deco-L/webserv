/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/08/29 17:49:18 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Error.hpp"

void  outputError(std::string error) {
  std::cerr << ERROR_COLOR << error << COLOR_RESET << std::endl;
  return ;
}
