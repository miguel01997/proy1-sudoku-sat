#include <stdio.h>
#include <stdlib.h>
#define ABS(a) (a>=0?a:-a)

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
    int* array;
    int size;
} rarray_clause;

typedef struct{
    short state;
    rarray_clause pW;
    rarray_clause nW;
} variable;

int* assign_queue;
int index_assign_queue;

int updateVariableArray(rarray_clause* W,int index)
{
    W->size += 1;
    printf("JOLOPOSOSI\n"); 
    W->array = (int*) realloc(W->array,
		(W->size)*sizeof(int));
    if(W->array == NULL)
    {
	printf("Ran out of memory.\n");
	return -1;
    }
    W->array[W->size-1] = index;
    return 0;
}

int satisfiable(clause* clause_array,variable* variable_array,
		int carray_size)
{	int i,j;
	int true_count;
	int false_count;
	int var_pos;
	true_count = 0;
	false_count = 0;
	for(i=1;i<=carray_size;i++)
	{	clause* c = &clause_array[i];
		false_count = 0;
		for(j=0;j<c->literals.size;j++)
		{   var_pos = ABS(c->literals.array[j]);
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


//Returns first literal(variable) if it is pure.
//If no pure literal is found, zero is returned.
int pureLiteral(clause* clause_array,variable* variable_array,int carray_size)
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
				int size_clause_k = clause_array[k].literals.size;
				for(m=0;m<size_clause_k;m++)
				{	p = clause_array[k].literals.array[m];
					if(p == -l)
						not_pure = 1;
				}
			}
			if(not_pure == 0)
			{   int pp = ABS(p);
			    if(variable_array[pp].state == -1)
				return p;
			}
		}
	}
	return 0;
}

int pureLiteralAssign(variable* variable_array, int literal)
{   if(literal > 0) 
	variable_array[literal].state = 1;
    else
	variable_array[-literal].state = 0; 
}

int chooseNextLiteral(variable* variable_array, 
		    int varray_size, int hint)
{   int i;
    for(i=hint+1;i<=varray_size;i++)
	if(variable_array[i].state == -1)
	    return i;
}

void printAssignments(variable* variable_array,int varray_size)
{   int i;
    for(i=1;i <= varray_size;i++)
	    printf("%d state: %d ",i,variable_array[i].state);
    printf("\n");
}
    
//pos is always positive.
int updateWatchedLiterals(variable* variable_array, int pos,
			int v,clause* clause_array,int* free_pos)
{   rarray_clause* list;
    int i,flag_true,flag_update;
    printf("pos: %d\n",pos);
    if(v == 1)   
	list = &variable_array[pos].nW;
    else
	list = &variable_array[pos].pW;
 
    int fp1 = *free_pos + 1;
    
    printf("posd: %d\n",pos);  
    for(i=0;i<list->size;i++)
    {	if(list->array[i] == 0) 
	    continue;
	printf("caca\n");
	clause* c = &clause_array[list->array[i]];
	int j;
	int fc = 0;
	int state_w1 = variable_array[ABS(c->literals.array[c->w_1_i])].state;
	flag_true = 0;	
    printf("coco\n");
    printf("%d\n",c->literals.size);
	for(j=0;j<c->literals.size;j++)
	{   int lit = c->literals.array[j];
	    int lit_abs = ABS(lit);
	    int state_lit = variable_array[lit_abs].state;
	    printf("j: %d\n",j);
	    if(lit > 0 && state_lit == 1)
	    {	flag_true = 1;
		break;
	    }
	    if(lit < 0 && state_lit == 0)
	    {	flag_true = 1;
		break;
	    }
	    if(state_lit == -1 && j != c->w_1_i && j != c->w_2_i)
	    {
		if(state_w1 == -1)
		    c->w_2_i = j;
		else
		    c->w_1_i = j; 

		if(lit > 0)
		{   
		    rarray_clause* modpW = &variable_array[lit_abs].pW; 
		    updateVariableArray(modpW,list->array[i]);
		}
		else
		{   
		    printf("JELLOOO2!\n");	
		    rarray_clause* modnW = &variable_array[lit_abs].nW;
		    updateVariableArray(modnW,list->array[i]);
		    printf("JALLOOO!\n");	
		}	

		if(v==1)
		    variable_array[pos].nW.array[i] = 0;
		else	
		    variable_array[pos].pW.array[i] = 0;

		break;
	    }
	    if(lit > 0 && state_lit == 0) 
		fc++;
	    if(lit < 0 && state_lit == 1)
		fc++;
	}
	if(fc == c->literals.size - 1 && flag_true != 1)
	{   int w1 = c->literals.array[c->w_1_i];
	    int w2 = c->literals.array[c->w_2_i];
	    int abs_w1 = ABS(w1);
	    int abs_w2 = ABS(w2);

	    if(state_w1 == -1)   
	    {	printf("Hi\n");
		if(w1 > 0)
		{   variable_array[abs_w1].state = 1;
		    assign_queue[*free_pos] = abs_w1;
		    if(updateWatchedLiterals(variable_array,abs_w1,1,clause_array,&fp1) == 0)
		    {	
			int k;
			for(k=free_pos;k<fp1;k++)
			{   printf("AQ: %d\n",assign_queue[k]);
			    variable_array[assign_queue[k]].state = -1;}
			index_assign_queue = 0;
			return 0;   
		    }
		}
		else
		{   variable_array[abs_w1].state = 0;
		    assign_queue[index_assign_queue] = abs_w1;
		    index_assign_queue++;
		    if(updateWatchedLiterals(variable_array,abs_w1,0,clause_array) == 0)
		    {	
			int k;
			for(k=0;k<index_assign_queue;k++)
			{   printf("AQ: %d\n",assign_queue[k]);
			    variable_array[assign_queue[k]].state = -1;
			}
			index_assign_queue = 0;
			return 0;
		    }
		}
	    }
	    else
	    {	printf("Hi\n");
		if(w2 > 0)
		{   variable_array[abs_w2].state = 1;
		    assign_queue[index_assign_queue] = abs_w2;
		    index_assign_queue++;

		    if(updateWatchedLiterals(variable_array,abs_w2,1,clause_array) == 0)
		    {	
			int k;
			for(k=0;k<index_assign_queue;k++)
			{   printf("AQ: %d\n",assign_queue[k]);  
			    variable_array[assign_queue[k]].state = -1;
			}
			index_assign_queue = 0;
			return 0;
		    }
		}
		else
		{   variable_array[abs_w2].state = 0;
		    assign_queue[index_assign_queue] = abs_w2;
		    index_assign_queue++;

		    if(updateWatchedLiterals(variable_array,abs_w2,0,clause_array) == 0)	
		    {	
			int k;
			for(k=0;k<index_assign_queue;k++)
			{   printf("AQ: %d\n",assign_queue[k]);
			    variable_array[assign_queue[k]].state = -1;
			}
			index_assign_queue = 0;
			return 0;
		    }
		}
	    }
	}
	if(fc == c->literals.size && flag_true != 1)
	    return 0;
    }
    return 1;
}

