#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

int main(){
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9090);

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr))) 
    {
        perror("bind() failed");
        return 1;
    }

    if (listen(listener, 5)) 
    {
        perror("listen() failed");
        return 1;
    }
    printf("Waiting for a new client ...\n");

    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    int client = accept(listener, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (client == -1)
    {
        perror("accept() failed");
        return 1;
    }
    printf("New client connected: %d\n", client);
    //nhận dữ liệu
    char buf[2048];
    int ret = recv(client, buf, sizeof(buf), 0);
    buf[ret] = 0;
    // xử lý
    char *p;
    int count = 0;
    p = strstr(buf, "0123456789");
    while (p != NULL)
    {
        count++;
        // printf("count: %d", count);
        p = strstr(p+10, "0123456789");
    }
    printf("So lan tim thay chuoi 123456789 la: %d\n", count);
    // Đóng kết nối
    close(client);
    close(listener);    
    return 0;
}