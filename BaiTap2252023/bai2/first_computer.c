#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <poll.h>
/*
    Sử dụng hàm select/poll, lập trình ứng dụng chat giữa
    2 máy sử dụng giao thức UDP. Địa chỉ IP, cổng của máy
    nhận và cổng chờ được nhập từ dòng lệnh.
*/
int main(){
    // tạo socket
    int receiver = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    // cấu hình địa chỉ nhan 
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    bind(receiver, (struct sockaddr *)&addr, sizeof(addr));

    int sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    // cấu hình địa chỉ gui
    struct sockaddr_in addr1;
    addr1.sin_family = AF_INET;
    addr1.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr1.sin_port = htons(8000);


    struct pollfd fds[2];
    
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    fds[1].fd = receiver;
    fds[1].events = POLLIN;

    char buf[256];

    while (1)
    {
        int ret = poll(fds, 2, -1);
        if (ret < 0)
        {
            perror("poll() failed");
            break;
        }

        if (ret == 0)
        {
            printf("Timed out.\n");
            continue;
        }

        printf("ret = %d\n", ret);

        if (fds[0].revents & POLLIN)
        {
            fgets(buf, sizeof(buf), stdin);
            //send(sender, buf, strlen(buf), 0);
            int ret = sendto(sender, buf, strlen(buf), 0, 
                        (struct sockaddr *)&addr1,sizeof(addr1));
        }

        if (fds[1].revents & POLLIN)
        {
            // ret = recv(receiver, buf, sizeof(buf), 0);
            int ret = recvfrom(receiver, buf, sizeof(buf), 0,
             NULL, NULL);
            buf[ret] = 0;
            printf("Received: %s\n", buf);
        }
    }

    // Ket thuc, dong socket
    close(receiver);
    close(sender);

    return 0;
}