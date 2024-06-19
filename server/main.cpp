#include <iostream>
#include <cstdio>
#include <cstring>
#include "sys/socket.h"
#include "arpa/inet.h"
#include <unistd.h>


int main(){
    std::cout << "Hi from server" << std::endl;

    // 创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    // 配置IP和端口信息
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) == 0){
        perror("ERROR");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // bind绑定IP和端口
    if(bind(sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("ERROR");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // listen监听一个连接
    if(listen(sockfd, 1) < 0){
        perror("ERROR");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // accept接收
    sockaddr_in client;
    socklen_t len = sizeof(client);
    int accept_fd = accept(sockfd, (sockaddr*)&client, &len);
    if(accept_fd < 0){
        perror("ERROR");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    std::cout << "connect successed!" << std::endl;
    std::cout << "client ip: " << inet_ntoa(client.sin_addr) << ", client port: " << ntohs(client.sin_port) << std::endl;
    char write_buf[1024];
    char read_buf[1024];
    while(1){
        // 读取消息
        memset(read_buf, 0, sizeof(read_buf));
        ssize_t bytes_recv = read(accept_fd, read_buf, sizeof(read_buf) - 1);
        if(bytes_recv > 0){
            std::cout << "Client: " << read_buf;
        }else if(bytes_recv == 0){
            std::cout << "Client closed connect, server finished!" << std::endl;
            break;
        }else{
            perror("ERROR");
            break;
        }

        // 写入消息
        memset(write_buf, 0, sizeof(write_buf));
        printf("You: ");
        fgets(write_buf, sizeof(write_buf), stdin);
        ssize_t bytes_sent = write(accept_fd, write_buf, sizeof(write_buf) - 1);
        if(bytes_sent == -1){
            perror("ERROR");
            break;
        }

    }

    close(accept_fd);
    close(sockfd);

    return 0;
}