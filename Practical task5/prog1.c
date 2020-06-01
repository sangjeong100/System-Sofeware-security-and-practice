
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int count =0;                   //count
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void * function2(void *);
void * function1(void *);

int main()
{
    pthread_t tid1, tid2;
    void *tret;
    int err;

    err = pthread_create(&tid1, NULL, function1, NULL);
    if (err != 0)    
    {
        fprintf(stderr,"can't create thread 1\n");
        exit(-1);
    }
    sleep(1); //첫번째 쓰레드가 먼저 수행 하도록 보장

    err = pthread_create(&tid2, NULL, function2, NULL);
    if (err != 0)
    {
            fprintf(stderr, "can't create thread 2\n");
            exit(-1);
    }
    
    //pthread_join 통해 thread 종료를 기다린다.
    err = pthread_join(tid1,&tret); 
    if (err != 0)  
    {
        fprintf(stderr, "can't join with thread 1\n");
        exit(-1);
    }
    err = pthread_join(tid2, &tret);
    if(err != 0)
    {
        fprintf(stderr, "can't join with thread 2");
        exit(-1);
    }

    pthread_cond_destroy(&cond);        //조건변수 파괴
    pthread_mutex_destroy(&mutex);      //mutex파괴
    

    return 0;

}

void * function1(void * arg)    //thread1
{
    while(count < 11)
    {
        if(count < 3 || (7 <= count && count < 10)) //thread1이 count값이 3이하, 8~10일때 출력하도록 보장
        {
            pthread_mutex_lock(&mutex);             //혹시모르는 2번thread의 접근을 방지하기 위해 mutex lock
            count++;                                //count값 증가
            printf("By function1, count value: %d\n",count); //출력       
            pthread_mutex_unlock(&mutex);           //mutex_unlock
        }
        else    
        {
            pthread_cond_signal(&cond); //thread2번을 깨움
            sleep(1);                   //thread2를 깨우고 1초간 대기
            pthread_cond_wait(&cond, &mutex);   //thread1번이 signal올때까지 wait
        }
    }
}
void * function2(void * arg)    //thread2
{
    while(count < 11)
    {
        if((3 <= count && count < 7 ) || count ==10 ) //thread1의 수행부분을 제외한 부분 수행
        {
            pthread_mutex_lock(&mutex);                 //mutex_lock
            count++;                                    //count증가
            printf("By function2, count value: %d\n",count); //출력
            pthread_mutex_unlock(&mutex);               //unlock
        }
        else
        {
            pthread_cond_signal(&cond);             //thread1을 꺠움
            sleep(1);                               //1초간 대기
            pthread_cond_wait(&cond, &mutex);       //signal이 올때까지 대기한다.
        }
        if(count == 11)
            pthread_cond_signal(&cond);          //count가 11이 되었으면 프로그램을 끝내기 위해 thread1을 깨운다.
    }
}