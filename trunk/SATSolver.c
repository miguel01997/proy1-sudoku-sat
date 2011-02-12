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
    short state;
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

int satisfiable(clause** clause_array,variable* variable_array,
		int carray_size)
{	int i,j;
	int true_count;
	int false_count;
	int var_pos;
	true_count = 0;
	false_count = 0;
	for(i=1;i<=carray_size;i++)
	{	clause* c = clause_array[i];
		false_count = 0;
		for(j=0;j<c->literals.size;j++)
		{   if(c->literals.array[j] < 0)
			var_pos = -c->literals.array[j];
		    else
			var_pos = c->literals.array[j];	
		    if(variable_array[var_pos].state == 0 
			&& c->literals.array[j] > 0)
			    false_count += 1;   			
		    if(variable_array[var_pos].state == 1 
			&& c->literals.array[j] < 0)
			    false_count += 1;    
		    //At least one variable is true
		    if(variable_array[var_pos].state == 1
			&& c->literals.array[j] > 0)
			break;
		    if(variable_array[var_pos].state == 0
			&& c->literals.array[j] < 0)
			break;
		}
		if(j < c->literals.size)
		    true_count += 1;
		//one clause is completely false
		if(false_count == c->literals.size)
		    return 0;
	}
	if(true_count == carray_size)
	    return 1;

	return -1;
}

//Returns the first unit clause.If no unit 
//clause is found, return 0.
int unitClause(clause** clause_array,int carray_size)
{	int i;
	for(i=1;i<=carray_size;i++)
	{	if(clause_array[i]->w_1_i == clause_array[i]->w_2_i)
			return clause_array[i]->literals.array[clause_array[i]->w_1_i];	
	}
	return 0;
}


//Returns first literal(variable) if it is pure.
//If no pure literal is found, zero is returned.
int pureLiteral(clause** clause_array,int carray_size)
{	int i,j,k,l,m,p;
	int not_pure;
	//Grab a clause
	for(i=1;i<carray_size;i++)
	{//Grab a literal
		int size_clause_i = clause_array[i]->literals.size;
		for(j=0;j<size_clause_i;j++)
		{	not_pure = 0;
			l = clause_array[i]->literals.array[j];
			//Compare with other clauses
			for(k=i+1;k<carray_size;k++)
			{//Grab another literal from another clause
				int size_clause_k = clause_array[k]->literals.size;
				for(m=0;m<size_clause_k;m++)
				{	p = clause_array[k]->literals.array[m];
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

int unitPropagate(int unit_clause,variable* variable_array)
{   int i,j,false_count;
    rarray_clause* list;
    int v;
    if(unit_clause > 0)
    {	list = &variable_array[unit_clause].nW;
	v = 1;
    }
    else
    {	list = &variable_array[unit_clause].pW;
	v = 0;
    }

    variable_array[unit_clause].state = v;
    for(i=0;i<list->size;i++) 
    {   clause* c = list->array[i];
	false_count = 0;
	for(j=0;j<c->literals.size;j++)
	{   if(variable_array[c->literals.array[j]].state == 1);

	    if(variable_array[c->literals.array[j]].state == -1
		&& j != c->w_1_i && j != c-> w_2_i)
	    {   //w_1_i is the watched literal
	    if(c->literals.array[c->w_1_i] ==  unit_clause)
		c->w_1_i = j;
	    else
		c->w_2_i = j;
		break;
	    }
	    if(variable_array[c->literals.array[j]].state == 0)
		false_count++;	
	}
	if(false_count == c->literals.size)
	    return 0;
	//w_2_i is pointing to a unit clause.
	if(false_count == c->literals.size - 1
		&& variable_array[c->literals.array[c->w_2_i]].state == -1)
	{   unitPropagate(c->literals.array[c->w_2_i],variable_array);
	}
	//w_1_i is pointing to a unit clause
	if(false_count == c->literals.size - 1
	    && variable_array[c->literals.array[c->w_1_i]].state == -1)
	{   unitPropagate(c->literals.array[c->w_1_i],variable_array);
	}
    }
    return 1;
}

int pureLiteralAssign(variable* variable_array, int literal)
{   if(literal > 0) 
	variable_array[literal].state = 1;
    else
	variable_array[literal].state = 0; 
}

int chooseNextLiteral(variable* variable_array, 
		    int varray_size, int hint)
{   int i;
    for(i=hint;i<varray_size;i++)
	if(variable_array[i].state == -1)
	    return i;
}
    
int DPLL(clause** clause_array,int carray_size,
	variable* variable_array,int varray_size,int hint)
{	int unit_clause,pure_literal,i;
	if(satisfiable(clause_array,variable_array,carray_size) > 0)
		return 1;
	if(satisfiable(clause_array,variable_array,carray_size) == 0)	
	    return 0;

	unit_clause = unitClause(clause_array,carray_size);
	
	while(unit_clause != 0)
	{   unitPropagate(unit_clause,variable_array);
	    unit_clause = unitClause(clause_array,carray_size);
	    printf("%d\n",unit_clause);
	}
	
	printf("Hallo!\n");	
	pure_literal = pureLiteral(clause_array,carray_size);
	while(pure_literal != 0)
	{   if(pureLiteralAssign(variable_array,pure_literal) == 0)
		return 0;
	    pure_literal = pureLiteral(clause_array,carray_size);
	}
	i = chooseNextLiteral(variable_array,varray_size,hint);
	variable_array[i].state = 1;
	if(DPLL(clause_array,carray_size,variable_array,varray_size,i) == 1)
	    return 1;
	else
	{   variable_array[i].state = 0;
	    return DPLL(clause_array,carray_size,variable_array,varray_size,i);
	}
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
      variable_array[k].state = -1;
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
    
    
    /*int j;
    for(j=1;j<=C;j++)
    {   printf("Clause %d:\n",j);
	printf("\tw_1_i: %d w_2_i: %d\n",clause_array[j]->w_1_i,clause_array[j]->w_2_i);
	int k;
	printf("Literals: ");
	for(k=0;k<clause_array[j]->literals.size;k++)
	    printf("%d ",clause_array[j]->literals.array[k]);	
	printf("Number of literals %d\n",clause_array[j]->literals.size);
	printf("\n");
    }*/

    printf("%d\n",DPLL(clause_array,C,variable_array,V,0)); 
    return 0;
 }


      
