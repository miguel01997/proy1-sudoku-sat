#include <stdio.h>
#include <stdlib.h>
#define FOR(a,b) for(a=0;a<b;a++)
int order;
int N;
int dim;
int V,C,UC;
char* a;

int matrixFill()
{	int i,j;
	char v;
	i = 0;
	j = 0;
	UC = 0;
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

void problem_line()
{   int sum = dim*(dim-1) - (dim*(dim+1)/2 - 1) + dim - 1;
    //number of one entry and unit clauses
    int A = dim*dim;
    //number of unique entry clauses
    int B = dim*dim*sum;
    //number of row,column and subgrid clauses
    int D = 3*dim*dim*sum;
    V = dim*dim*dim;
    C = A + B + D;
    printf("p cnf %d %d\n",V,C);
}

void one_number_clause()
{   int i,j,k;
    FOR(i,dim)
    {   FOR(j,dim)
	{   if(a[i*dim+j] == '0')
	    {   FOR(k,dim)
		    printf("%d%d%d ",i+1,j+1,k+1);   
		printf("%d\n",0);
	    }
	    else
		printf("%d%d%d 0\n",i+1,j+1,a[i*dim+j]-'0');
	    
	}
    }
}
/* Given an N^2 x N^2 sudoku matrix produces the clauses
 * which guarantee that there is no more than one number
 * per cell */

void unique_number_clause()
{   int i,j,k,m;

    for(i=1;i<=dim;i++)
    { // for every cell
	for(j=1;j<=dim;j++)
	{/* where the variable count for the current
	* cell starts */
	/* create a clause for every possible pair of
	* variables that refer to the cell's value */
	for(k=1; k<dim; k++)
	    for(m=k+1; m<=dim; m++)
		printf("-%d%d%d -%d%d%d 0\n",i,j,k,i,j,m);
	}
    }
}

void row_clause()
{   int y,z,x,i;
    for(y=1;y<dim+1;y++)
    	for(z=1;z<dim+1;z++)
	    for(x=1;x<dim;x++)
    	    	for(i=x+1;i<dim+1;i++)
		    printf("-%d%d%d -%d%d%d 0\n",x,y,z,i,y,z);	    
}

void column_clause()
{   int y,z,x,i;
    for(x=1;x<dim+1;x++)
    	for(z=1;z<dim+1;z++)
	    for(y=1;y<dim;y++)
    	    	for(i=y+1;i<dim+1;i++)
		    printf("-%d%d%d -%d%d%d 0\n",x,y,z,x,i,z);	    
}

void subgrid_clause()
{   int z,i,j,x;
    int y,k,l;
    for(z=1;z<dim+1;z++)
	for(i=0;i<order;i++)
	    for(j=0;j<order;j++)
		for(x=1;x<=order;x++)
		    for(y=1;y<=order;y++)
			for(k=y+1;k<=order;k++)
			{   printf("-%d%d%d -%d%d%d 0\n",
				    order*i+x,order*j+y,z,
				    order*i+x,order*j+k,z);
			}
    
    for(z=1;z<dim+1;z++)
	for(i=0;i<order;i++)
	    for(j=0;j<order;j++)
		for(x=1;x<=order;x++)
		    for(y=1;y<=order;y++)
			for(k=y+1;k<=order;k++)
			    for(l=1;l<=order;l++)
			    {	printf("-%d%d%d -%d%d%d 0\n",
					order*i+x,order*j+y,z,
					order*i+k,order*j+l,z);
			    }	
}


int main(void)
{	if(matrixFill() == -1)
	{   printf("Bad input\n");    
	    return 0;
	}
	if(checkSudoku() == -1)
	{   printf("Invalid Sudoku configuration\n");
	    return 0;
	}
	problem_line();	
	one_number_clause();
	unique_number_clause();
	row_clause();
	column_clause();
	subgrid_clause();
	return 0;
}
