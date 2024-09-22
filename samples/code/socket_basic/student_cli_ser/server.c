#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

// Server nhan file tu client
/*
Viết chương trình sv_server, nhận dữ liệu từ
sv_client, in ra màn hình và đồng thời ghi vào file sv_log.

Ngoài dữ liệu được gửi từ client cần phải lưu lại địa chỉ IP
và thời gian client đã gửi. (Tham số cổng ) và (tên file log)
được nhập từ tham số dòng lệnh.

Thoát chương trình khi nhập exit.
*/

char* getStringTime(char *timerecv , struct tm date){
    sprintf(timerecv, "%d/%02d/%02d  %02d:%02d:%02d", (date.tm_year + 1900), (date.tm_mon + 1), 
    (date.tm_mday), (date.tm_hour), (date.tm_min), (date.tm_sec));
    return timerecv;
}

int main(int argc, char *argv[])
{
    // Tao socket
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    // Khai bao cau truc dia chi server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    // Gan dia chi voi socket
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
    printf("Waiting for a new client ...\n\n");

    // khai bao cau truc client
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(addr);

    // Chap nhan ket noi va lay ra dia chi client , port
    int client = accept(listener, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (client == -1){
        printf("accept() failed.\n");
        exit(1);
    }
    printf("New client connected: %d\n", client);
    printf("Client IP: %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
    printf("-----------------------------------------------\n");
    char *clientIp = inet_ntoa(clientAddr.sin_addr);

    //lưu nội dung client gửi đến
    char msg[1024];
    char *filename = argv[2];
    FILE *f = fopen(filename, "a");

    while (1)
    {
       int ret = recv(client, msg, sizeof(msg), 0);
        if (ret <= 0 || strncmp(msg, "exit", 4) == 0)
        {
            printf("Ket noi bi dong.\n");
            break;    
        }
        if (ret < sizeof(msg)){
            msg[ret] = '\0';
        }
        // thoi gian gui
        time_t t = time(NULL);
        struct tm date = *localtime(&t);
        char timerecv[50];
        // ket qua
        char buf[2048];
        sprintf(buf,"Mesage: %sAddress: %s Time: %s\n", msg, clientIp, getStringTime(timerecv,date));
        printf("%s\n", buf);
        printf("%d byte da nhan\n", ret);
        int store = fprintf(f,"%s", buf);
        printf("So byte da ghi vao file: %d\n", store);
        printf("-----------------------------------------------\n");
    }

    // Dong file
    fclose(f);

    // Dong ket noi
    close(client);
    close(listener);

    return 0;
}
