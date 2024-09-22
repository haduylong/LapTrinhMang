#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>

/*
____________________(main)____________________
khởi tạo server nhận một chuỗi ký tự từ client
sau đó gửi lại client một chuỗi ký tự 

____________________(optional)____________________
phân giải tên miền thành địa chỉ ip, gửi lệnh GET
và nhận về thông báo
*/ 

int main(){
    // khai báo socket kết nối đến server
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(client == -1)
    {
        perror("socket() failed\n");
        return 0;    
    }

    // phân giải tên miền server (optinal)
    struct addrinfo *domain;
    int ret = getaddrinfo("httpbin.org", "http", NULL, &domain);
    if(ret!=0 || domain==NULL) 
    {
        printf("Failed to get IP: %s\n", gai_strerror(ret));
        return 1;
    }

    // // khai báo địa chỉ server (main)
    // struct sockaddr_in addr;
    // addr.sin_family = AF_INET;
    // addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // addr.sin_port = htons(8080);

    // kết nối đến server
    // int res = connect(client, (struct sockaddr *) &addr, sizeof(addr));  // (main)
    int res = connect(client, domain->ai_addr , domain->ai_addrlen);  // (optional)  
    if(res == -1)
    {
        perror("connect() failed\n");
        return 1;
    }

    // gửi tin nhắn đến server
    // char *msg = "hello server"; // (main)
    char *msg = "GET /get HTTP/1.1\r\nHost: httpbin.org\r\nConnection: close\r\n\r\n"; // (optional)
    send(client, msg, strlen(msg), 0);

    // nhận tin nhắn từ server
    char buf[4096];
    int len = recv(client, buf, sizeof(buf), 0);
    if(len <= 0)
    {
        printf("recv() fail\n");
        return 0;
    }
    
    // thêm ký tự kết thúc và in ra màn hình
    if(len < sizeof(buf))
        buf[len] = 0;
    printf("Data from server: %s\n", buf);

    // đóng kết nối
    close(client);

    return 0;
}