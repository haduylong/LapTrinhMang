#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
/*
    Ứng dụng udp_file_sender cho phép người dùng nhập
    tên file từ dòng lệnh, sau đó truyền tên và nội dung
    file sang udp_file_receiver. Địa chỉ IP và cổng của
    receiver cũng được nhập từ dòng lệnh

    Ứng dụng udp_file_receiver nhận dữ liệu từ
    udp_file_sender và ghi vào file. Cổng chờ được nhập
    từ dòng lệnh
*/
int main(){
    // tạo socket
    int sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    // cấu hình địa chỉ server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);
    // gửi dữ liệu
    char *filename = "sender_file.txt";

    FILE *f = fopen(filename, "r");

    char buf[1024];
    char tmp[255];
    int pos = 0;
    //gui ten file
    // sendto(sender, filename, strlen(filename), 0, 
    //                     (struct sockaddr *)&addr,sizeof(addr));
    // gui noi dung file
    while (fgets(tmp, sizeof(tmp), f) != NULL)
    {
        memcpy(buf + pos, tmp, strlen(tmp)+1);
        pos = strlen(buf);
    }
    char msg[2048];
    sprintf(msg, "Ten file: %s\nData\n%s", filename, buf);
    int ret = sendto(sender, msg, strlen(msg), 0, 
                        (struct sockaddr *)&addr,sizeof(addr));
    // Dong file vaf dong ket noi
    fclose(f);
    
    close(sender);
    return 0;
}