#include <iostream>
#include "arpa/inet.h"
#include "sys/socket.h"
#include <unistd.h>
#include <string>
#include <string.h>
#include <functional>


int main(){
    std::cout << "Hi from client!" << std::endl;

    // 创建socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd == -1){
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    // 配置服务器地址
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0){
        perror("ERROR");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    // connect()连接
    if(connect(socket_fd, (sockaddr*)&server_addr, sizeof(server_addr)) != 0){
        perror("ERROR");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    // 连接成功
    std::cout << "connect successed!" << std::endl;

    // write()发送数据
    char write_buf[1024];
    char read_buf[1024];

    while(1){
        // 向服务端发送消息
        memset(write_buf, 0, sizeof(write_buf));
        std::cout << "You: "; 
        fgets(write_buf, sizeof(write_buf), stdin);
        ssize_t bytes_sent = write(socket_fd, write_buf, strlen(write_buf));
        if(bytes_sent == -1){
            perror("ERROR");
            close(socket_fd);
            exit(EXIT_FAILURE);
        }

        // 读取服务器消息
        memset(read_buf, 0, sizeof(read_buf));
        ssize_t bytes_recv = read(socket_fd, read_buf, sizeof(read_buf) - 1);
        if(bytes_recv > 0){
            std::cout << "Server: " << read_buf;
        }else if(bytes_recv == 0){
            std::cout << "Server closed the connection." << std::endl;
        }else{
            perror("ERROR");
            close(socket_fd);
            exit(EXIT_FAILURE);
        }
    }

    // 关闭套接字
    close(socket_fd);

    return 0;
}