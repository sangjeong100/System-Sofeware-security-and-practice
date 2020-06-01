#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

//여러 개의 프로세스가 한 파일씩 담당하여 동시에 수행하고, 나중에 한 프로세스가 이를 모으는 프로그램

#define BUF_SIZE 64

int ChildRoutine(int child);

int main()
{
    time_t start = clock();
    
    char buf[32];    
    int pipe_[2]; //각 자식프로세스와 부모 프로세스와의 pipe통신을 통해 숫자 전달

    
    int pid[4];

    if (pipe(pipe_) < 0) {
        fprintf(stderr,"pipe error!\n");
        exit(-1);
    }


    for(int child=0;child<4;child++)
    {
        if((pid[child]=fork())<0)
        {
            fprintf(stderr,"fork error!\n");
            exit(-1);
        }
        else if(pid[child] == 0)
        {
            sprintf(buf,"%d",ChildRoutine(child)); //각 파일별 합 return

            close(pipe_[0]); //read 용 pipe close
            write(pipe_[1],buf,sizeof(buf)); // 부모에게 파일별 sum 전달
            
            exit(1); //  자식 process 할일끝내고 사망
        }
    }

    close(pipe_[1]); //부모가 write 용 pipe close

    int status;
    int wpid;
    int sum = 0;
    
    //총 4개의 자식 프로세스로 부터 pipe 통신하여 각 파일의 sum을 받는다.
    read(pipe_[0],buf,sizeof(buf)); 
    sum += atoi(buf);
    
    read(pipe_[0],buf,sizeof(buf));
    sum += atoi(buf);

    read(pipe_[0],buf,sizeof(buf));
    sum += atoi(buf);

    read(pipe_[0],buf,sizeof(buf));
    sum += atoi(buf);

    for(int child = 0 ; child < 4 ; child++) //모든 자식 끝날떄까지 wait
        if((wpid = wait(&status))== -1) 
        {
            fprintf(stderr,"wait error!\n");
            exit(-1);
        }
        
    
    printf("sum = %d\n",sum);

    clock_t end = clock();
    printf("여러 프로세스가 각각 수행 하는 프로그램의 수행Time : %lf\n", (double)(end - start)/CLOCKS_PER_SEC);

        
    return 0; //모든 자식 끝나면 종료

}

int ChildRoutine(int child)
{
    FILE *fp;
    int file_sum = 0;
    char buf[BUF_SIZE];

    if(child == 0)
    {
        if((fp = fopen("A","r")) == NULL) //A파일 open
        {
            fprintf(stderr,"fopen error!\n");
            exit(-1);
        }
    }
    else if(child == 1)
    {
        if((fp = fopen("B","r")) == NULL) //B파일 open
        {
            fprintf(stderr,"fopen error!\n");
            exit(-1);
        }
    }
    else if(child == 2)
    {
        if((fp = fopen("C","r")) == NULL) //C파일 open
        {
            fprintf(stderr,"fopen error!\n");
            exit(-1);
        }
    }        
    else if(child == 3)
    {
        if((fp = fopen("D","r")) == NULL) //D파일 open
        {
            fprintf(stderr,"fopen error!\n");
            exit(-1);
        }
    }
    while(!feof(fp))  // 파일의 끝이 아니라면
    {
        fgets(buf, sizeof(buf), fp);  // buf size만큼 파일 한줄 읽기
        
        char *ptr;
        if((ptr = strchr(buf,'\n')) != NULL) // 개행문자 있으면 널값으로 바꿈
        {
            *ptr = '\0'; 
        }
        
        char * token = strtok(buf,"\t\n "); //숫자들 띄어쓰기 tab \n 를 기준으로 자름

        while(token != NULL)
        {
                file_sum += atoi(token);
                token = strtok(NULL,"\t\n ");
        } 
    
    }

    fclose(fp);

    return file_sum;    

}