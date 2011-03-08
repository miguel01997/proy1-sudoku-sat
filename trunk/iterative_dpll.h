#define MAXSIZELEARNEDCLAUSE 20
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
int d,C,V,literal_count;
//Variable de decision actual
variable* x;
//Nodo de decision actual
node decision_variable;
clause conflict_clause;
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
    decision_variable.toggled = 0;
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
 *         1 En caso de satisfacer la instancia
 *        -1 En otro caso
 *
*/
int deduce(int assigned){
    int i,flag_true,flag_update,value_assigned;
    int j,fc,flag_true,which_val,which_watched;
    int which_watched_abs,w1,w2,true_count;

    true_count = 0;

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
		true_count += 1;
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
	    decision_variable.implied_vars[decision_variable.implied_vars.size] = assigned;
	    decision_variable.implied_vars.size += 1;
	    
	    if(deduce(assigned) == 0){
		return 0;   
            }
        }
	else if(fc == c->literals.size && flag_true != 1){
	    igraph[0].decision_level = d;
	    igraph[0].implicant_clause = clause_list->array[i];
	    return 0;
        }	
    }
    if(true_count == clause_list->size)
	return 1; 
    
    return -1; 
}

/**
 * Calcula el nivel del backtracking (backjumping en el caso 
 * no cronologico). Realiza aprendizaje de clausulas para limitar 
 * el espacio de busqueda.
 *
 * @return Nivel de backtracking
 *
*/
int analyze_conflict(){	
    int blevel;
    conflict_clause.tag = 0;
    conflict_clause.w_1_i = 0;
    conflict_clause.w_2_i = 0;
    conflict_clause.literals.size = 0;
    create_conflict_induced_clause(0);
    conflict_clause.tag = (conflict_clause.literals.size 
			    > MAXSIZELEARNEDCLAUSE ? 1:0);
    clause_array[T] = conflict_clause;
    T += 1;
    blevel = compute_max_decision_level(conflict_clause);
    erase();
    return blevel;
}

/**
 * Crea la clausula de conflicto buscando las raices del
 * grafo de implicacion.
 *
 * @param index Indice de la variable de un nodo del grafo
 *              de implicacion
 *
 * @return Clausula de conflicto
 *
*/
void create_conflict_induced_clause(int index){
    int j,k,rep_flag;
    clause c;
    rep_flag = 0
    nodo m = igraph[index];
    if(m.implicant_clause == -1){
	for(j=0;j<conflict_clause.literals.size;j++){
	    if(index == conflict_clause.literals[j]){
		rep_flag = 1;
		break;
	    }
	}
	if(rep_flag != 1){
	    conflict_clause.literals.size += 1;
	    conflict_clause.literals.array = realloc(conflict_clause.literals.array,literal_count);
	    if(conflict_clause.literals.array == NULL){
		printf("Out of memory. Sorry.");
		exit(1);
	    }
	}
    }
    else{
	c = clause_array[n.implicant_clause];
	for(k=0;k<c.literals.size;k++){
	    create_conflict_induced_clause(c.literals.array[k]);
	}	
    }
}



/**
 * Retorna el indice de la variable en el arreglo de variables
 * que representa el nodo al cual se hizo backjumping.
 *
 * @param blevel El nivel de backjumping escogido por analyze_conflict.
 *
 * @return El indice de la variable
 *
*/
int backtrack(int blevel){
    int i;
    node n;
    d = blevel;
    while(true){
	n = pop();
	for(i=0;i<n.implied_vars.size;i++){
	    variable_array[ABS(n.implied_vars[i])].state = -1;
	}
	n.implied_vars.size = 0;
	if(n.decision_level == blevel){
	    variable_array[ABS(n.variable)] = (variable_array[ABS(n.variable)] == 1?0:1);
	    n.toggled = 1;
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
	
	int status,blevel,a;
	clause_array = c_a;
	variable_array = v_a;
	igraph = ig;
	c = C;
	v = V;
	T = C + 1;
	while(1){
	    a = decide_next_branch();
	    while(1){
		status = deduce(a);
		if(status == 0){
		    blevel = analyze_conflict();
		    if(blevel == 0)
			return 0;
		    else
			a = backtrack(blevel); 
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





