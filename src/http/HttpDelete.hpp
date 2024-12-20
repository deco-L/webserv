/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpDelete.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/12/06 16:16:32 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPDELETE_HPP
#define HTTPDELETE_HPP

#include "AHttpMethod.hpp"
#include <cstdio>

class HttpDelete : public AHttpMethod {
private:
  HttpDelete(void);
  HttpDelete(const HttpDelete& obj);
  HttpDelete& operator=(const HttpDelete& obj);

public:
  HttpDelete(std::string uri, std::string version);
  ~HttpDelete();

  void setResponseMessage(const ConfigServer& config, HttpRequest& request, HttpResponse& response) const;
  void execute(const ConfigServer& config, HttpRequest& request, HttpResponse*& response);
};

#endif
