/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpGet.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/11/16 22:32:56 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPGET_HPP
#define HTTPGET_HPP

#include "AHttpMethod.hpp"

class HttpGet : public AHttpMethod {
private:
  HttpGet(void);
  HttpGet(const HttpGet& obj);
  HttpGet& operator=(const HttpGet& obj);

public:
  HttpGet(std::string uri, std::string version);
  ~HttpGet();

  void setResponseMessage(const ConfigServer& config, HttpRequest& request, HttpResponse& response) const;
  void execute(const ConfigServer& config, HttpRequest& request, HttpResponse*& response);
};

#endif
