/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpPost.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/12/06 20:39:26 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPOST_HPP
#define HTTPPOST_HPP

#include <fstream>
#include "AHttpMethod.hpp"

class HttpPost : public AHttpMethod {
private:
  HttpPost(void);
  HttpPost(const HttpPost& obj);
  HttpPost& operator=(const HttpPost& obj);

  bool _uploadFile(HttpRequest& request);

public:
  HttpPost(std::string uri, std::string version);
  ~HttpPost();

  void setResponseMessage(const ConfigServer& config, HttpRequest& request, HttpResponse& response) const;
  void execute(const ConfigServer& config, HttpRequest& header, HttpResponse*& response);
};

#endif