int DPLL(clause* clause_array,int carray_size,
	variable* variable_array,int varray_size,int hint)
{	
	int unit_clause,pure_literal,i,j;
	int sat_res;

	sat_res = satisfiable(clause_array,variable_array,carray_size); 
	if(sat_res > 0)
	{   printAssignments(variable_array,varray_size);
	    return 1;}
	if(sat_res == 0)	 
	    return 0;

	i = chooseNextLiteral(variable_array,varray_size,hint);
	
	printf("hint: %d\n",i);
	variable_array[i].state = 1;

	index_assign_queue = 0;
	if(updateWatchedLiterals(variable_array,i,1,clause_array) == 0)
	{   variable_array[i].state = 0;		
	    index_assign_queue = 0;
	    if(updateWatchedLiterals(variable_array,i,0,clause_array) == 0)
	    {	variable_array[i].state = -1;
		return 0;
	    }		
	    if(DPLL(clause_array,carray_size,variable_array,varray_size,i) == 0)
	    {	variable_array[i].state = -1;
		int k;	
		for(k=0;k<index_assign_queue;k++)
		    variable_array[assign_queue[k]].state = -1;
		index_assign_queue = 0;
		return 0;
	    }
	    return 1;
	}

	if(DPLL(clause_array,carray_size,variable_array,varray_size,i) == 1)
	    return 1;
	else
	{   variable_array[i].state = 0;
	    index_assign_queue = 0;
	    if(updateWatchedLiterals(variable_array,i,0,clause_array) == 0)
	    {   variable_array[i].state = -1;
		return 0;		
	    }
	}
	if(DPLL(clause_array,carray_size,variable_array,varray_size,i) == 0)
	{   variable_array[i].state = -1;
	    int k;	
	    for(k=0;k<index_assign_queue;k++)
		variable_array[assign_queue[k]].state = -1;
	    index_assign_queue = 0;
	    return 0;
	}
	
	return 1;
}

int main(void){

    int V,C;
    scanf("p cnf %d%d",&V,&C);
    //To count from 1 to N.
    clause clause_array[C+1];
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
      
    while(scanf("%d",&v) != EOF)
    {	if(v == 0) 
	{   
	    int k;
	    int w1,w2;
	     
	    clause_array[I].literals.array = malloc(varcount*sizeof(int));
	    if(clause_array[I].literals.array == NULL)
	    {	printf("Oops!\n");
		return 0;
	    }
	    clause_array[I].literals.size = varcount;
	    
	    for(k = 0;k<varcount;k++)
		clause_array[I].literals.array[k] = buffer_variables[k];
	    clause_array[I].w_1_i = 0;
	    clause_array[I].w_2_i = varcount - 1;

	    w1 = clause_array[I].literals.array[clause_array[I].w_1_i];
            w2 = clause_array[I].literals.array[clause_array[I].w_2_i];

	    varcount = 0;

	    
	    if( w1 > 0)
		{   if(updateVariableArray(&(variable_array[w1].pW),
				       I) == -1)
		    return -1;
		}
		else
		{   if(updateVariableArray(&(variable_array[-w1].nW),
				       I) == -1)
		    return -1;
		}
		if(w2 > 0)
		{	if(updateVariableArray(&(variable_array[w2].pW),
				       I) == -1)
			return -1;
		}
		else
		{	if(updateVariableArray(&(variable_array[-w2].nW),
				       I) == -1)
			return -1;
		}
		I++;
	}
	else 
	{   buffer_variables[varcount] = v;
	    varcount++;	    
	}
	
    } 
   
    assign_queue = malloc(V*sizeof(int));
    if(assign_queue == NULL)
    {	printf("Out of memory.\n");
	return 0;
    }
    printf("%d\n",DPLL(clause_array,C,variable_array,V,0)); 
    return 0;
 }


      
