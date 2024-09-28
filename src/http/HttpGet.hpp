/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpGet.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/28 15:32:25 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPGET_HPP
#define HTTPGET_HPP

#include "AHttpMethod.hpp"

class HttpGet : public AHttpMethod {
private:
  HttpGet(const HttpGet& obj);
  HttpGet& operator=(const HttpGet& obj);

public:
  HttpGet(void);
  ~HttpGet();

  void execute(void);
};

#endif
