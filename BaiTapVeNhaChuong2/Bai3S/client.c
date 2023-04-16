#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
/*
3. Viết chương trình sv_client, cho phép người dùng
nhập dữ liệu là thông tin của sinh viên bao gồm MSSV,
Họ tên, ngày sinh, và điểm trung bình các môn học. Các
thông tin trên được đóng gói và gửi sang sv_server. Yêu
cầu số byte cần gửi là nhỏ nhất có thể. Địa chỉ server
được nhập từ tham số dòng lệnh
*/

typedef struct 
{
    char id[10];
    char hoten[255];
    char ngaySinh[20];
    char diemtb[5];
} Sinhvien;

int len_sv(Sinhvien sv){
    return strlen(sv.id) + strlen(sv.diemtb) + strlen(sv.hoten) + strlen(sv.ngaySinh);
}

int main(){
    // Khai bao socket
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Khai bao dia chi cua server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);
    // Ket noi den server
    int res = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if (res == -1) {
        printf("Khong ket noi duoc den server!");
        return 1;
    }
    // Gui tin nhan den server
    Sinhvien sv;
    printf("Nhap id: ");
    fgets(sv.id, sizeof(sv.id), stdin);

    printf("Nhap ho ten: ");
    fgets(sv.hoten, sizeof(sv.hoten), stdin);

    printf("Nhap ngay sinh: ");
    fgets(sv.ngaySinh, sizeof(sv.ngaySinh), stdin);

    printf("Nhap diem: ");
    fgets(sv.diemtb, sizeof(sv.diemtb), stdin);

    char buf[sizeof(sv)];
    memcpy(buf, &sv, sizeof(sv));
    send(client, buf, sizeof(buf), 0);
    // char buf[len_sv(sv)];
    // memcpy(buf, &sv, len_sv(sv));
    // send(client, buf, sizeof(buf), 0);
    // Dong ket noi
    close(client);
    return 0;
}