#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <poll.h>

// void chuanHoaXau(char *a){
//     int n = strlen(a);
//     for(int i=0;i<n;i++){ //Duyệt các ký tự từ đầu xâu tới cuối xâu
//     if(a[i]==' '){//Kiểm tra xem nếu ký tự là khoảng trắng thì thực hiện khối lệnh
//         for(int j=i;j<n-1;j++){//Duyệt j từ vị trí thứ i
//             a[j] = a[j+1]; //Gắn phần tử j bằng phần tử kế tiếp nó
//         }
//          //=>Sau khi thực hiện vòng lặp j, xâu bị đẩy về phía trước 1 đơn vị và phần thứ i sẽ bị mất
//         a[n-1]=NULL; //gắn phần tử cuối cùng của xâu là NULL
//         i--; // Vì xâu đã bị đẩy lùi về phía trước, thực hiện kiểm tra lại ký tự đó
//         n--; //Vì đã xóa khoảng trắng xâu sẽ ngắn đi 1 ký tự
//     }
//     else break;//Nếu gặp ký tự không phải khoảng trắng thì thoát vòng lặp ngay lập tức
// }

// for(int i=n-1;i>=0;i--){//Duyệt các ký tự từ cuối xâu về đầu xâu
//     if(a[i]==' '){//kiểm tra ký tự thứ i có phải là khoảng trắng
//         a[i]=NULL; //Nếu là khoảng trắng chỉ cần gắn phần tử cuối xâu này là null
//         n--; //Lúc này xâu cũng sẽ bị ngắn đi 1 ký tự
//     }
//     else break;//nếu gặp ký tự không phải là khoảng trắng ngày lập tức dừng vòng lặp
// }

// //Khối lệnh chạy tương tự như xóa khoảng trắng ở đầu xâu, tuy nhiên sẽ không có lệnh dừng vòng lặp nếu gặp ký tự khác ký tự khoảng trắng    
// for(int i=1;i<n-1;i++) //Vì ký tự khoảng trắng ở đầu và cuối xâu đã bị xóa nên i chỉ cần chạy từ vị trí thứ 1 tới n-2
//     {
//         if(a[i]==a[i+1]){
//             for(int j=i;j<n-1;j++){
//                 a[j] = a[j+1];
//             }
//             a[n-1]=NULL;
//             i--;
//             n--;
//         }
//     }

//     for(int i=0;i<n-1;i++)
//     {
//         if(a[i] ==' ' && a[i+1]!=' ') 
//             if(a[i+1] >= 'a' && a[i+1] <='z') 
//                 a[i+1] = a[i+1]-32;  //in hoa chữ cái đầu tiên mỗi từ
//     }
// }

//Hàm biến đổi ký tự thường thành ký tự hoa
char toupper(char charx){
    if(charx >= 'a' && charx <='z') charx = charx-32;
    return charx;
}
 
 
//Hàm chuẩn hóa xâu
void chuanHoa(char a[])
{
    int n = strlen(a); //Lấy độ dài xâu
 
//Xóa khoảng trắng đầu xâu
    for(int i=0;i<n;i++){
        if(a[i]==' '){
            for(int j=i;j<n-1;j++){
                a[j] = a[j+1];
            }
            a[n-1]=NULL;
            i--;
            n--;
        }
        else break;
    }
     
 
//Xóa khoảng trắng cuối xâu
    for(int i=n-1;i>=0;i--){
        if(a[i]==' '){
            a[i]=NULL;
            n--;
        }
        else break;
    }
     
    //Xóa khoảng trắng không hợp lệ(khoảng trắng bị thừa)
    for(int i=1;i<n-1;i++)
    {
        if(a[i]==a[i+1]){
            for(int j=i;j<n-1;j++){
                a[j] = a[j+1];
            }
            a[n-1]=NULL;
            i--;
            n--;
        }
    }
     
//In hoa ký tự đầu tiên
//    a[0] =  toupper(a[0]); 
for(int i=0;i<n-1;i++)
{
  if(a[i] ==' ' && a[i+1]!=' ') a[i+1] =  toupper(a[i+1]);  //in hoa chữ cái đầu tiên mỗi từ
} 
}

int main() 
{
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

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

    struct pollfd fds[64];
    int nfds = 1;

    fds[0].fd = listener;
    fds[0].events = POLLIN;

    char buf[256];
    int users[64];      // Mang socket client da dang nhap
    char *user_ids[64]; // Mang id client da dang nhap
    int num_users = 0;  // So luong client da dang nhap

    while (1)
    {
        int ret = poll(fds, nfds, -1);
        if (ret < 0)
        {
            perror("poll() failed");
            break;
        }

        if (fds[0].revents & POLLIN)
        {
            int client = accept(listener, NULL, NULL);
            if (nfds == 64)
            {
                // Tu choi ket noi
                close(client);
            }
            else
            {
                fds[nfds].fd = client;
                fds[nfds].events = POLLIN;
                nfds++;
                char sendbuf[512];
                sprintf(sendbuf, "xin chao. hien co %d client dang ket noi", nfds);
                send(client, sendbuf, strlen(sendbuf), 0);
               
                printf("New client connected: %d\n", client);
            }
        }

        for (int i = 1; i < nfds; i++)
            if (fds[i].revents & POLLIN)
            {
                ret = recv(fds[i].fd, buf, sizeof(buf), 0);
                if (ret <= 0)
                {
                    close(fds[i].fd);
                    // Xoa khoi mang
                    if (i < nfds - 1)
                        fds[i] = fds[nfds - 1];
                    nfds--;
                    i--;
                }else if(strncmp(buf, "exit", 4)){
                    close(fds[i].fd);
                    // Xoa khoi mang
                    if (i < nfds - 1)
                        fds[i] = fds[nfds - 1];
                    nfds--;
                    i--;
                }else{
                    // gui sau chuan hoa
                    chuanHoa(buf);
                    // Forward du lieu cho user
                    char sendbuf[1024];
                        for (int k = 0; k < num_users; k++)
                            if (users[k] == fds[i].fd)
                                send(users[k], buf, strlen(buf), 0);
                }

            } 
    } 

    close(listener);
    return 0;
}
