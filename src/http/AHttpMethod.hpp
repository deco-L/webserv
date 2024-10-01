/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpMethod.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/01 21:39:52 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AHTTPMETHOD_HPP
#define AHTTPMETHOD_HPP

#include <iostream>

class Socket;
class HttpHeader;
class HttpResponse;

class AHttpMethod {
private:
  AHttpMethod(const AHttpMethod& obj);
  AHttpMethod& operator=(const AHttpMethod& obj);

protected:
  const std::string _method;

  class MethodError : public std::exception {
  private:
    std::string _error_message;
  
  public:
    MethodError(std::string error);
    virtual ~MethodError() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW;
    virtual const char* what(void) const throw();
  };

public:
  AHttpMethod(void);
  AHttpMethod(std::string method);
  virtual ~AHttpMethod();

  const std::string& getMethod(void) const;
  virtual void execute(Socket& socket, HttpHeader& header, HttpResponse* response) = 0;
};

#endif
