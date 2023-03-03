#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
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
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 替换为服务器的 IP 地址
    server_addr.sin_port = htons(5477);

    // 连接服务器
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    // 发送 POST 请求
    char *request = "POST /post HTTP/1.1\r\nContent-Length: 13\r\n\r\nHello, world!";
    if (send(client_fd, request, strlen(request), 0) < 0) {
        perror("send failed");
        exit(EXIT_FAILURE);
    }

    // 读取响应
    memset(buffer, 0, BUFFER_SIZE);
    ssize_t read_size = read(client_fd, buffer, BUFFER_SIZE);
    if (read_size < 0) {
        perror("read failed");
        exit(EXIT_FAILURE);
    }

    printf("Server response: %s\n", buffer);

    // 关闭套接字
    close(client_fd);

    return 0;
}