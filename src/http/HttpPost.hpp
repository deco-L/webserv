/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpPost.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/29 19:47:01 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPOST_HPP
#define HTTPPOST_HPP

#include "AHttpMethod.hpp"

class HttpPost : public AHttpMethod {
private:
  HttpPost(const HttpPost& obj);
  HttpPost& operator=(const HttpPost& obj);

public:
  HttpPost(void);
  ~HttpPost();

  void execute(Socket& socket, HttpHeader& header, HttpResponse& response);
};

#endif
