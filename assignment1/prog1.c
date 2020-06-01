#define BUF_SIZE 16

#include <string.h> //strtok
#include <stdio.h>
#include <stdlib.h> //exit
#include <fcntl.h> //open
#include <unistd.h> //close

int main()
{
    int fd;
    char buf[BUF_SIZE+1];
    
    char* Student_id[2];
    int unix_[2];
    int c_[2];
    int network_[2];
    float ave[2];
    
    if((fd = open("./data", O_RDONLY))== -1){
        printf("file open error!\n");
        exit(-1);
    }

    for(int cnt=0;cnt<2;cnt++)
    {
        if(read(fd,buf,BUF_SIZE+1)==-1)
        {
            printf("file read error!\n");
            close(fd);
            exit(-1);
        }
        buf[BUF_SIZE+1] = '\0';

        char token[4][8];
        char *tok = strtok(buf," ");
        int i = 0;
        while(tok != NULL)
        {   
            strcpy(token[i], tok);
            tok = strtok(NULL," ");
            i++;
        }

        Student_id[cnt] = token[0];
        unix_[cnt] = atoi(token[1]);
        c_[cnt] = atoi(token[2]);
        network_[cnt] = atoi(token[3]);
        ave[cnt] = (network_[cnt]+unix_[cnt]+c_[cnt])/3;

        printf("ÇÐ¹ø : %s\tÆò±Õ : %.1lf\n",Student_id[cnt],ave[cnt]);
    }    

    close(fd);
    return 0;
}