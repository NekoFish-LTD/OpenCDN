#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
using namespace std;
// 检查请求中是否包含指定的key
bool has_key(const char* buffer, const char* key) {
    std::string haystack(buffer);
    std::string needle(key);
    return haystack.find("POST /api/v1") != std::string::npos && haystack.find(key) != std::string::npos;
}

int main(int argc, char* argv[]) {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];

    // 创建套接字
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 绑定端口和地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(5477);
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听端口
    if (listen(server_fd, 1) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server started, listening on port 5477..." << std::endl;

    while (1) {
        socklen_t client_addr_len = sizeof(client_addr);
        // 接受客户端连接
        if ((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        std::cout << "New connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;

        // 读取请求
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t read_size = read(client_fd, buffer, BUFFER_SIZE);
        if (read_size < 0) {
            perror("read failed");
            close(client_fd);
            continue;
        }

        // 判断是否为 POST 请求，并检查key
        if (has_key(buffer, "my_secret_key")) {
            char* post_path = strstr(buffer, "POST /api/v1");
            if (post_path) {
                // 查找请求头结束标记
                void* header_end = memmem(buffer, read_size, "\r\n\r\n", 4);
                if (header_end) {
                    // 获取请求体
                    std::string body(buffer, buffer + read_size);
                    std::string domain_key = "domain="; // 获取请求中的Domain变量
                    std::string mod_key = "mod=";
                    std::string enable_ssl = "ssl=";
                    std::string waf = "waf=";
                    std::string uam = "uam=";
                    std::size_t domain_pos = body.find(domain_key);
                    std::size_t mod_pos = body.find(mod_key);
                    std::size_t enable_ssl_pos = body.find(enable_ssl);
                    std::size_t waf_pos = body.find(waf);
                    std::size_t uam_pos = body.find(uam);
                    if (mod_pos != std::string::npos) {
                    std::string mod_value = body.substr(mod_pos + mod_key.length(), body.length() - mod_pos - mod_key.length());
                    switch (mod_value) {
                        case create:
                            create();
                        case delete:
                            delete();
                        case uamon:
                            uamon()
                        case uamoff:
                            uamoff();
                        case wafon:
                            wafon();
                        case wafoff:
                            wafoff();
                        case info:
                            info();
                        case statistics:
                            statistics();
                        default:
                            std::cout << "Mod error!" <<< std::endl;
                    }
                    if (domain_pos != std::string::npos) {
                        std::string domain_value = body.substr(domain_pos + domain_key.length(), body.length() - domain_pos - domain_key.length()); // 实际使用domain_cualue变量
            } else {
            // 发送响应
            std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
            if (send(client_fd, response.c_str(), response.length(), 0) < 0) {
            perror("send failed");
            close(client_fd);
            continue;
            }
                std::cout << "Invalid request handled" << std::endl;
            }
                } else {
                    // 发送响应
                    std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
                    if (send(client_fd, response.c_str(), response.length(), 0) < 0) {
                        perror("send failed");
                        close(client_fd);
                        continue;
                }

            std::cout << "Invalid key" << std::endl;
        }

        // 关闭连接
        close(client_fd);
    }

// 关闭服务器套接字
close(server_fd);

return 0;
}