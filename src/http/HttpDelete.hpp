/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpDelete.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/18 16:45:07 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPDELETE_HPP
#define HTTPDELETE_HPP

#include "AHttpMethod.hpp"
#include <cstdio>

class HttpDelete : public AHttpMethod {
private:
  std::string _uri;
  std::string _version;

  HttpDelete(void);
  HttpDelete(const HttpDelete& obj);
  HttpDelete& operator=(const HttpDelete& obj);

public:
  HttpDelete(std::string uri, std::string version);
  ~HttpDelete();

  const std::string& getUri(void) const;
  const std::string& getVersion(void) const;
  HttpResponse* setResponseStatus(void);
  void setResponseMessage(HttpRequest& request, HttpResponse& response) const;
  void execute(HttpRequest& request, HttpResponse*& response);
};

#endif
