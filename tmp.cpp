#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define PORT 8080
#define MAX_EVENTS 10
#define BUFFER_SIZE 1024
#define TIMEOUT_MS 1000 // 1秒のタイムアウト

// シンプルなHTTPレスポンスを生成する関数
std::string generate_response(const std::string& content) {
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/plain\r\n";
    response += "Content-Length: " + std::to_string(content.size()) + "\r\n";
    response += "Connection: close\r\n";
    response += "\r\n"; // ヘッダーとボディの区切り
    response += content; // ボディ
    return response;
}

// ソケットを作成する関数
int create_server_socket() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // ソケットを再利用可能に設定
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // 任意のアドレスをバインド
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

int main() {
    int server_fd = create_server_socket();
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    std::cout << "HTTP Server is running on port " << PORT << "..." << std::endl;

    struct epoll_event events[MAX_EVENTS];

    while (true) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, TIMEOUT_MS);
        if (nfds == 0) {
            // タイムアウト処理
            std::cout << "Timeout occurred, no events to process." << std::endl;
            continue; // タイムアウトが発生した場合は次のループへ
        }
        
        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == server_fd) {
                // 新しいクライアント接続の処理
                int client_fd = accept(server_fd, NULL, NULL);
                if (client_fd < 0) {
                    perror("accept");
                    continue;
                }
                fcntl(client_fd, F_SETFL, O_NONBLOCK); // ノンブロッキングに設定
                ev.events = EPOLLIN | EPOLLET; // エッジトリガー
                ev.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                    perror("epoll_ctl: client_fd");
                    close(client_fd);
                }
            } else {
                // クライアントからのリクエストを処理
                char buffer[BUFFER_SIZE];
                memset(buffer, 0, sizeof(buffer));
                int bytes_read = read(events[i].data.fd, buffer, sizeof(buffer) - 1);
                if (bytes_read <= 0) {
                    // エラーまたはクライアントが接続を閉じた場合
                    close(events[i].data.fd);
                } else {
                    // リクエスト内容を出力
                    std::cout << "Received request:\n" << buffer << std::endl;

                    // 簡単なレスポンスを生成
                    std::string response = generate_response("Hello, World!");
                    write(events[i].data.fd, response.c_str(), response.size());
                    close(events[i].data.fd); // クライアント接続を閉じる
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
