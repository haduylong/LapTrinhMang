#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int main(){
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9090); 

    if (connect(client, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("connect() failed");
        return 1;
    }

    // truyền nhận dữ liệu
    char *filename = "vanban.txt";
    FILE *f = fopen(filename, "r");
    char buf[2048];
    char tmp[512];
    int pos = 0;
    // đọc file
    while (fgets(tmp, sizeof(tmp), f) != NULL)
    {
        memcpy(buf + pos, tmp, strlen(tmp)+1);
        pos = strlen(buf);
    }
    printf("%s", buf);
    // gửi đi
    send(client, buf, strlen(buf), 0);
    // Ket thuc, dong socket
    close(client);
    return 0;
}