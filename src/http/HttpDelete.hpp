/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpDelete.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/09/28 15:32:33 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPDELETE_HPP
#define HTTPDELETE_HPP

#include "AHttpMethod.hpp"

class HttpDelete : public AHttpMethod {
private:
  HttpDelete(const HttpHeader& obj);
  HttpHeader& operator=(const HttpHeader& obj);

public:
  HttpDelete(void);
  ~HttpDelete();

  void execute(void);
};

#endif
