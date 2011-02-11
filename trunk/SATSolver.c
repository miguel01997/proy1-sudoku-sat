#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 65536

typedef struct{
	int* array;
	int size;
} rarray_variable;

typedef struct{
    int w_1_i;
    int w_2_i;
    rarray_variable literals;
} clause;

typedef struct{
    clause** array;
    int size;
} rarray_clause;

typedef struct{
    rarray_clause pW;
    rarray_clause nW;
} variable;


int updateVariableArray(rarray_clause* W,clause* n)
{
    W->size += 1;

 
    W->array = (clause**) realloc(W->array,
		(W->size)*sizeof(clause*));
    if(W->array == NULL)
    {
	printf("Ran out of memory.\n");
	return -1;
    }
    W->array[W->size-1] = n;
    return 0;
}

 int main(void){



    int V,C;
    scanf("p cnf %d%d",&V,&C);
    //To count from 1 to N.
    clause * clause_array[C+1];
    variable variable_array[V+1];
    int buffer_variables[V];
    int v;
    int i;
    int I;
    int varcount;

    int k;
    for(k = 1; k<= V; k++){
      variable_array[k].pW.array = 0;
      variable_array[k].nW.array = 0;
      variable_array[k].pW.size = 0;
      variable_array[k].nW.size = 0;
    }

    I = 1;
    varcount = 0;
     
    clause * n1;
    while(scanf("%d",&v) != EOF)
    {	if(v == 0) 
	{   
	    int k;
	    int w1,w2;
	    
	    n1 = malloc(sizeof(clause));

	    

	    if((n1->literals.array = malloc(varcount*sizeof(int))) 
				   == NULL)
	    {	printf("Out of memory");
		return -1;
	    }
	   
	    
	    n1->literals.size = varcount;
	    for(k = 0;k<varcount;k++)
		n1->literals.array[k] = buffer_variables[k];
	    n1->w_1_i = 0;
	    n1->w_2_i = varcount - 1;

	    w1 = n1->literals.array[n1->w_1_i];
            w2 = n1->literals.array[n1->w_2_i];

	    	    if( w1 > 0)
	      {	if(updateVariableArray(&(variable_array[w1].pW),
				       n1) == -1)
		    return -1;
	    }
	    else
	      {	if(updateVariableArray(&(variable_array[-w1].nW),
				       n1) == -1)
		    return -1;
	    }
	    if(w2 > 0)
	      {	if(updateVariableArray(&(variable_array[w2].pW),
				       n1) == -1)
		    return -1;
	    }
	    else
	      {	if(updateVariableArray(&(variable_array[-w2].nW),
				       n1) == -1)
		    return -1;
		    }

	    clause_array[I] = n1;

	    varcount = 0;
	    I++;
	    
	}else {
	
	buffer_variables[varcount] = v;
	varcount++;	    
      }
	
    } 
    
    
    int j;
    for(j=1;j<=C;j++)
    {   printf("Clause %d:\n",j);
	printf("\tw_1_i: %d w_2_i: %d\n",clause_array[j]->w_1_i,clause_array[j]->w_2_i);
	int k;
	printf("Literals: ");
	for(k=0;k<clause_array[j]->literals.size;k++)
	    printf("%d ",clause_array[j]->literals.array[k]);	
	printf("Number of literals %d\n",clause_array[j]->literals.size);
	printf("\n");
    }
    return 0;
 }


      
