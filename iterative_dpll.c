/**********************************************************
iterative_dpll.c

@author Alex Tough (07-41604)
@author Daniel Bruzual (07-40681)

************************************************************/
#include "iterative_dpll.h"

int main(void){

    int v;
    int i;
    int I;
    int varcount;
    int k,s,x,y;
    int solve,resolved;

    scanf("p cnf %d%d",&V,&C);

    d = 1;
    int decision_levels[V+1];
    //Para contar desde 1 a N
    if((clause_array = malloc((2*C+1)*sizeof(clause))) == NULL){
	printf("Out of memory. Sorry.\n");
	exit(1);
    }

    if((variable_array = malloc((V+1)*sizeof(variable))) == NULL){
	printf("Out of memory. Sorry.\n");
	exit(1);
    }

    if((igraph = malloc((V+1)*sizeof(igraph_node))) == NULL){
	printf("Out of memory. Sorry.\n");
	exit(1);
    }

    int buffer_variables[V]; 

    //Inicializar nodo de conflicto
    igraph[0].decision_level = -1;
    igraph[0].implicant_clause = -1;
    //Inicializar variables y nodos del grafo de implicacion
    for(k = 1; k<= V; k++){
	variable_array[k].pW.array = NULL;
	variable_array[k].nW.array = NULL;
	variable_array[k].pW.size = 0;
	variable_array[k].nW.size = 0;
	variable_array[k].state = -1;
	variable_array[k].toggled = 0;

	igraph[k].decision_level = -1;
	igraph[k].implicant_clause = -2; 	
    }

    I = 1;
    varcount = 0;
      
    while(scanf("%d",&v) != EOF){	
	if(v == 0 && varcount == 1){
	    //Clausula unitaria preprocesada
	    variable_array[ABS(buffer_variables[0])].state = ((buffer_variables[0] > 0) ? 3 : -3);
	    variable_array[ABS(buffer_variables[0])].toggled = 1;
	    varcount = 0;
	    C--;
	    igraph[ABS(buffer_variables[0])].decision_level = 1;
	    igraph[ABS(buffer_variables[0])].implicant_clause = -1;
	    decision_levels[ABS(buffer_variables[0])] = d;
	    d += 1;
	    
        } else if(v==0){   
	    int k;
	    int w1,w2;
	     
	    //Initialize clause.
	    clause_array[I].literals.array = malloc(varcount*sizeof(int));
	    if(clause_array[I].literals.array == NULL){	
		printf("Out of memory.Sorry.\n");
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


	    if( w1 > 0){   
		if(addClause(&(variable_array[w1].pW),
			I) == -1){
		    return -1;
		}
	    }
	    else{   
		if(addClause(&(variable_array[-w1].nW),
			I) == -1){
		    return -1;
		}
	    }
	    if(w2 > 0){	
		if(addClause(&(variable_array[w2].pW),
			I) == -1){
		    return -1;
		}
	    }
	    else
	    {	if(addClause(&(variable_array[-w2].nW),
			I) == -1){
		    return -1;
		}
	    }
	    I++;
	}
	else {   
	    buffer_variables[varcount] = v;
	    varcount++;	    
	}

    }  
    solve = 0;
    resolved = 0;
    T = C + 1;
    D = d;

    for(s=1;s<=V;s++){
	if(variable_array[s].state == 3 || variable_array[s].state == -3){
	    variable_array[s].state = (variable_array[s].state == 3 ? 1 : 0);	

	    d = decision_levels[s];     
	    initializeDecisionVariable(s);
	    solve = deduce(s);	
	    if(solve == 0){
		printf("%d\n",0);
		resolved = 1;
		break;
	    }
	}
    }

    d = D;
    if(resolved != 1){
	printf("%d\n",dpll()); 
    }
    return 0;
}
