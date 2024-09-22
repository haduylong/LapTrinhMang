#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
// #include <netdb.h>

int main() {
    // tạo socket chờ kết nối
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listener == -1)
    {
        perror("socket() failed\n");
        return 1;
    }

    // khai báo cấu trúc địa chỉ của server
    struct sockaddr_in addr; // IPv4
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    // gắn socket với cấu trúc địa chỉ
    if(bind(listener, (struct sockaddr *) &addr, sizeof(addr)))
    {
        perror("bind() failed\n");
        return 1;
    }

    // chuyển socket sang trạng thái chờ kết nối
    if(listen(listener, 5))
    {
        perror("listen() failed\n");
        return 1;
    }

    printf("waiting for new client ...\n");

    // chờ và chấp nhận kết nối
    int client = accept(listener, NULL, NULL);
    if(client == -1)
    {
        perror("accept() failed");
        return 1;
    }

    printf("new client connected: %d\n", client);

    // nhận dữ liệu từ client
    char buf[256];
    int ret = recv(client, buf, sizeof(buf), 0);

    if(ret <= 0)
    {
        printf("recv() failed\n");
        return 1;
    }

    // thêm ký tự kết thúc sâu và in ra màn hình
    if(ret < sizeof(buf))
        buf[ret] = 0;
    printf("Data from client: %s\n", buf);

    char *msg = "hello client";
    // gửi dữ liệu sang client
    send(client, msg, strlen(msg), 0);

    // đóng kết nối
    close(client);
    close(listener);

    return 0;
}