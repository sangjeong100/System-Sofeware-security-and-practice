#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

static void sigquit_handler(int signo);

int main()
{
    
    char str[100];
    struct sigaction sa; //sigaction 함수 선언
    sa.sa_handler = sigquit_handler; //handler 함수 등록
    sigemptyset(&sa.sa_mask);   //mask 비움
    sa.sa_flags = 0;    //flag= 0

    if (sigaction(SIGQUIT, &sa, 0) == -1) // sigaction 함수를 통해 SIGQUIT발생시 handler 실행
    {
        printf ("signal(SIGQUIT) error\n");
        exit(-1);
        //sigaction 실패시
    }    

    printf("Input SIGQUIT: "); 
    while(1)
    {
    fgets(str,sizeof(str),stdin);  //SIGQUIT 핸드러 사용을 기다리는 무한 입력 구간
    }
}

static void sigquit_handler(int signo) //handler 함수
{
    printf("Receive Signal:: Quit\n");
    sleep(2); // 2초간 대기
    printf("In Signal Handler, After sleep\n");
    printf("After Signal Handler\n");
    printf("program end\n");
    printf("아무내용\n");
    exit(1);
}

