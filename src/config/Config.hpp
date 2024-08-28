/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/08/28 14:08:28 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

class Config {
private:
  int         _argc;
  std::string _file_path;

public:
  Config(void);
  Config(int argc, char** argv);
  ~Config();

  int         getArgc(void) const;
  std::string getFileName(void) const;
  void        check_nbr_arg(void) const;

  Config&      operator=(const Config& obj);
};

#endif
