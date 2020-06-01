#include <stdio.h>

int main()
{
    char a[64];
    
    fgets(a,sizeof(a),stdin);
    printf("%s",a);
    return 0;
}