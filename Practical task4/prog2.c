#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main()
{
    sigset_t set; //signal set 선언
    sigemptyset(&set); // set 비우기
    sigaddset(&set,SIGINT); // SIGINT를 set에 추가

    sigprocmask(SIG_BLOCK,&set,NULL); //sigprocmask를 사용하여 SIGINT를 BLOCK mode로 mask씌운다. 즉, signal 처리가 안된다.
    //Ctrl + C 안먹게 만듬

    while(1) //아무내용출력
    {
        printf("아무내용\n");
        sleep(1); // 1초간 대기
    }
}
