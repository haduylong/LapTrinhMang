#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

/* receiver liên tục nhận dữ liệu cho đến khi nhận được chuỗi
không hợp lệ
*/
int main()
{
    // tạo socket bên nhận
    int receiver = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // khai báo cấu trúc địa chỉ bên nhận
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9090);

    // gắn socket với cấu trúc địa chỉ
    if(bind(receiver, (struct sockaddr *) &addr, sizeof(addr)))
    {
        perror("bind() failed\n");
        return 1;
    }
    printf("Ready for receiving message.\n");
    // nhận và in dữ liệu ra màn hình
    char buf[256];
    while(1)
    {
        int ret = recvfrom(receiver, buf, sizeof(buf), 0, NULL, NULL);
        if(ret == -1)
        {
            perror("recvfrom() failed\n");
            return 1;
        }
        else if (ret == 1) // xau rong
            break;
        else
        {
            buf[ret] = 0;
            printf("Data received %d byte: %s\n", ret, buf);
        }
    }

    // đóng kết nối 
    close(receiver);

    return 0;
}