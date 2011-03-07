#include "iterative_dpll.h"
int main(void){

    int V,C;
    int v;
    int i;
    int I;
    int varcount;
    int k;

    scanf("p cnf %d%d",&V,&C);
    //To count from 1 to N.
    clause clause_array[2*C*C+C+1];
    variable variable_array[V+1];
    igraph_node igraph[V+1];
    int buffer_variables[V]; 

    //Initialize variables.
    for(k = 1; k<= V; k++){
	
	variable_array[k].pW.array = 0;
	variable_array[k].nW.array = 0;
	variable_array[k].pW.size = 0;
	variable_array[k].nW.size = 0;
	variable_array[k].state = -1;
	variable_array[k].toggled = 0;

	igraph[k].decision_level = -1;
	igraph[k].implicant_clause = -1; 	
    }

    I = 1;
    varcount = 0;
      
    while(scanf("%d",&v) != EOF)
      {	if(v == 0 && varcount == 1){
          variable_array[ABS(buffer_variables[0])].state = (buffer_variables[0] > 0)? 1 : 0;
          varcount = 0;
          assigned_vars++;
          C--;
        } else if(v==0)  
	{   
	    int k;
	    int w1,w2;
	     
	    //Initialize clause.
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
	    clause_array[I].tag = 0;

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
   
    assign_queue = malloc((V)*sizeof(decision));
    index_assign_queue = 0;
    if(assign_queue == NULL)
    {	printf("Out of memory.\n");
	return 0;
    }
    
    printf("\n%d\n",DPLL(clause_array,variable_array,igraph,C,V)); 
    return 0;
}


