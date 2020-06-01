#include <grp.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    struct group *primary_grp; 
    struct passwd *pw;          
    struct group *grp;         

    char user_name[20];

    printf("Input user name: ");scanf("%s",user_name); 
    //user name 입력
    pw = getpwnam(user_name);
    //user이름으로 user정보 받아오기

    if(pw == NULL) //이름과 같은 user존재하지 않는지 체크
    {
        printf("존재하지 않는 User입니다.\n");
        exit(-1);
    }
    primary_grp = getgrgid(pw->pw_gid); //user의 primary group받아온다.
    
    if(primary_grp == NULL) //primary group이 없다면 오류 발생. 

    {
        printf("미등록된 그룹ID이거나 오류가 발생하였습니다.\n");
        exit(-1);
    }


    printf("User: %s\n",pw->pw_name);
    printf("Primary Group: %s\n",primary_grp->gr_name);
    //user name과 primary group 출력

    printf("Secondary Group:");
    while(grp = getgrent())
    {
        for(int i=0;grp->gr_mem[i]!=NULL;i++)
        {
        if(!strcmp(grp->gr_mem[i],pw->pw_name))
        {
            printf("%s ",grp->gr_name);
            break;
        }
        //각 그룹에 속한 menber를 참조하여 만약 입력받은 user가 존재하면 group name 출력
        //secondary group 출력
        }
    }
    endgrent();
    printf("\n");
    //getgrent로 전체 group 목록 참조




    return 0;
}