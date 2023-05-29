// 客户端代码
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>

#define SERVER_IP "127.0.0.1" // 服务器的 IP 地址
#define BUFFER_SIZE 1024 // 缓冲区大小

char message[BUFFER_SIZE]; // 发送的消息
char reply[BUFFER_SIZE]; // 接收的回复

static int sock;
static int port;

int sim_lcd_send_screen(void *buffer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t bits){
    int ret;
    struct sockaddr_in server; // 服务器的地址结构体

    // 创建套接字
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("创建套接字失败\n");
        return 1;
    }

    // 设置服务器的地址结构体
    server.sin_addr.s_addr = inet_addr(SERVER_IP); // IP 地址
    server.sin_family = AF_INET; // 协议族
    server.sin_port = htons(port); // 端口号

    // 连接服务器
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("连接服务器失败\n");
        return 1;
    }

    strcpy(message, "sim_lcd_send_screen"); // 消息内容
    ret = send(sock, message, strlen(message), 0);
    assert(ret >=0 );
    
    ret = recv(sock, reply, BUFFER_SIZE, 0);
    assert(ret >= 0);

    int32_t tmp_buffer[5];
    tmp_buffer[0] = x0;
    tmp_buffer[1] = y0;
    tmp_buffer[2] = x1;
    tmp_buffer[3] = y1;
    tmp_buffer[4] = bits;

    ret = send(sock, &tmp_buffer, sizeof(tmp_buffer), 0);
    assert(ret >=0 );

    ret = recv(sock, reply, BUFFER_SIZE, 0);
    assert(ret >= 0);

    ret = send(sock, buffer, (x1-x0+1)*(y1-y0+1)*bits/8, 0);
    assert(ret >=0 );

    ret = recv(sock, reply, BUFFER_SIZE, 0);
    assert(ret >= 0);
    
    // printf("[sim_lcd_client] %s:send success\n", __FUNCTION__);

    // 关闭套接字
    close(sock);

}

bool sim_lcd_touchpad_is_pressed(){

    int ret;
    struct sockaddr_in server; // 服务器的地址结构体

    // 创建套接字
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("创建套接字失败\n");
        return 1;
    }

    // 设置服务器的地址结构体
    server.sin_addr.s_addr = inet_addr(SERVER_IP); // IP 地址
    server.sin_family = AF_INET; // 协议族
    server.sin_port = htons(port); // 端口号

    // 连接服务器
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("连接服务器失败\n");
        return 1;
    }

    strcpy(message, "sim_lcd_touchpad_is_pressed"); // 消息内容
    ret = send(sock, message, strlen(message), 0);
    assert(ret >=0 );
    
    ret = recv(sock, reply, sizeof(int32_t), 0);
    assert(ret >= 0);
    
    close(sock);
    // printf("[sim_lcd_client] %s success\n", __FUNCTION__);

    printf("press:%d\n", ((int32_t*)reply)[0]);
    if (((int32_t*)reply)[0] == 0)
    {
        printf("press:false\n");
        return false;
    }
    else if (((int32_t*)reply)[0] == 1)
    {
        printf("press:true\n");
        return true;
    }
    else
        assert(false);
    
}

void sim_lcd_touchpad_get_xy(int32_t* x, int32_t* y){
    int ret;
    struct sockaddr_in server; 

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("创建套接字失败\n");
        return 1;
    }

    // 设置服务器的地址结构体
    server.sin_addr.s_addr = inet_addr(SERVER_IP); // IP 地址
    server.sin_family = AF_INET; // 协议族
    server.sin_port = htons(port); // 端口号

    // 连接服务器
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("连接服务器失败\n");
        return 1;
    }

    strcpy(message, "sim_lcd_touchpad_get_xy"); // 消息内容
    ret = send(sock, message, strlen(message), 0);
    assert(ret >=0 );
    
    ret = recv(sock, reply, sizeof(int32_t)*2, 0);
    assert(ret >= 0);
    
    close(sock);

    // printf("[sim_lcd_client] %s success\n", __FUNCTION__);

    *x = ((int32_t*)reply)[0];
    *y = ((int32_t*)reply)[1];
    printf("%d %d\n", *x, *y);

}


void sim_lcd_release()
{

}


int sim_lcd_init(int port_)
{
    port = port_;
    // struct sockaddr_in server; // 服务器的地址结构体


    // // 创建套接字
    // sock = socket(AF_INET, SOCK_STREAM, 0);
    // if (sock == -1)
    // {
    //     printf("创建套接字失败\n");
    //     return 1;
    // }
    // printf("套接字创建成功\n");

    // // 设置服务器的地址结构体
    // server.sin_addr.s_addr = inet_addr(SERVER_IP); // IP 地址
    // server.sin_family = AF_INET; // 协议族
    // server.sin_port = htons(port); // 端口号

    // // 连接服务器
    // if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    // {
    //     printf("连接服务器失败\n");
    //     return 1;
    // }
    // printf("连接服务器成功\n");

    // // 发送消息
    // strcpy(message, "hello world"); // 消息内容
    // if (send(sock, message, strlen(message), 0) < 0)
    // {
    //     printf("发送消息失败\n");
    //     return 1;
    // }
    // printf("发送消息成功: %s\n", message);

    // // 接收回复
    // if (recv(sock, reply, BUFFER_SIZE, 0) < 0)
    // {
    //     printf("接收回复失败\n");
    //     return 1;
    // }
    // printf("接收回复成功: %s\n", reply);


    return 0;
}
