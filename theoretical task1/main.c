#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <sys/wait.h>

int main()
{
    int fd_A;    //file A
    int fd_B;    //file B
    int pid;

    if((fd_A = open("./A",O_RDONLY)) == -1) 
    { 
        printf("open error!\n");
        exit(-1);
    }  //입력 값이 될 A파일 read only로 open

    if((fd_B = open("./B", O_WRONLY | O_CREAT | O_TRUNC,0777)) == -1) 
    {
        printf("open error!\n");
        exit(-1);
    } //출력 값이 저장될 B파일을 open 
    
    //dup2 함수 사용 
    if(dup2(fd_A,0) == -1)  
    {
        printf("dup error!!\n");
        exit(-1);
    }
    //fd_A를 fd 0(stdin)에 복사하여
    //표준 입력이 키보드와의 연결 close 후 file A와 연결된다.  

    if(dup2(fd_B,1) == -1)
    {
        printf("dup error!!\n");
        exit(-1);
    } 
    //fd_B를 fd 1(stdout)에 복사하여
    //표준 출력이 monitor와의 연결 close 후 file B와 연결된다.

    if((pid=fork())==0)//child
    {
        execl("./P",NULL,NULL);
    }
    else if(pid==-1)
    {
        printf("fork error!\n");
        exit(-1);
    }
    else{

        if(wait(0)==-1){
        printf("wait error!\n");
        exit(-1);
        }
    }

    
}
