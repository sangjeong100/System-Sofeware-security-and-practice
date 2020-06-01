#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

//한 프로세스가 모든 연샨 수행하는 program

#define BUF_SIZE 64

int main()
{
    clock_t start = clock(); //clock 함수 선언 시작 시간 check
   
    FILE * fp1, *fp2, *fp3, *fp4;

    int sum = 0;
    char buf[4][BUF_SIZE]; //각 파일별 buffer


    if((fp1 = fopen("A","r")) == NULL)
    {
        fprintf(stderr,"fopen error\n");
        exit(-1);
    }

    if((fp2 = fopen("B","r")) == NULL)
    {
        fprintf(stderr,"fopen error\n");
        exit(-1);
    }

    if((fp3 = fopen("C","r")) == NULL)
    {
        fprintf(stderr,"fopen error\n");
        exit(-1);
    }

    if((fp4 = fopen("D","r")) == NULL)
    {
        fprintf(stderr,"fopen error\n");
        exit(-1);
    }
    
    int a_sum =0;
    while(!feof(fp1))  // 파일의 끝이 아니라면
    {
        fgets(buf[0], sizeof(buf[0]), fp1);  // buf size만큼 한줄 읽기

        char * token = strtok(buf[0],"\t\n "); //숫자들 띄어쓰기 tab \n 를 기준으로 자름
        
        char *ptr;
        if((ptr = strchr(buf[0],'\n')) != NULL) // 개행문자 있으면 널값으로 바꿈
        {
            *ptr = '\0'; 
        }
        
        while(token != NULL)
        {
            a_sum += atoi(token);
            token = strtok(NULL,"\t\n ");
        } 
    }

    sum += a_sum;
    
    int b_sum =0;
    while(!feof(fp2))  // 파일의 끝이 아니라면
    {
        fgets(buf[1], sizeof(buf[1]), fp2);  //buf size만큼 한줄 읽기

        char * token = strtok(buf[1],"\t\n "); //숫자들 띄어쓰기 tab \n 를 기준으로 자름
        
        char *ptr;
        if((ptr = strchr(buf[1],'\n')) != NULL) // 개행문자 있으면 널값으로 바꿈
        {
            *ptr = '\0'; 
        }

        while(token != NULL)
        {
            b_sum += atoi(token);
            token = strtok(NULL,"\t\n ");
        } 
    
    }

    sum += b_sum;

    int c_sum =0;
    while(!feof(fp3))  // 파일의 끝이 아니라면
    {
        fgets(buf[2], sizeof(buf[2]), fp3);  // buf size만큼 한줄 읽기

        char * token = strtok(buf[2],"\t\n "); //숫자들 띄어쓰기 tab \n 를 기준으로 자름
        
        char *ptr;
        if((ptr = strchr(buf[2],'\n')) != NULL) // 개행문자 있으면 널값으로 바꿈
        {
            *ptr = '\0'; 
        }        
        
        while(token != NULL)
        {
            c_sum += atoi(token);
            token = strtok(NULL,"\t\n ");
        }
    }

    sum += c_sum;

    int d_sum =0;
    while(!feof(fp4))  // 파일의 끝이 아니라면
    {
       fgets(buf[3], sizeof(buf[3]), fp4);  // buf size만큼 한줄 읽기
        
        char *ptr;
        if((ptr = strchr(buf[3],'\n')) != NULL) // 개행문자 있으면 널값으로 바꿈
        {
            *ptr = '\0'; 
        }


       char * token = strtok(buf[3],"\t\n "); //숫자들 띄어쓰기 tab \n 를 기준으로 자름

       while(token != NULL)
       {
            d_sum += atoi(token);
            token = strtok(NULL,"\t\n ");
       } 
    
    }

    sum += d_sum;    


    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);

    printf("sum = %d\n",sum);

    clock_t end = clock();
    printf("한 프로세스가 모두 수행 하는 프로그램의 수행Time : %lf\n", (double)(end - start)/CLOCKS_PER_SEC);



    return 0;



}