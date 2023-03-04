#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
using namespace std;

int main(int argc, char* argv[]) {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // 创建套接字
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址和端口
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5477);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

    // 连接服务器
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    // 发送 POST 请求，包含指定的 key 和一些数据
    std::string request = "POST /post HTTP/1.1\r\n";
    request += "Content-Type: text/plain\r\n";
    request += "Content-Length: 5\r\n";
    request += "Connection: close\r\n";
    request += "my_secret_key: secret\r\n";
    request += "\r\n";
    request += "hello";
    if (send(client_fd, request.c_str(), request.length(), 0) < 0) {
        perror("send failed");
        exit(EXIT_FAILURE);
    }

    // 读取服务器响应
    memset(buffer, 0, BUFFER_SIZE);
    ssize_t read_size = read(client_fd, buffer, BUFFER_SIZE);
    if (read_size < 0) {
        perror("read failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    // 打印服务器响应
    std::cout << buffer << std::endl;

    // 关闭连接
    close(client_fd);

    return 0;
}
