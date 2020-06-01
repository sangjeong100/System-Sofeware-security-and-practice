#define STUDENT_ID_SIZE 8

#include <stdio.h>  //고수준 파일 입출력
#include <stdlib.h> //exit

int main()
{
    FILE *fp = NULL;
    char* Student_id[2];
       
    int unix_[2];
    int c_[2];
    int network_[2];
    float ave[2];
    
    if((fp = fopen("./data","r"))== NULL){
        printf("file open error!\n");
        exit(-1);
    }
    
    for(int i=0;i<2;i++)
    {   Student_id[i] = (char*)malloc(sizeof(char)*STUDENT_ID_SIZE); 
        fscanf(fp,"%s %d %d %d", Student_id[i], &unix_[i], &c_[i], &network_[i]);
        ave[i] = (unix_[i] + c_[i] + network_[i])/3; 
        printf("학번 : %s\t평균 : %.1lf\n",Student_id[i],ave[i]);
    }
    
    fclose(fp);
    return 0;
}