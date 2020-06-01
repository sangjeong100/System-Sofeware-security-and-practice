#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char **argv)
{
    int socketfd;
    struct sockaddr_in server_addr, my_addr;

    char buf[256];
    
    //socket 생성
    if((socketfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr,"socket error!\n");
        exit(-1);
    }

    //socket addr 구조체에 server 정보 등록
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("10.178.0.3");
    server_addr.sin_port = htons(4000);

    //connection 요청
    if ((connect(socketfd, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0) { 
        printf("bind: connection refused\n"); 
        exit(-1); 
    } 
    read(socketfd,buf, sizeof(buf));
    printf("Get From Server - My Host Name : %s\n",buf);
    memset(buf, 0 , sizeof(buf));
    sleep(1);
    read(socketfd,buf, sizeof(buf));
    printf("Get From Server - My Ip Address : %s\n",buf);
    memset(buf, 0, sizeof(buf));
    sleep(1);
    read(socketfd,buf, sizeof(buf));
    printf("Get From Server - My Port Number : %s\n",buf);

    close(socketfd);

    return 0;

}
