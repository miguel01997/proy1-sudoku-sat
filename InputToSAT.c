#include "bitvector.h"
int order;
int N;
int dim;

void fillVector()
{	int i;
	int v;
	scanf("%d",&order);
	dim = order*order;
	N = dim*dim*dim;
	FOR(i,N)
		clr(i);
	i = 0;
	while( scanf("%d",&v) != EOF)
	{	if (v == 0){
			i += dim;
		}
		else
		{
			i += v-1;
			set(i);
			i += dim - v + 1;  
		}
	}
}


int main(void){
	fillVector();
	int i,j,k,d;
	i = 0;
	while(i != 18)
	{   j = i;
	    FOR(j,i+dim)
		if(test(j))
		{
		    break;
		}
	    printf("j: %d\n",j);
	    if(j == i+dim)
	    {	d = 1;
		FOR(k,i+dim)
		{   printf("%d ",(i/dim)*100 + k*10 + d);
		    d++;
		}
		printf("%d\n",0);
	    } 
	    i += dim;
	}
	return 0;
}
