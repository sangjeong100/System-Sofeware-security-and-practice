#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <setjmp.h>
#include <ncurses.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>

#define KEY_CR_         0x0d        //0x0D(13), CR: 복귀(Carriage Return)
#define KEY_BACKSPACE_  0x7f        //DEL , 제어문자 

#define BUFLEN_MAX      255
#define TRY_MAX         5           //비밀번호 입력가능 횟수

#define INIT_X          (0)
#define INIT_Y          (0)

#define ID_X            (INIT_X + 10)
#define ID_Y            (INIT_Y + 3)

#define PASSWORD_X      (ID_X)
#define PASSWORD_Y      (ID_Y + 1)

#define RESULT_X        (0)
#define RESULT_Y        (PASSWORD_Y + 2)

#define EXPIRE_TIME      30       //passwd 입력 세션 30초로 제한

int g_remain;
int g_timer_param;
jmp_buf env; 
void stop_try();
void sig_jmp();

void sig_handler(int signum)
{
    if(signum != SIGALRM) exit(0);

    if(g_timer_param == EXPIRE_TIME)
    {
        clear();                //화면 지우기
        move(INIT_Y,INIT_X);    //커서 이동
        printw("Timer expired.. (%d seconds)\n", EXPIRE_TIME);  
        refresh();  //buffer 화면 출력          
        sleep(3);
        stop_try();     //30초이내에 로그인 되지 않아서 10분간 정지

        
    }
    else
    {   //잘못된 파라미터 입력
        clear();    //화면 지우기
        move(0,0);  //커서 이동
        printw("illegal parameter...");
        refresh();  //buffer 화면 출력
        getch();
        exit(0);
    }

}

void draw_interface(void)
{
    clear();    //화면 지우기
    move(INIT_Y,INIT_X);    //커서 이동
    printw(  "#####################################");
    printw("\n#     Login program using signal    #");
    printw("\n#####################################");
    printw("\nLogin ID: ");
    printw("\nPassword: ");
    refresh();

}
#define MODE_PASSWORD       1
#define MODE_ID             2
int get_string(int mode, char *buf)
{
    int n,res;
    char ch;

    if(mode != MODE_PASSWORD && mode != MODE_ID) return -1;

    memset(buf,0,BUFLEN_MAX);   //buffer 비우기

    noecho();       //문자 에코 기능 끄기

    mode == MODE_PASSWORD ? move(PASSWORD_Y, PASSWORD_X) : move(ID_Y, ID_X); //password 치는 부분인지, ID 치는 부분인지 체크
    refresh(); //buffer 화면 출력

    n=0;
    while((res = read(0, &ch, 1)))  //한글자씩 체크
    {
        if(res < 0) return -1; //read 실패
    
        if(ch == KEY_CR_) break;    //문자열의 끝 도달
        
        if(ch == KEY_BACKSPACE_)
        {
            if( n > 0)
            {
                n--;
                mode == MODE_PASSWORD ? move(PASSWORD_Y,PASSWORD_X + n) : move(ID_Y, ID_X + n);

                printw(" ");
                mode == MODE_PASSWORD ? move(PASSWORD_Y,PASSWORD_X + n) : move(ID_Y, ID_X + n);
                refresh(); 
            }

        }
        else
        {
            mode == MODE_PASSWORD ? printw("*") : printw("%c", ch);
            refresh();

            buf[n++] = ch;  
            if(n >= BUFLEN_MAX) return -1; //buffer 길이 초과 입력 되면, return    
        }
    }
    buf[n] = '\0'; //마지막에 null값 삽입

    return n;   //입력된 길이 return
}

int main(int argc, char *argv[])
{
    int nTry;
    char buf_login[BUFLEN_MAX], buf_password[BUFLEN_MAX];

    const char *user_login = "sangjeong";
    const char *user_password = "1q2w3e4r!";
    //id, pw 지정
    struct sigaction sa;

    if(setjmp(env) == 2)        //longjmp 지점 저장
    {
        sigset_t set1;
        sigaddset(&set1,SIGALRM);
        sigprocmask(SIG_UNBLOCK, &set1, NULL); //longjmp 후 SIGALAM이 block 되므로 unblock 해준다.
    }

    initscr(); //라이브러리 초기화 ; ncurses 라이브러리 쓰기위해선 반드시 선언

    memset(&sa, 0, sizeof(struct sigaction)); //sigaction 구조체 초기화
    sa.sa_handler = sig_handler;
    sigemptyset(&sa.sa_mask);    //sa signal mask 초기화
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, NULL); //handler 함수 등록
    
    g_remain = EXPIRE_TIME;     
    
    g_timer_param = EXPIRE_TIME; //pw 구간 30초 제한

    nTry = 0;                   //try 수 0으로 초기화

    while(nTry < TRY_MAX)       //로그인 시도 횟수 체크
    {
        alarm(0);                   //알람 초기화
        draw_interface();       //초기화면 띄우기

        if(get_string(MODE_ID, buf_login) <= 0);     //ID 부분 아무것도 입력안됐거나 예기치않은 경우 발생하면 처음부터 다시 입력, try횟수는 증가하지 않음.
        else
        {
            alarm(g_timer_param);       //pw 세션 30초 알람 등록
            if(get_string(MODE_PASSWORD, buf_password) <= 0) nTry++; //password 부분 아무것도 입력안됐거나 예기치않은 경우 발생, try 증가
            else
            {
                if(!strcmp(buf_login, user_login) && !strcmp(buf_password, user_password))
                {
                    alarm(0);
                    break;
                } //id, pw 잘 입력되었으면 알람 종료
                else    //id, pw 잘못 입력되었으면
                {
                    nTry++; //try 증가
                }
                
            }
            
        }
        
    }
    
    clear();
    move(INIT_Y, INIT_X);   //커서 처음으로 이동
    
    if(nTry < TRY_MAX) 
    {
        printw("Correct !!\n*ID - %s\n*Password= %s", buf_login, buf_password);
        refresh();
        getch();
        endwin();
    }
    else
    {

        printw("Fail to login !! (You tried %d times)\n",nTry);
        refresh();
        sleep(3);
        stop_try();     //10분간 대기 시키는 함수


    }
    
    return 0;
}

void stop_try()
{
    alarm(0);

    clear();
    move(INIT_Y, INIT_X);
    printw("You can't try to log in for 10 minutes.\n");
    printw("Please wait for ten minutes.");    
    refresh();
    sleep(600); //로그인 10분간 못하게 막음
    longjmp(env,2);
}