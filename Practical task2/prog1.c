#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
    if(argc!=3)
    {
        printf("프로그램의 인자가 부족하거나 많습니다.\n");
        return 0;
    }
    if(access(argv[2],F_OK)==-1)
    {
        printf("file이 존재하지 않거나 경로가 잘못 되었습니다.\n");
        return 0;
    }
    
    struct stat buf;
    
    stat(argv[2],&buf);
    

    if(*(argv[1]) == 'u')
    {
        if(*(argv[1]+1) == '+')
        {
            if(*(argv[1]+2) == 'r')
                buf.st_mode |= S_IRUSR;
            
            else if(*(argv[1]+2) == 'w')
                buf.st_mode |= S_IWUSR;
                
            else if(*(argv[1]+2) == 'x')
                buf.st_mode |= S_IXUSR;
            else
            {
                printf("옵션이 잘못되었습니다.\n");
                return 0;
            }

        }
        else if(*(argv[1]+1) == '-')
        {
            if(*(argv[1]+2) == 'r')
                buf.st_mode &= ~(S_IRUSR);
            else if(*(argv[1]+2) == 'w')
                buf.st_mode &= ~(S_IWUSR);
            else if(*(argv[1]+2) == 'x')
                buf.st_mode &= ~(S_IXUSR);                
            else
            {
                printf("옵션이 잘못되었습니다.\n");
                return 0;
            }
        }
        else
        {
            printf("옵션이 잘못되었습니다.\n");
            return 0;
        }
        
    }
    else if(*(argv[1]) == 'g')
    {
        if(*(argv[1]+1) == '+')
        {
            if(*(argv[1]+2) == 'r')
                buf.st_mode |= S_IRGRP;
            
            else if(*(argv[1]+2) == 'w')
                buf.st_mode |= S_IWGRP;
                
            else if(*(argv[1]+2) == 'x')
                buf.st_mode |= S_IXGRP;
            else
            {
                printf("옵션이 잘못되었습니다.\n");
                return 0;
            }
        }
        else if(*(argv[1]+1) == '-')
        {
            if(*(argv[1]+2) == 'r')
                buf.st_mode &= ~(S_IRGRP);
            else if(*(argv[1]+2) == 'w')
                buf.st_mode &= ~(S_IWGRP);
            else if(*(argv[1]+2) == 'x')
                buf.st_mode &= ~(S_IXGRP);                
            else
            {
                printf("옵션이 잘못되었습니다.\n");
                return 0;
            }
        }
        else
        {
            printf("옵션이 잘못되었습니다.\n");
            return 0;
        }
    }
    else if(*(argv[1]) == 'o')
    {
        if(*(argv[1]+1) == '+')
        {
            if(*(argv[1]+2) == 'r')
                buf.st_mode |= S_IROTH;
            
            else if(*(argv[1]+2) == 'w')
                buf.st_mode |= S_IWOTH;
                
            else if(*(argv[1]+2) == 'x')
                buf.st_mode |= S_IXOTH;
            else
            {
                printf("옵션이 잘못되었습니다.\n");
                return 0;
            }
        
        }
        else if(*(argv[1]+1) == '-')
        {
            if(*(argv[1]+2) == 'r')
                buf.st_mode &= ~(S_IROTH);
            else if(*(argv[1]+2) == 'w')
                buf.st_mode &= ~(S_IWOTH);
            else if(*(argv[1]+2) == 'x')
                buf.st_mode &= ~(S_IXOTH);                
            else
            {
                printf("옵션이 잘못되었습니다.\n");
                return 0;
            }
        }
        else
        {
            printf("옵션이 잘못되었습니다.\n");
            return 0;
        }
    }
    else{
        printf("옵션이 잘못되었습니다.\n");
        return 0;
    }
    chmod(argv[2],buf.st_mode);
    return 0;
}