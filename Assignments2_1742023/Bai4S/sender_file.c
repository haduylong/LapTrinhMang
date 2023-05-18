#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void readFile(FILE *file, char* buf){
    char tmp[255];
    int pos = 0;
    while (fgets(tmp, sizeof(tmp), file) != NULL)
    {
        memcpy(buf + pos, tmp, strlen(tmp)+1);
        pos = strlen(buf);
    }
    // return buf;
}

int main(){
    // tạo socket
    int sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    // cấu hình địa chỉ server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);

    struct sockaddr_in addr1;
    addr1.sin_family = AF_INET;
    addr1.sin_addr.s_addr = inet_addr("127.0.0.2");
    addr1.sin_port = htons(9000);

    struct sockaddr_in addr2;
    addr2.sin_family = AF_INET;
    addr2.sin_addr.s_addr = inet_addr("127.0.0.3");
    addr2.sin_port = htons(9000);
    // gửi dữ liệu
    char *filename = "first_sender.txt";
    char *filename1 = "second_sender.txt";
    char *filename2 = "thid_sender.txt";

    FILE *f = fopen(filename, "r");
    FILE *f1 = fopen(filename1, "r");
    FILE *f2 = fopen(filename2, "r");

    char buf[1024]; readFile(f, buf);
    char buf1[1024]; readFile(f1, buf1);
    char buf2[1024]; readFile(f2, buf2);

    int ret = sendto(sender, buf, strlen(buf)+1, 0, 
                        (struct sockaddr *)&addr,sizeof(addr));

    int ret1 = sendto(sender, buf1, strlen(buf1)+1, 0, 
                        (struct sockaddr *)&addr1,sizeof(addr1));

    int ret2 = sendto(sender, buf2, strlen(buf2)+1, 0, 
                        (struct sockaddr *)&addr2,sizeof(addr2));                    
    /*
    char buf[1024];
    char tmp[255];
    int pos = 0;
    while (fgets(tmp, sizeof(tmp), f) != NULL)
    {
        memcpy(buf + pos, tmp, strlen(tmp)+1);
        pos = strlen(buf);
    }
    char msg[2048];
    sprintf(msg, "Ten file: %s\nData\n%s", filename1, buf);
    int ret = sendto(sender, msg, strlen(msg), 0, 
                        (struct sockaddr *)&addr,sizeof(addr));
    */
    
    // Dong file vaf dong ket noi
    fclose(f);
    fclose(f1);
    fclose(f2);

    close(sender);
    return 0;
}