#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#define BUF_SIZE 256

typedef struct client {
    int fd;
    char ip[20];
    char port[20];
    char hostname[50];
}client_st;

int main(int argc, char** argv)
{
    int server_socketfd, client_socketfd;
    int client_addr_size = 0;
    struct sockaddr_in client_addr, server_addr;
    char buf[BUF_SIZE] = { 0, };
    client_st client_data = { 0 };

    if ((server_socketfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "socket error!!\n");
        exit(-1);
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;                //IPv4 Internet protocol
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //32bit IPV4 address
    server_addr.sin_port = htons(4000);              //I use 4000 port num

    //Associate a hosts's local address with a socket
    if (bind(server_socketfd, (struct sockaddr*) & server_addr, sizeof(server_addr)) == -1)
    {
        fprintf(stderr, "bind error!\n");
        exit(-1);
    }

    //Wait for a connection request coming through the socket
    if (listen(server_socketfd, 5) == -1) {
        fprintf(stderr, "listen error!\n");
        exit(-1);
    }

    //Save the communication type size of client 
    client_addr_size = sizeof(client_addr);
    if ((client_socketfd = accept(server_socketfd, (struct sockaddr*) & client_addr, &client_addr_size)) < 0)
    {
        fprintf(stderr, "accept error!\n");
    }
    client_data.fd = client_socketfd;
    
    //ip, port save
    strcpy(client_data.ip, inet_ntoa(client_addr.sin_addr));
    sprintf(client_data.port, "%d", ntohs(client_addr.sin_port));
    
    //client's hostname
    struct hostent* client_host;
    
    client_host = gethostbyaddr((char*)&client_addr.sin_addr, sizeof(client_addr), AF_INET);
    sprintf(client_data.hostname, "%s", client_host->h_name);
    
    printf("Client Info - Client Name : %s\n", client_data.hostname);
    write(client_socketfd, client_data.hostname, strlen(client_data.hostname));
    sleep(1);
    printf("Client Info - Client IP : %s\n", client_data.ip);
    write(client_socketfd, client_data.ip, strlen(client_data.ip));
    sleep(1);
    printf("Client Info - Client Port : %s\n", client_data.port);
    write(client_socketfd, client_data.port, strlen(client_data.port));
 
    close(client_socketfd);
    close(server_socketfd);
    return 0;
}