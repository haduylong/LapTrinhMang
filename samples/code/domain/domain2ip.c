#include <stdio.h>
#include <netdb.h>      // phân giải tên miền
#include <string.h>
#include <arpa/inet.h>  // các hàm chuyển đổi địa chỉ

// phân giải tên miền thành địa chỉ ip
// tham số nhập vào từ dòng lệnh là tên miền cần phân giải

int main(int argc, char *argv[]) {

    // kiểm tra tham số nhập vào
    if(argc != 2) {
        printf("Tham so khong hop le.\n");
        return 1;
    }

    // khai báo con trỏ kết quả
    struct addrinfo *res, *p;
    int ret = getaddrinfo(argv[1], "http", NULL, &res);
    if(ret!=0 || res==NULL) 
    {
        printf("Failed to get IP: %s\n", gai_strerror(ret));
        return 1;
    }

    // duyệt danh sách kết quả và in ra địa chỉ ip
    p = res;
    while(p != NULL) {
        if(p->ai_family == AF_INET)
        {
            printf("IPv4\n");
            struct sockaddr_in addr;
            memcpy(&addr, p->ai_addr, p->ai_addrlen);
            printf("IP: %s\n", inet_ntoa(addr.sin_addr));
        }else if(p->ai_family == AF_INET6)
        {
            printf("IPv6\n");
            char buf[64];
            struct sockaddr_in6 addr6;
            memcpy(&addr6, p->ai_addr, p->ai_addrlen);
            printf("IP: %s\n", inet_ntop(p->ai_family, &addr6.sin6_addr, buf, sizeof(buf)));
        }
        p = p->ai_next;
    }

    // giải phóng con trỏ
    freeaddrinfo(res);

    return 0;
}