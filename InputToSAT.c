#include <stdio.h>
#include <stdlib.h>
int order;
int N;
int dim;
char* a;

int matrixFill()
{	int i,j;
	char v;
	i = 0;
	j = 0;
	scanf("%d",&order);
	dim = order*order;
	N = dim*dim*dim;
	a = malloc(dim*dim*sizeof(char));
	while( scanf("%c",&v) != EOF)
	{   if(v == '\n') break;
	    if(v==' ') continue;
	    a[i*dim + j] = v;
	    j += 1;
	    if(j >= dim){
		i += 1;
		j = 0;
	    }
	}
	printf("%s\n",a);
	if(!(i == dim-1 && j == dim-1))
	{
	    free(a);
	    return -1;
	}
	return 0;
}


int main(void){
	matrixFill();	
	return 0;
}
