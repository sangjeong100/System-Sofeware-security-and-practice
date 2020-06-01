#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/wait.h> 

//동기화하여 좀비 프로세스가 발생하지 않도록 하는 프로그램

int main()
{
    int pid;
    int status;
    printf("Parent process: Begin\n");

    if((pid=fork())==0){
        //fork 함수 사용해서 who로 process 대체
        printf("Child process: Begin\n");
        system("who");
        printf("Child process: End\n");
    }
    else if (pid>0)
    {
        system("date");
        //wait함수를 사용하여 자식프로세스가 끝날때까지 기다림
        //즉, 동기화 처리한다. 동기화됨으로서 고아나 좀비process없음
        if(wait(&status)==-1){
            printf("wait error\n");
            exit(-1);
        }
        
        //자식 프로세스 종료 후 date 호출
        printf("Parent process: End\n");
    }
    //fork error 판단
    else if(pid == -1){
        printf("fork error!\n");
        exit(-1);
    }
}