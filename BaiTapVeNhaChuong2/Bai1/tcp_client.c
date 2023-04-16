#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
/*
1. Viết chương trình tcp_client, kết nối đến một máy
chủ xác định bởi địa chỉ IP và cổng. Sau đó nhận dữ liệu
từ bàn phím và gửi đến server. Tham số được truyền
vào từ dòng lệnh có dạng
tcp_client <địa chỉ IP> <cổng>
*/

/*
• Tạo socket qua hàm socket()
• Điền thông tin về server vào cấu trúc sockaddr_in
• Kết nối tới server qua hàm connect()
• Gửi dữ liệu tới server thông qua hàm send()
• Nhận dữ liệu từ server thông qua hàm recv()
• Đóng socket khi việc truyền nhận kết thúc bằng hàm
close()
*/
int main( int argc, char *argv[]){
    // Khai bao socket
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Khai bao dia chi cua server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));
    // Ket noi den server
    int res = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if (res == -1) {
        printf("Khong ket noi duoc den server!");
        return 1;
    }
    // Gui tin nhan den server
    char msg[256];
    printf("send to server: ");
    fgets(msg, sizeof(msg), stdin);
    send(client, msg, strlen(msg), 0);
    // Nhan tin nhan tu server
    char buf[2048];
    int len = recv(client, buf, sizeof(buf), 0);
    buf[len] = 0;
    printf("Data received from server: %s\n", buf);
    // Ket thuc, dong socket
    close(client);
    return 0;
}