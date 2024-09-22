#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

void *client_thread(void *);
void *join(void *);
void *msg(void *);
void *pmsg(void *);
void *op(void *);
void *kick(void *);
void *topic(void *);
void *quit(void *);

int users[100];
char* user_names[100];
int curr_user = -1;
pthread_t thread_ids[100];

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

    while (1)
    {
        int client = accept(listener, NULL, NULL);
        if (client == -1)
        {
            perror("accept() failed");
            continue;;
        }
        printf("New client connected: %d\n", client);

        pthread_t thread_id;
        // pthread_create(&thread_id, NULL, client_thread, &client);
        pthread_create(&thread_id, NULL, client_thread , &client);
        pthread_detach(thread_id);
    }
    
    close(listener);    

    return 0;
}

void *client_thread(void *param)
{
    int client = *(int *)param;
    char buf[256];
    
    while (1)
    {
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        
        buf[ret] = 0;
        printf("Received from %d: %s\n", client, buf);

        char cmd[32], id[32], tmp[32];
        ret = sscanf(buf, "%s%s%s", cmd, id, tmp);

        if(strncmp(cmd, "JOIN", 4)) // JOIN
        {
            curr_user+=1;
            users[curr_user] = client;
            user_names[curr_user] = id;
            // memcpy(user_names[curr_user], buf, sizeof(buf));

            char * msg = "100 OK\n";
            send(client, msg, strlen(msg), 0);

            for(int i=0; i<curr_user; i++)
            {
                char *msg;
                sprintf(msg, "%s joined", user_names[i]);
                // send(users[i], user_names[i], sizeof(user_names[i]), 0);
                send(users[i], msg, strlen(msg), 0);
            }
        }
        else if(strncmp(cmd, "MSG", 3)) // MSG
        {
            
        }
    }

    close(client);
}

void *join(void *param)
{
    int client = *(int *)param;
    char buf[256];

    int ret = recv(client, buf, sizeof(buf), 0);     
    buf[ret] = 0;

    curr_user+=1;
    users[curr_user] = client;
    user_names[curr_user] = buf;
    // memcpy(user_names[curr_user], buf, sizeof(buf));

    char * msg = "100 OK\n";
    send(client, msg, strlen(msg), 0);

    for(int i=0; i<curr_user; i++)
    {
        char *msg;
        sprintf(msg, "%s joined", user_names[i]);
        // send(users[i], user_names[i], sizeof(user_names[i]), 0);
        send(users[i], msg, strlen(msg), 0);
    }
}

void *msg(void *param)
{
    int client = *(int *)param;
    char buf[256];

    int ret = recv(client, buf, sizeof(buf), 0);     
    buf[ret] = 0;

    for(int i=0; i<curr_user; i++)
    {
        char *msg;
        sprintf(msg, "%s", buf);
        // send(users[i], user_names[i], sizeof(user_names[i]), 0);
        if(users[i]!=client)
            send(users[i], msg, strlen(msg), 0);
    }
}

void *pmsg(void *param)
{
    int client = *(int *)param;
    char buf[256];

    int ret = recv(client, buf, sizeof(buf), 0);     
    buf[ret] = 0;

    for(int i=0; i<curr_user; i++)
    {
        char *msg;
        sprintf(msg, "%s", buf);
        // send(users[i], user_names[i], sizeof(user_names[i]), 0);
        if(users[i]==client)
            send(users[i], msg, strlen(msg), 0);
    } 
}