#include <stdio.h>
#include <stdlib.h>
#define FOR(a,b) for(a=0;a<b;a++)
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
	free(a);
	if(!(i == dim-1 && j == dim-1))
	    return -1;
	return 0;
}

int checkSudoku()
{   //Check for repetitions in rows
    int i,j;
    int check[dim];
    FOR(i,dim)
    {	FOR(j,dim)
	{//Check and mark
	    if(check[a[(i*dim+j) - '0']] == 0)
		check[a[(i*dim+j)- '0']] == 1;
	    else
		return -1;	
	}	
    }
    //Check for repetitions in columns
    FOR(i,dim)
    {	j = 0;
	while(j != i + (dim-1)*dim)
	{   if(check[a[(i*dim+j) - '0']] == 0)
		check[a[(i*dim+j)- '0']] == 1;
	    else
		return -1;
	    j +=  dim;
	}
    }
}

int main(void){
	matrixFill();	
	int i,j,k;
	FOR(i,dim)
	{   FOR(j,dim)
	    {	if(a[i][j] == 0)
		{   FOR(k,dim)
		    {;}   
		}

	    }

	}
	return 0;
}
