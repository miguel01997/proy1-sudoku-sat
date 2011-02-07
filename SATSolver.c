#include <stdio.h>
typedef struct{
    int w_1_i;
    int w_2_i;
    int* literals;
} clause;

typedef struct{
    clause** pW;
    clause** nW;
} variable;

 int main(void)
{   FILE* fp;
    char header[50];
    int V,C;
    int i;
    variable* VarList;
    fp = fopen("result.out","r");
    fscanf(fp,"%*s%*s%d%d",&V,&C);
    for(i=0;i<C;i++)
    {;
    }
}
