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
	    if(v ==' ') continue;
	    a[i*dim + j] = v;
	    j += 1;
	    if(j >= dim){
		i += 1;
		j = 0;
	    }
	}
	if(!(i == dim && j == 0))
	    return -1;
	return 0;
}

int checkSudoku()
{   //Check for repetitions in rows
    int i,j,k;
    int check[dim+1];
    FOR(i,dim)
    {	FOR(k,dim+1)
	    check[k] = 0;
	FOR(j,dim)
	{//Check and mark
	    if(	a[i*dim+j] != '0' &&
		check[a[i*dim+j] - '0'] == 0)
		    check[a[i*dim+j]- '0'] = 1;
	    else if( a[i*dim+j] == '0'); 
	    else
		return -1;	
	}	
    }
    //Check for repetitions in columns
    FOR(i,dim)
    {	FOR(k,dim+1)
	    check[k] = 0;
	FOR(j,dim)
	{//Check and mark
	    if(	a[j*dim+i] != '0' &&
		check[a[j*dim+i] - '0'] == 0)
		    check[a[j*dim+i]- '0'] = 1;
	    else if( a[j*dim+i] == '0'); 
	    else
		return -1;	
	}
    }
    return 0;
}

int main(void)
{	int i,j,k;
	if(matrixFill() == -1)
	{   printf("Bad input\n");    
	    return 0;
	}
	if(checkSudoku() == -1)
	{   printf("Invalid Sudoku configuration\n");
	    return 0;
	}	
	FOR(i,dim)
	{   FOR(j,dim)
	    {	if(a[i*dim+j] == '0')
		{   FOR(k,dim)
			printf("%d%d%d ",i+1,j+1,k+1);   
		    printf("%d\n",0);
		}
		else
		    printf("%d%d%d 0\n",i+1,j+1,a[i*dim+j]-'0');
	    }
	}
	return 0;
}
