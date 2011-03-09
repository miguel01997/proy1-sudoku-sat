#include "iterative_dpll.h"
int main(void){

    int V,C;
    int v;
    int i;
    int I;
    int varcount;
    int k,s;
    int solve,resolved;

    scanf("p cnf %d%d",&V,&C);
    //To count from 1 to N.
    clause clause_array0[2*C+1];
    variable variable_array0[V+1];
    igraph_node igraph0[V+1];
    int buffer_variables[V]; 

    //Initialize conflict node
    igraph0[0].decision_level = -1;
    igraph0[0].implicant_clause = -1;
    //Initialize variables and nodes of implication graph
    for(k = 1; k<= V; k++){
	
	variable_array0[k].pW.array = 0;
	variable_array0[k].nW.array = 0;
	variable_array0[k].pW.size = 0;
	variable_array0[k].nW.size = 0;
	variable_array0[k].state = -1;
	variable_array0[k].toggled = 0;

	igraph0[k].decision_level = -1;
	igraph0[k].implicant_clause = -2; 	
    }

    I = 1;
    varcount = 0;
      
    while(scanf("%d",&v) != EOF){	
	if(v == 0 && varcount == 1){
	    variable_array0[ABS(buffer_variables[0])].state = ((buffer_variables[0] > 0) ? 1 : 0);
	    variable_array0[ABS(buffer_variables[0])].toggled = 1;
	    varcount = 0;
	    C--;
	    igraph0[ABS(buffer_variables[0])].decision_level = 1;
	    igraph0[ABS(buffer_variables[0])].implicant_clause = -1;
	    
        } else if(v==0)  
	{   
	    int k;
	    int w1,w2;
	     
	    //Initialize clause.
	    clause_array0[I].literals.array = malloc(varcount*sizeof(int));
	    if(clause_array0[I].literals.array == NULL)
	    {	printf("Oops!\n");
		return 0;
	    }
	    clause_array0[I].literals.size = varcount;
	    
	    for(k = 0;k<varcount;k++)
		clause_array0[I].literals.array[k] = buffer_variables[k];
	    clause_array0[I].w_1_i = 0;
	    clause_array0[I].w_2_i = varcount - 1;
	    clause_array0[I].tag = 0;

	    w1 = clause_array0[I].literals.array[clause_array0[I].w_1_i];
            w2 = clause_array0[I].literals.array[clause_array0[I].w_2_i];

	    varcount = 0;

	    
	    if( w1 > 0)
		{   if(addClause(&(variable_array0[w1].pW),
				       I) == -1)
		    return -1;
		}
		else
		{   if(addClause(&(variable_array0[-w1].nW),
				       I) == -1)
		    return -1;
		}
		if(w2 > 0)
		{	if(addClause(&(variable_array0[w2].pW),
				       I) == -1)
			return -1;
		}
		else
		{	if(addClause(&(variable_array0[-w2].nW),
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
    solve = 0;
    resolved = 0;
    clause_array = clause_array0;
    variable_array = variable_array0;
    igraph = igraph0;
    T = C + 1;
    d = 1;
    for(s=1;s<=V;s++){
	if(variable_array[s].state == 1 || variable_array[s].state == 0){	    
	    solve = deduce(s);
	    if(solve == 0){
		printf("%d\n",0);
		resolved = 1;
		break;
	    }
	}
    }
    if(resolved != 1){
	printf("\n%d\n",dpll()); 
    }

    return 0;
}


