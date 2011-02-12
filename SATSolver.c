#include <stdio.h>
#include <stdlib.h>


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

int satisfiable(clause* clause_array,variable* variable_array,
		int carray_size)
{	int i,j;
	int true_count;
	int false_count;
	true_count = 0;
	false_count = 0;
	for(i=1;i<=carray_size;i++)
	{	clause c = clause_array[i];
		false_count = 0;
		for(j=0;j<c.literals.size;j++)
		{   if(variable_array[c.literals.array[j]].state == 0)
			false_count += 1;   			
		    //At least one variable is true
		    if(variable_array[c.literals.array[j]].state == 1)
			break;
		}
		if(j < c.literals.size)
		    true_count += 1;
		//one clause is completely false
		if(false_count == c.literals.size)
		    return 0;		
	}
	if(true_count == carray_size)
	    return 1;

	return -1;
}

//Returns the first unit clause.If no unit 
//clause is found, return 0.
int unitClause(clause* clause_array,int carray_size)
{	int i;
	for(i=1;i<=carray_size;i++)
	{	if(clause_array[i].w_1_i == clause[i].w_2_i)
			return clause_array[i].literals.array[w_1_i];	
	}
	return 0;
}


//Returns first literal(variable) if it is pure.
//If no pure literal is found, zero is returned.
int pureLiteral(clause* clause_array,int carray_size)
{	int i,j,k,l,m,p;
	int not_pure;
	//Grab a clause
	for(i=1;i<carray_size;i++)
	{//Grab a literal
		int size_clause_i = clause_array[i].literals.size;
		for(j=0;j<size_clause_i;j++)
		{	not_pure = 0;
			l = clause_array[i].literals.array[j];
			//Compare with other clauses
			for(k=i+1;k<carray_size;k++)
			{//Grab another literal from another clause
				int size_clause_k = clause.array[k].literals.size;
				for(m=0;m<size_clause_k;m++)
				{	p = clause_array[k].literals.array[m];
					if(p == -l)
						not_pure = 1;
				}
			}
			if(not_pure == 0)
				return p;
		}
	}
	return 0;
}

int unitPropagate(clause* clause_array, int carray_size,
		    int unit_clause,variable* variable_array)
{   int i,j;
    if(unit_clause > 0)
    {	variable_array[unit_clause].state = 1;
	for(i=0;i<variable_array[unit_clause].nW.size) 
	{   clause* c = variable_array[unit_clause].nW.array[i];
	    for(j=0;j<c->literals.size;j++)
	    {	if(variable_array[c->literals.array[j]].state == 1)
		    return 1;	
	    }
	}
    }
    else
	variable_array[unit_clause].state = 0;
   
    int i,j;
}
int DPLL(clause* clause_array,int carray_size)
{	int unit_clause;
	if(satisfiable(clause_array,carray_size) > 0)
		return 1;
	if(satisfiable(clause_array,carray_size) == 0)
		return 0;
	unit_clause = unitClause(clause_array,carray_size);
	if(unit_clause > 0)
	    unitPropagate(clause_array,carray_size,unit_clause);
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


      
