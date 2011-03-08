#include "node_stack.h"
/**
  * Determina si una instancia de SAT es satisfacible mediante
  * la aplicacion del algoritmo DPLL iterativo. 
  * 
  * @param clause_array Lista indexada de clausulas de la instancia
  *
  * @param variable_array Lista indexada de variables de la instancia
  *
  * @param igraph Arreglo de nodos del grafo de implicacion
  *
  * @param C Tamano de la lista de clausulas
  * 
  * @param V Tamano de la lista de variables y del arreglo de nodos
  *  
*/

//Nivel de decision actual
int d,C,V;
//Variable de decision actual
variable* x;
//Nodo de decision actual
node decision_variable;
clause* clause_array;
variable* variable_array;
igraph_node* igraph;

/**
 * Actualiza la lista de clausulas W con una clausula 
 * indexada index.
 *
 * @param watched_clauses La lista de clausulas
 *
 * @param index El indice donde la clausula a insertar
 *              esta indexada en el arreglo de clausulas
 *
 * @return -1 Si llamada en realloc fallo
 *          0 En caso contrario
 *
*/
int addClause(rarray_clause* watched_clauses,int index){
    watched_clauses->size += 1;
    watched_clauses->array = (int*) realloc(watched_clauses->array,
		(watched_clauses->size)*sizeof(int));
    if(watched_clauses->array == NULL)
    {
	printf("Ran out of memory.\n");
	return -1;
    }
    watched_clauses->array[watched_clauses->size-1] = index;
    return 0;
}


/**
 * Escoge la siguiente variable a ser asignada, la asigna
 * un valor.
 *
 * @return La posicion en la lista de variables de la
 *         variable escogida.
*/
int decide_next_branch(){
    int i,j,k,literal,literal_state;
    int state_w1;
    clause* c;
    for(i=1;i<=varray_size;i++)
	if(variable_array[i].state == -1){
	    assigned = i;
	    x = &variable_array[i];
	    break;
	}
    //Asignar variable
    variable_array[assigned].state = 1;
    //Crear nodo de decision
    decision_variable.variable = assigned;
    decision_variable.decision_variable = d;
    decision_variable.implied_vars.size = 0;
    decision_variable.implied_vars.array = malloc(V*sizeof(int));
    if(decision_variable.implied_vars.array == NULL){
	printf("Out of memory.Sorry.\n");
	exit(1);
    }
    //Crear nodo raiz del grafo de implicacion
    igraph[assigned].decision_level = d;
    return assigned;
}

/**
 * Realiza propagacion de clausulas unitarias (BCP) recursivamente.
 *
 * @param assigned Nodo de decision
 *
 * @return 0 En caso de conflicto
 *         1 En caso contrario
*/
int deduce(int assigned){
    int i,flag_true,flag_update,value_assigned;
    int j,fc,flag_true,which_val,which_watched;
    int which_watched_abs,w1,w2;

    clause* c;
    rarray_clause* clause_list;
    value_assigned = variable_array[assigned].state;
    clause_list = (value_assigned == 1 ? &variable_array[assigned].nW:
					 &variable_array[assigned].pW);
    for(i=0;i<clause_list->size;i++){	 

	if(clause_list->array[i] == 0) 
	    continue;
	fc = 0;
	flag_true = 0;
	c = &clause_array[clause_list->array[i]];
	state_w1 = variable_array[ABS(c->literals[c->w_1_i])].state;

	for(k=0;k<c->literals.size;k++){
	    literal = c->literals.array[k];
	    literal_state = variable_array[ABS(literal)].state;
	    if(literal_state == -1 && k != c->w_1_i && k != c->w_2_i){
		if(state_w1 == -1)
		    c->w_2_i = k;
		else
		    c->w_1_i = k;	
		if(literal > 0)
		    addClause(&variable_array[ABS(literal)].pW,clause_list->array[i]);
		else
		    addClause(&variable_array[ABS(literal)].nW,clause_list->array[i]);
		clause_list->array[i] = 0;
		continue;
	    }
	    else if((literal > 0 && state_literal == 1) || (literal < 0 && state_literal == 0)){	
		flag_true = 1;
		break;
	    } 
	    else if((lit > 0 && state_lit == 0) || (lit < 0 && state_lit == 1)){
		fc++;
            }
	}
	
	if(fc == c->literals.size - 1 && flag_true != 1){   
	    w1 = c->literals.array[c->w_1_i];
	    w2 = c->literals.array[c->w_2_i];
	    abs_w1 = ABS(w1);
	    abs_w2 = ABS(w2);
          
	    if(state_w1 == -1){
		which_watched = w1;
		assigned = abs_w1;
	    } else {
		which_watched = w2;
		assigned = abs_w2;
	    }
          
	    which_val = (which_watched>0)? 1: 0;
	    variable_array[assigned].state = which_val;
	    igraph[assigned].decision_level = d;
	    igraph[assigned].implicant_clause = clause_list->array[i];
	    nodo.implied_vars[nodo.implied_vars.size] = assigned;
	    nodo.implied_vars.size += 1;
	    
	    if(deduce(assigned) == 0){
		return 0;   
            }
          
        }
	else if(fc == c->literals.size && flag_true != 1){
	    return 0;
        }	
    }
    return 1;  
}

/**
 * Retorna el indice de la variable en el arreglo de variables
 * que representa el nodo al cual se hizo backjumping.
 *
 * @return El indice de la variable
 *
*/
int backtrack(){
    int i;
    node n;
    while(true){
	n = pop();
	for(i=0;i<n.implied_vars.size;i++){
	    variable_array[ABS(n.implied_vars[i])].state = -1;
	}
	n.implied_vars.size = 0;
	if(n.decision_level == d){
	    variable_array[ABS(n.variable)] = -1;
	    push(n);
	    break;
	}
	else{
	    variable_array[ABS(n.variable)].state = -1;
	}
    }
}

int dpll (clause* c_a, variable* variable_array,
	    igraph_node* ig, int c, int v){
	
	int status,blevel;
	clause_array = c_a;
	variable_array = v_a;
	igraph = ig;
	c = C;
	v = V;`
	while(1){
	    decide_next_branch();
	    while(1){
		status = deduce();
		if(status == 0){
		    blevel = analyze_conflict();
		    if(blevel == 0)
			return 0;
		    else
			backtrack(blevel); 
		}
		else if(status == 1)
		    return 1;		
		else{
		    d += 1;
		    break;
		}
	    }  
	}
}





