#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <unistd.h> 

//process 동기화 하지 않은 코드
//wait함수를 사용하지 않아서 부모,자식 누가 먼저 죽을지 알 수 없다.

int main()
{
    int pid;
    printf("Parent process: Begin\n");
    if((pid=fork())==0){
        //fork후 자식프로세스면 who
        printf("Child process: Begin\n");
        system("who");
        printf("Child process: End\n");
        
        
    }
    else if(pid>0){
        //부모 프로세스면 date
        
        system("date");
        
        printf("Parent process: End\n");
    }
    else if(pid==-1)
    {
        //fork error 판단
        printf("fork error!\n");
        exit(-1);
    }

}