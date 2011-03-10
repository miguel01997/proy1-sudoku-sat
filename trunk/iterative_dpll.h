#define MAXSIZELEARNEDCLAUSE 20
#define ABS(a) (a>0 ? a : -a)
#include <stdlib.h>
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
int d,C,V,T,literal_count;
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
 * @return -1 si no hay mas variables para escoger
 *         La posicion en la lista de variables de la
 *         variable escogida en caso contrario.
 *
*/
int decide_next_branch(){
    int i,j,k,literal,literal_state,assigned;
    int state_w1;
    assigned = -1;
    clause* c;
    for(i=1;i<=V;i++)
	if(variable_array[i].state == -1){
	    assigned = i;
	    break;
	}
    
    if(assigned == -1)
	return -1;
    //Asignar variable
    variable_array[assigned].state = 1; 
    //Crear nodo raiz del grafo de implicacion
    igraph[assigned].decision_level = d;
    igraph[assigned].implicant_clause = -1; 
    return assigned;
}

/**
 * Realiza propagacion de clausulas unitarias (BCP) recursivamente.
 *
 * @param assigned Nodo de decision
 *
 * @return 0 En caso de conflicto
 *         1 En cualquier caso
 *
*/
int deduce(int assigned){
    int i,flag_true,flag_update,value_assigned;
    int j,k,fc,state_w1,which_val,which_watched;
    int which_watched_abs,w1,w2,true_count;
    int literal, literal_state,abs_w1,abs_w2;

    true_count = 0;
 
    clause* c;
    rarray_clause* clause_list;
   
    value_assigned = variable_array[assigned].state;
    
    clause_list = (value_assigned == 1 ? &(variable_array[assigned].nW):
					 &(variable_array[assigned].pW));

    for(i=0;i<clause_list->size;i++){	 


	if(clause_list->array[i] == 0) 
	    continue;
	fc = 0;
	flag_true = 0;
	c = &clause_array[clause_list->array[i]];
	state_w1 = variable_array[ABS(c->literals.array[c->w_1_i])].state;

	for(k=0;k<c->literals.size;k++){
	    literal = c->literals.array[k];
	    literal_state = variable_array[ABS(literal)].state;
	    //Se puede mover el literal observado.
	    if(literal_state == -1 && k != c->w_1_i && k != c->w_2_i){
		if(state_w1 == -1)
		    c->w_2_i = k;
		else
		    c->w_1_i = k;	
		if(literal > 0){

		    addClause(&(variable_array[ABS(literal)].pW),clause_list->array[i]);
		}
		else{
		    addClause(&(variable_array[ABS(literal)].nW),clause_list->array[i]);
		}
		clause_list->array[i] = 0;
		continue;
	    }
	    else if((literal > 0 && literal_state == 1) || (literal < 0 && literal_state == 0)){	
		flag_true = 1;
		break;
	    } 
	    else if((literal > 0 && literal_state == 0) || (literal < 0 && literal_state == 1)){
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
	    decision_variable.implied_vars.array[decision_variable.implied_vars.size] = assigned; 
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
   
    return 1; 
}
/**
 * Crea la clausula de conflicto buscando las raices del
 * grafo de implicacion.
 *
 * @param index Indice de la variable de un nodo del grafo
 *              de implicacion
 *
*/
void create_conflict_induced_clause(int index){
    int j,k,rep_flag;
    clause c;
    rep_flag = 0;
    igraph_node m = igraph[ABS(index)];
    if(m.implicant_clause == -1){
	for(j=0;j<conflict_clause.literals.size;j++){
          if(-index == conflict_clause.literals.array[j]){
		rep_flag = 1;
		break;
	    }
	}
	if(rep_flag != 1){
	    conflict_clause.literals.size += 1;
	    conflict_clause.literals.array = realloc(conflict_clause.literals.array,conflict_clause.literals.size*sizeof(int));
	    if(conflict_clause.literals.array == NULL){
		printf("Out of memory. Sorry.");
		exit(1);
	    }
	    conflict_clause.literals.array[conflict_clause.literals.size-1] = -index;
	}
    }
    else {
	c = clause_array[m.implicant_clause];
	for(k=0;k<c.literals.size;k++){
	    if(ABS(c.literals.array[k]) != ABS(index))
		create_conflict_induced_clause(c.literals.array[k]);
	}	
    }
}

/**
 * Calcula el nivel de backjumping (el nivel de decision mas alto 
 * de las variables predecesoras a un conflicto), dado una clausula 
 * de conflicto. Calcula las posiciones en la clausula de las dos
 * variables con mayor nivel de decision. Estos seran los nuevos
 * literales observados.
 *
 * @param conflict_clause Clausula de conflicto
 *
 * @param blevel Nivel de backjumping
 *
 * @param w1 Posicion de la variable con mas alto nivel de decision.
 *
 * @param w2 Posicion de la variable con el segundo mas alto nivel
 *           de decision.
 *
*/
void compute_max_decision_level(clause conflict_clause,int* blevel, int* w1,int* w2){
    igraph_node p;
    int j,q;
    *blevel = -1;
    *w1 = -1;
    *w2 = -1;
    for(j=0;j<conflict_clause.literals.size;j++){
	p = igraph[ABS(conflict_clause.literals.array[j])];
	if(p.decision_level >= *blevel){
	    *w2 = *w1;
	    *blevel = p.decision_level;
	    *w1 = j;
	    q = conflict_clause.literals.array[j];
	}	
    }
    //Hemos llegado a la raiz del arbol y ya fue toggled.
    if(*blevel == 1 && variable_array[q].toggled == 1){
	*blevel = 0;
    }
}

/**
 * Compacta el arreglo de clausulas.
*/
void compaq(){
    int i,shift;
    shift = 0;
    for(i=C+1;i<T;i++){
	if(clause_array[i].literals.size == 0){
	    shift += 1;
	}
	else if(shift != 0){
	    clause_array[i] = clause_array[i+shift];
	}
    }
    T -= shift;
}

/**
 * Borra las asignaciones implicadas en el nivel actual. 
 * Elimina clausulas aprendidas muy costosas.
*/

void erase(){
    int j,k;
    for(k=1;k<=V;k++){
      if(igraph[k].decision_level == d){
        igraph[k].decision_level = -1;
        igraph[k].implicant_clause = -1;
      }
    }
    for(j=C+1;j<T;j++){
      if(clause_array[j].tag == 1){
        if(variable_array[clause_array[j].literals.array[clause_array[j].w_1_i]].state == -1 &&
           variable_array[clause_array[j].literals.array[clause_array[j].w_2_i]].state == -1 ){
          clause_array[j].literals.size = 0;
        }
      }
    }
    compaq(); 
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
    int blevel,w1,w2;
    rarray_clause* wlist1;
    rarray_clause* wlist2;
    conflict_clause.tag = 0;
    conflict_clause.w_1_i = 0;
    conflict_clause.w_2_i = 0;
    conflict_clause.literals.size = 0;
    conflict_clause.literals.array = NULL;
    create_conflict_induced_clause(0);
  
    conflict_clause.tag = (conflict_clause.literals.size 
			    > MAXSIZELEARNEDCLAUSE ? 1:0);
    compute_max_decision_level(conflict_clause,&blevel,&w1,&w2);
    int r;
    printf("learned clause: ");
    for(r = 0; r< conflict_clause.literals.size; r++){
      printf("(%d,%d)   ",conflict_clause.literals.array[r], igraph[conflict_clause.literals.array[r]].decision_level);
    }
    printf("\n");
    printf("w1:%d   w2:%d\n",w1,w2);
    printf("blevel: %d\n", blevel);

    //Mantenimiento de literales observados
    if(blevel == 0)
	return 0;
    if(conflict_clause.literals.array[w1] > 0)
      wlist1 = &(variable_array[ABS(conflict_clause.literals.array[w1])].pW);
    else
      wlist1 = &(variable_array[ABS(conflict_clause.literals.array[w1])].nW);

    if(conflict_clause.literals.array[w2] > 0)
      wlist2 = &(variable_array[ABS(conflict_clause.literals.array[w2])].pW);
    else
      wlist2 = &(variable_array[ABS(conflict_clause.literals.array[w2])].nW);

     erase();
    //No considerar clausulas unitarias de conflicto
    if(conflict_clause.literals.size > 1 && T <= 2*C){
	addClause(wlist1,T);
	addClause(wlist2,T);
	conflict_clause.w_1_i = w1;
	conflict_clause.w_2_i = w2; 
	clause_array[T] = conflict_clause;
	T += 1;
    }

    return blevel;
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
    //Cambiar nivel de decision actual al nivel de backjumping
    d = blevel;
    printf("going to bt to %d\n", d);
    while(1){
  	n = pop();
	for(i=0;i<n.implied_vars.size;i++){
	    variable_array[ABS(n.implied_vars.array[i])].state = -1;
	}

	n.implied_vars.size = 0;
	if(n.decision_level == blevel){
	    variable_array[ABS(n.variable)].state = (variable_array[ABS(n.variable)].state == 1?0:1);
	    variable_array[ABS(n.variable)].toggled = 1;
	    push(n);
	    return n.variable;
	}
	else{
	    variable_array[ABS(n.variable)].state = -1;
	}
    }


    return n.variable;
}

/**
 * Initializa el nodo decision_variable.Ocurre con cada nuevo decision.
 *
 * @param var Nombre de la nueva variable de decision.
 *
*/
void initializeDecisionVariable(int var){
    //Crear nodo de decision
  printf("teta d_l: %d   i_c: %d\n",     igraph[ABS(var)].decision_level,     igraph[ABS(var)].implicant_clause);
  //    igraph[ABS(var)].decision_level = d;

  //igraph[ABS(var)].implicant_clause = -1;

    decision_variable.variable = var;
    decision_variable.decision_level = d;
    printf("var.d_l: %d\n", d);
    decision_variable.implied_vars.size = 0;
    decision_variable.implied_vars.array = malloc(V*sizeof(int));
    if(decision_variable.implied_vars.array == NULL){
	printf("Out of memory.Sorry.\n");
	exit(1);
    }
}

int dpll (){
	int status,blevel,a;
	/*while(1){
	    a = decide_next_branch();
	    printf("root: %d\n",a);
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
	}*/

	while(1){
	    a = decide_next_branch();
	    initializeDecisionVariable(a);
	    if(a != -1){
              printf("entro con %d\n",a);
		while(deduce(a) == 0){

		    push(decision_variable);
		    blevel = analyze_conflict();

		    if(blevel == 0)
			return 0;
		    else{
			a = backtrack(blevel);
                        printf("blevel: %d\na:%d\n",blevel,a);
                        free(decision_variable.implied_vars.array);
			initializeDecisionVariable(a);

		    }
		}
		push(decision_variable);
	    }
	    else{
		return 1;
	    }
	    d += 1;
	}
}





