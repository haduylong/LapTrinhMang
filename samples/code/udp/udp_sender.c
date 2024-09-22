#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main()
{
    // tạo socket bên gửi
    int sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // khai báo cấu trúc địa chỉ bên nhận
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9090);

    // gửi tin nhắn
    char buf[256];
    while(1)
    {
        printf("Send message: ");
        fgets(buf, sizeof(buf), stdin);
        sendto(sender, buf, strlen(buf), 0, (struct sockaddr*) &addr, sizeof(addr));
        
        if(strlen(buf) == 1)
            break;
    }

    // đóng kết nối
    close(sender);

    return 0;
}