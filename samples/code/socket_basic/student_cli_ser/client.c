#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// Client gửi file sang server
/*
Viết chương trình sv_server, nhận dữ liệu từ
sv_client, in ra màn hình và đồng thời ghi vào file sv_log.

Ngoài dữ liệu được gửi từ client cần phải lưu lại địa chỉ IP
và thời gian client đã gửi. (Tham số cổng) và (tên file log)
được nhập từ tham số dòng lệnh.

Thoát chương trình khi nhập exit.
*/

int main()
{
    // Khai bao socket
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Khai bao dia chi cua server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);

    // Ket noi den server
    int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("connect() failed");
        return 1;
    }

    // Gui tin nhan den server
     char msg[2048];
     while (1)
    {
        printf("Enter massage send to server: ");
        fgets(msg, sizeof(msg), stdin);
        send(client, msg, strlen(msg), 0);
        if (strncmp(msg, "exit", 4) == 0){
           break;
        }
    }

    // Ket thuc, dong socket  
    close(client);

    return 0;
}