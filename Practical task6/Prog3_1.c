#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <unistd.h> 
#include <sys/sem.h>

#define SHM_SIZE 30

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
};


int main()
{
    int shm_id, sem_id;
    void *shared_memory = (void*)0;
    key_t key = 1011;

    //semaphore
    union semun sem_union;
    struct sembuf mysem_open ; //독점권 획득
    struct sembuf mysem_close; //독점권 버림
    
    mysem_open.sem_num = 0;
    mysem_open.sem_op = -1;
    mysem_open.sem_flg = SEM_UNDO;

    mysem_close.sem_num = 0;
    mysem_close.sem_op = 1;
    mysem_close.sem_flg = SEM_UNDO;    

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

    if ((sem_id = semget( key, 1, 0666 | IPC_CREAT )) == -1 )
    {
        fprintf(stderr," semget error!\n");
        exit(-1);
    }

    sem_union.val = 1;
    semctl(sem_id, 0, SETVAL, sem_union);

    for(int i =0;i<10;i++)
    {
        if(semop(sem_id, &mysem_open, 1) == -1)
        {
            fprintf(stderr,"semaphore error!\n");
            exit(-1);
        }

        sprintf((char *)shared_memory, "Sema");
        sleep(1);
        printf("A: %s\n",(char*)shared_memory);
        
        //독점 해체
        semop(sem_id, &mysem_close,1);
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