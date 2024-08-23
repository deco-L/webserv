/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configMain.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/08/23 14:11:38 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Config.hpp"

void  configMain(Config config, int argc, char**argv) {
  config = Config(argc, argv);
  config.check_nbr_arg();
}
