#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <semaphore.h>
#include <unistd.h> 

#define SHM_SIZE 30

int main()
{
    int shm_id;
    void *shared_memory = (void*)0;
    key_t key = 1011;

    //share memory 공간 생성
    if((shm_id = shmget(key, SHM_SIZE, IPC_CREAT|0666)) < 0)
    {
        fprintf(stderr,"shmget error!\n");
        exit(-1);
    }

    //share memory 사용하기 위해 프로세스 메모리 붙이기
    shared_memory = shmat(shm_id, (void*)0,0);
    if(shared_memory == (void*)-1)
    {
        fprintf(stderr,"shmat error!\n");
        exit(-1);    
    }

    for(int i =0;i<10;i++)
    {
        sprintf((char *)shared_memory, "system");
        sleep(1);
        printf("A: %s\n",(char*)shared_memory);
    }

    sleep(2); //두 프로세스 접근시, 양쪽 프로세스 모두 끝났을때
              //공유메모리 제거 보장

   if ( shmdt( shared_memory) == -1)
    {
      printf( "shmdt error!\n");
      exit(-1);                 //공유 메모리 분리 실패
    }    
    
    
    shmctl(shm_id, IPC_RMID, 0); //공유 메모리 제거
    
    return 0;

}