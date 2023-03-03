#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
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
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听端口
    if (listen(server_fd, 1) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server started, listening on port 5477...\n");

    while (1) {
        socklen_t client_addr_len = sizeof(client_addr);
        // 接受客户端连接
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        printf("New connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // 读取请求
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t read_size = read(client_fd, buffer, BUFFER_SIZE);
        if (read_size < 0) {
            perror("read failed");
            close(client_fd);
            continue;
        }

        // 判断是否为 POST 请求
// 判断是否为 POST 请求
char *post_path = strstr(buffer, "POST /post");
if (post_path) {
    // 查找请求头结束标记
    char *header_end = memmem(buffer, read_size, "\r\n\r\n", 4);
    if (header_end) {
        // 写入文件
        FILE *file = fopen("a.txt", "w");
        if (!file) {
            perror("open file failed");
            close(client_fd);
            continue;
        }
        fwrite(header_end + 4, 1, read_size - (header_end - buffer) - 4, file);
        fclose(file);

        // 发送响应
        char *response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
        if (send(client_fd, response, strlen(response), 0) < 0) {
            perror("send failed");
            close(client_fd);
            continue;
        }

        printf("POST request handled, data saved to a.txt\n");
    } else {
        // 发送响应
        char *response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        if (send(client_fd, response, strlen(response), 0) < 0) {
            perror("send failed");
            close(client_fd);
            continue;
        }

        printf("Invalid request handled\n");
    }
}

        // 关闭连接
        close(client_fd);
    }

    // 关闭服务器套接字
    close(server_fd);

    return 0;
}