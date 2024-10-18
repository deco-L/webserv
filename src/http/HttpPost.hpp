/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpPost.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/10/18 12:10:51 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPOST_HPP
#define HTTPPOST_HPP

#include "AHttpMethod.hpp"

class HttpPost : public AHttpMethod {
private:
  std::string _uri;
  std::string _version;

  HttpPost(void);
  HttpPost(const HttpPost& obj);
  HttpPost& operator=(const HttpPost& obj);

public:
  HttpPost(std::string uri, std::string version);
  ~HttpPost();

  const std::string& getUri(void) const;
  const std::string& getVersion(void) const;
  HttpResponse* setResponseStatus(void);
  void setResponseMessage(HttpRequest& request, HttpResponse& response) const;
  void execute(HttpRequest& header, HttpResponse*& response);
};

#endif
