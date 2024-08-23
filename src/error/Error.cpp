/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/08/22 18:20:29 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Error.hpp"

void  outputError(std::string error) {
  std::cerr << error << std::endl;
  return ;
}