#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){
    // tạo socket
    int receiver = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    // cấu hình địa chỉ server 
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    bind(receiver, (struct sockaddr *)&addr, sizeof(addr));

    struct sockaddr_in sender_addr;
    int sender_addr_len = sizeof(sender_addr);
    //
    struct sockaddr_in sender_addr1;
    int sender_addr_len1 = sizeof(sender_addr1);
    struct sockaddr_in sender_addr2;
    int sender_addr_len2 = sizeof(sender_addr2);
    struct sockaddr_in sender_addr3;
    int sender_addr_len3 = sizeof(sender_addr3);
    // nhận dữ liệu
    // file ghi cua server
    char buf[2048];
    FILE *f = fopen("recv_file.txt", "a");

    int ret = recvfrom(receiver, buf, sizeof(buf), 0,
             (struct sockaddr *)&sender_addr, &sender_addr_len);

    
    int store = fprintf(f,"%s", buf);
    printf("So byte da ghi vao file: %d\n", store);

    //Dong file
    fclose(f);
    // dong ket noi
    close(receiver);
    return 0;
}