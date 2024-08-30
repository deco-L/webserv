/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketMain.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#             */
/*   Updated: 2024/08/31 04:51:36 by csakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Socket.hpp"
#include "Error.hpp"

#define SIZE (5*1024)

unsigned int  getFilesize(const char* path) {
  int size, read_size;
  char  read_buf[SIZE];
  FILE* f;

  f = fopen(path, "rb");
  if (f == NULL) {
    return (0);
  }
  size = 0;
  do {
    read_size = fread(read_buf, 1, SIZE, f);
    size += read_size;
  } while (read_size != 0);
  fclose(f);
  return (size);
}

int recvRequestMessage(int sock, char *request_message, unsigned int buf_size) {
  int recv_size;

  recv_size = recv(sock, request_message, buf_size, 0);
  return (recv_size);
}

int parseRequestMessage(char* method, char* target, char* request_message) {
  char* line;
  char* tmp_method;
  char* tmp_target;

  line = strtok(request_message, "\n");

  tmp_method = strtok(line, " ");
  if (tmp_method == NULL) {
    std::cout << ERROR_COLOR << "get method error" << COLOR_RESET << std::endl;
    return (-1);
  }
  strcpy(method, tmp_method);

  tmp_target = strtok(NULL, " ");
  if (tmp_target == NULL) {
    std::cout << ERROR_COLOR << "get target error" << COLOR_RESET << std::endl;
    return (-1);
  }
  strcpy(target, tmp_target);
  return (WSV_OK);
}

int getProcessing(char* body, char* file_path) {
  FILE* f;
  int   file_size;

  file_size = getFilesize(file_path);
  if (file_size == 0) {
    std::cout << ERROR_COLOR << "getFilesize error." << COLOR_RESET << std::endl;
    return 404;
  }
  f = fopen(file_path, "r");
  fread(body, 1, file_size, f);
  fclose(f);
  return (200);
}

int createResponseMessage(char* response_message, int status, char* header, char* body, unsigned int body_size) {
  unsigned int  no_body_len;
  unsigned int  body_len;

  response_message[0] = '\0';

  if (status == 200) {
    sprintf(response_message, "HTTP/1.1 200 OK\r\n%s\r\n", header);
    no_body_len = strlen(response_message);
    body_len = body_size;
    memcpy(&response_message[no_body_len], body, body_len);
  } else if (status == 404) {
    sprintf(response_message, "HTTP/1.1 404 Not Found\r\n%s\r\n", header);

    no_body_len = strlen(response_message);
    body_len = 0;
  } else {
    std::cout << ERROR_COLOR << "Not support status(" << status << ")" << COLOR_RESET << std::endl;
    return (-1);
  }
  return (no_body_len + body_len);
}

int sendResponseMessage(int sock, char* response_message, unsigned int message_size) {
  int send_size;

  send_size = send(sock, response_message, message_size, 0);
  return (send_size);
}

int httpServer(int sock) {
  int request_size, response_size;
  char  request_message[SIZE];
  char  response_message[SIZE];
  char  method[SIZE];
  char  target[SIZE];
  char  header_field[SIZE];
  char  body[SIZE];
  int status;
  unsigned int  file_size;

  while (true)
  {
    request_size = recvRequestMessage(sock, request_message, SIZE);
    if (request_size == -1) {
      std::cout << ERROR_COLOR << "recvRequestMessage error" << COLOR_RESET << std::endl;
      break ;
    } else if (request_size == 0) {
      std::cout << "connection endes" << std::endl;
      break ;
    }

    std::cout << request_message << std::endl;

    if (parseRequestMessage(method, target, request_message) == -1) {
      std::cout << ERROR_COLOR << "parseRequestMessage error" << COLOR_RESET << std::endl;
      break;
    }

    if (strcmp(method, "GET") == 0) {
      if (strcmp(target, "/") == 0) {
        strcpy(target, "/index.html");
      }
      status = getProcessing(body, &target[1]);
    } else {
      status = 404;
    }

    file_size = getFilesize(&target[1]);
    sprintf(header_field, "Content-Length: %u\r\n", file_size);

    response_size = createResponseMessage(response_message, status, header_field, body, file_size);
    if (response_size == -1) {
      std::cout << ERROR_COLOR << "createResponseMessage error" << COLOR_RESET << std::endl;
      break ;
    }

    std::cout << response_message << std::endl;

    sendResponseMessage(sock, response_message, response_size);
  }

  return (WSV_OK);
}

void  socketMain(Socket socketData) {
  socketData = Socket();

  int w_addr, c_sock;
  struct sockaddr_in a_addr;

  w_addr = socket(AF_INET, SOCK_STREAM, 0);
  if (w_addr == -1) {
    std::cerr << ERROR_COLOR << "socket error." << COLOR_RESET << std::endl;
    std::exit(EXIT_FAILURE);
  }
  std::memset(&a_addr, 0, sizeof(struct sockaddr_in));
  a_addr.sin_family = AF_INET;
  a_addr.sin_port = htons((unsigned int)8080);
  a_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (bind(w_addr, (const struct sockaddr*)&a_addr, sizeof(a_addr) == -1)) {
    std::cerr << ERROR_COLOR << "bind error." << COLOR_RESET << std::endl;
    close(w_addr);
    std::exit(EXIT_FAILURE);
  }

  if (listen(w_addr, WSV_LISTEN_BACKLOG) == -1) {
    std::cout << ERROR_COLOR << "listen error." << COLOR_RESET << std::endl;
    close(w_addr);
    std::exit(EXIT_FAILURE);
  }

  while (true) {
    std::cout << "Waiting connect..." << std::endl;
    c_sock = accept(w_addr, NULL, NULL);
    if (c_sock == -1) {
      std::cerr << ERROR_COLOR << "accept error." << COLOR_RESET << std::endl;
      close(w_addr);
      std::exit(EXIT_FAILURE);
    }
    std::cout << "Connected!" << std::endl;
    httpServer(c_sock);
    close(c_sock);
  }
  close(w_addr);
  return ;
}