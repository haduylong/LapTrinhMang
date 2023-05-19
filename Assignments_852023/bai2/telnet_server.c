#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <poll.h>

int checkUserPass(char *s, FILE *f){
    char *msg;
    while (fgets(msg, strlen(s)+1, f) != NULL)
    {
        if(strncmp(s, msg, strlen(s))==0) return 1;
    }
    return 0;
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

    FILE *f; // file co so du lieu
    f = fopen("pass.txt", "r");

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

                printf("New client connected: %d\n", client);
                char *msg = "Nhap user pass: \n";
                send(client, msg, strlen(msg), 0);
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
                }
                else
                {
                    buf[ret] = 0;
                    printf("Received from %d: %s\n", fds[i].fd, buf);
                
                    int client = fds[i].fd;
                    // Kiem tra trang thai dang nhap
                    int j = 0;
                    for (; j < num_users; j++)
                        if (users[j] == client)
                            break;

                    if (j == num_users) // Chua dang nhap
                    {
                        // Xu ly cu phap lenh dang nhap
                        char cmd[32], id[32], tmp[32];
                        ret = sscanf(buf, "%s%s%s", cmd, id, tmp);
                        
                        if (ret == 2)
                        {
                            if (checkUserPass(buf, f) == 1)
                            {
                                char *msg = "Dung user and pass. Hay nhap lenh de chuyen tiep.\n";
                                send(client, msg, strlen(msg), 0);

                                // Luu vao mang user
                                users[num_users] = client;
                                user_ids[num_users] = malloc(strlen(id) + 1);
                                strcpy(user_ids[num_users], id);
                                num_users++;
                            }
                            else
                            {
                                char *msg = "Sai user or pass. Hay nhap lai.\n";
                                send(client, msg, strlen(msg), 0);
                            }
                        }
                        else
                        {
                            char *msg = "Sai tham so. Hay nhap lai.\n";
                            send(client, msg, strlen(msg), 0);
                        }
                    }
                    else // Da dang nhap
                    {
                        // id: user_ids[j]
                        // data: buf
                        char sendbuf[256];

                        strcat(sendbuf, buf);
                        strcat(sendbuf, " > out.txt");
                        system(sendbuf);
                        // Forward du lieu cho user
                        send(client, sendbuf, strlen(sendbuf), 0);
                    }
                }
            }
    }
    
    close(listener);    
    fclose(f);
    return 0;
}