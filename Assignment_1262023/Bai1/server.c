#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <dirent.h>

/*
    Sử dụng kỹ thuật đa tiến trình, lập trình ứng dụng file
    server đơn giản như sau:
    • Khi client mới được chấp nhận, server sẽ gửi danh sách các
    file cho client. Các file này trong thư mục được thiết lập trên
    server. Dòng đầu danh sách là chuỗi “OK N\r\n” nếu có N file
    trong thư mục. Các tên file phân cách nhau bởi ký tự \r\n, kết
    thúc danh sách bởi ký tự \r\n\r\n.
    • Nếu không có file nào trong thư mục thì server gửi chuỗi
    “ERROR No files to download \r\n” rồi đóng kết nối.
    • Client gửi tên file để tải về, server kiểm tra nếu file tồn tại thì
    gửi chuỗi “OK N\r\n” trong đó N là kích thước file, sau đó là
    nội dung file cho client và đóng kết nối, nếu file không tồn tại
    thì gửi thông báo lỗi và yêu cầu client gửi lại tên file.
*/

int checkUserPass(char *s, FILE *f){
    char *msg;
    while (fgets(msg, strlen(s)+1, f) != NULL)
    {
        if(strncmp(s, msg, strlen(s))==0) return 1;
    }
    return 0;
}

void signalHandler(int signo)
{
    int pid = wait(NULL);
    printf("Child %d terminated.\n", pid);
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

    signal(SIGCHLD, signalHandler);

    while (1)
    {
        printf("Waiting for new client...\n");
        int client = accept(listener, NULL, NULL);
        // gửi cho clinet ds flie khi kết nối
        DIR *dir;
        struct dirent *entry;

        dir = opendir(".");
        if (dir == NULL) {
            perror("opendir");
        }
        int count = 0;
        char buf_file_list[4960] = "OK \r\n";
        while ((entry = readdir(dir)) != NULL) 
        {
            if(entry->d_type == DT_REG)
            {
                strcat(buf_file_list, entry->d_name);
                strcat(buf_file_list, "\r\n");
                count++;
            }
        }

        // closedir(dir);
        if(count>0){
            char *msg;
            sprintf(msg, "%s %d\r\n", "OK", count);
            send(client, msg, strlen(msg), 0);
            send(client, buf_file_list, strlen(buf_file_list), 0);
        }else{
            char *msg = "ERROR No files to download \r\n";
            send(client, msg, strlen(msg), 0);
            close(client);
        }
       
        if (fork() == 0)
        {
            // Tien trinh con
            close(listener);
            printf("child %d", getpid());
            // Xu ly ket noi tu client

            char buf[4096];
            while (1)
            {
                // yêu cầu nhâp ten file
                char *msg = "Nhap ten file";
                send(client, msg, strlen(msg), 0);
                // nhận tên flie và xử lý 
                int ret = recv(client, buf, sizeof(buf), 0);
                if (ret <= 0)
                    break;
                buf[ret] = 0;
                printf("Received from %d: %s\n", client, buf);
                //gửi về file
                while ((entry = readdir(dir)) != NULL) 
                {
                    if(entry->d_type == DT_REG)
                    {
                        if(strncmp(buf, entry->d_name, strlen(entry->d_name)))
                        {
                            FILE *f = fopen(entry->d_name, "rb");
                            fseek(f, 0L, SEEK_END);
                            int size = ftell(entry);
                            fseek(f, 0, SEEK_SET);
                            char *msg;
                            sprintf(msg, "%s %d\r\n", "OK", size);
                            char sendbuf[4096];
                            while (1)
                            {
                                ret = fread(buf, 1, sizeof(buf), f);
                                if (ret <= 0)
                                    break;
                                send(client, buf, ret, 0);
                            }

                            fclose(f);
                            break;  
                        }
                    }
                }
            }

            close(client);
            exit(0);
        }

        // Tien trinh cha
        close(client);
    }
    
    close(listener);    

    return 0;
}