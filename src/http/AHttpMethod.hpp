/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpMethod.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/28 15:29:56 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AHTTPMETHOD_HPP
#define AHTTPMETHOD_HPP

#include <iostream>

class Socket;
class Http;
class HttpHeader;

class AHttpMethod {
private:
  AHttpMethod(void);
  AHttpMethod(const AHttpMethod& obj);
  AHttpMethod& operator=(const AHttpMethod& obj);

protected:
  std::string _method;

public:
  AHttpMethod(std::string method);
  virtual ~AHttpMethod() = 0;

  void executeMethod(Socket socket, Http http, HttpHeader header);
};

#endif
