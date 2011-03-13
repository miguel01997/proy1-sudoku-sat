#define MAXSIZELEARNEDCLAUSE 20
#define ABS(a) (a>0 ? a : -a)
#include <stdio.h>
#include <stdlib.h>
#include "data_structures.h"
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
int d,C,V,T,D;
//Nodo de decision actual
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
    for(i=1;i<=V;i++){
	if(variable_array[i].state == -1){
	    assigned = i;
	    break;
	}
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

    if(value_assigned == 1){
	clause_list = &variable_array[assigned].nW;
    }
    else{
	clause_list = &variable_array[assigned].pW;
    }

    for(i=0 ; i < clause_list->size ; i++){

	if(clause_list->array[i] == 0) 
	    continue;
	fc = 0;
	flag_true = 0;
	c = &clause_array[clause_list->array[i]];
	state_w1 = variable_array[ABS(c->literals.array[c->w_1_i])].state;
	int new_watched_literal = -1;

	for(k=0;k<c->literals.size;k++){
	    literal = c->literals.array[k];
	    literal_state = variable_array[ABS(literal)].state;
	    //la clausula se cumple.
	    if((literal > 0 && literal_state == 1) || (literal < 0 && literal_state == 0)){	
		flag_true = 1;
		break;
	    }
	}

	if(flag_true != 1 ){
	    for(k=0;k<c->literals.size;k++){
		literal = c->literals.array[k];
		literal_state = variable_array[ABS(literal)].state;
		//se puede mover el literal observado.
		if(literal_state == -1 && k != c->w_1_i && k != c->w_2_i){
		    new_watched_literal = k;
		    break;
		}
		//llevar la cuenta del numero de literales que falsos
		else if((literal > 0 && literal_state == 0) || (literal < 0 && literal_state == 1)){
		    fc++;
		}
	    }
	}
	//clausula unitaria, puesto que todos los literales son falsos,
	//salvo por uno, que es no asignado
	if(fc == c->literals.size - 1 && flag_true != 1){   
	    w1 = c->literals.array[c->w_1_i];
	    w2 = c->literals.array[c->w_2_i];
	    abs_w1 = ABS(w1);
	    abs_w2 = ABS(w2);


	    int org_assigned = assigned;
	    //revisa cual es el centinela libre, esa es la variable implicada
	    if(state_w1 == -1){
		which_watched = w1;
		assigned = abs_w1;
	    } else {
		which_watched = w2;
		assigned = abs_w2;
	    }

	    //que valor hay que asignarle para satisfacer la clausula
	    which_val = (which_watched>0)? 1: 0;

	    variable_array[assigned].state = which_val;
	    igraph[assigned].decision_level = d;
	    igraph[assigned].implicant_clause = clause_list->array[i];

	    //continuar el bcp
	    if(deduce(assigned) == 0){
		return 0;   
	    }
	}
	//todos los literales en la clausula son falsos
	//hay un conflicto
	else if(fc == c->literals.size){
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
	printf("(Index,toggled,ic_clause,decision_level): (%d,%d,%d,%d)\n",index,variable_array[ABS(index)].toggled,igraph[ABS(index)].implicant_clause,igraph[ABS(index)].decision_level);
	for(j=0;j<conflict_clause.literals.size;j++){
	    if(ABS(index) == ABS(conflict_clause.literals.array[j])){
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
	    if(variable_array[ABS(index)].state == 0){
		conflict_clause.literals.array[conflict_clause.literals.size-1] = ABS(index);
	    }
	    else if(variable_array[ABS(index)].state == 1){
		conflict_clause.literals.array[conflict_clause.literals.size-1] = -ABS(index);	
	    }
	}
    }
    else {
	c = clause_array[m.implicant_clause];
	for(k=0;k<c.literals.size;k++){
	    if(ABS(c.literals.array[k]) != ABS(index)){
		create_conflict_induced_clause(c.literals.array[k]);
	    }
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
    int j,q,b;
    //w1 es asociado a blevel, w2 es asociado a b.
    b = -2;
    *blevel = -2;
    *w1 = -2;
    *w2 = -2;
    for(j=0;j<conflict_clause.literals.size;j++){
	p = igraph[ABS(conflict_clause.literals.array[j])];
	if(p.decision_level > *blevel){
	    *w2 = *w1;
	    b = *blevel;
	    *blevel = p.decision_level;
	    *w1 = j;
	    q = ABS(conflict_clause.literals.array[j]);
	}
	if(p.decision_level > b && p.decision_level < *blevel){
	    b = p.decision_level;
	    *w2 = j;
	}	
    }
    //Hemos llegado a la raiz del arbol preprocesada y ya fue toggled.
    if(*blevel < D ){
	*blevel = 0;
    }
}

/**
 * Compacta el arreglo de clausulas.
 */
void compaq(){
    int i,k,shift;
    rarray_clause* clist;
    shift = 0;
    for(i=C+1;i<T;i++){
	if(clause_array[i].literals.size == 0){
	    shift += 1;
	}
	else if(shift != 0){
	    //Actualizar watchers.
	    clist = &(variable_array[ABS(clause_array[i].literals.array[clause_array[i].w_1_i])].pW);
	    for(k=0;k<clist->size;k++){
		if(clist->array[k] == i+shift){
		    clist->array[k] = i;
		    break;
		}
	    }
	    clist = &(variable_array[ABS(clause_array[i].literals.array[clause_array[i].w_1_i])].nW);
	    for(k=0;k<clist->size;k++){
		if(clist->array[k] == i+shift){
		    clist->array[k] = i;
		    break;
		}
	    }
	    clist = &(variable_array[ABS(clause_array[i].literals.array[clause_array[i].w_2_i])].pW);
	    for(k=0;k<clist->size;k++){
		if(clist->array[k] == i+shift){
		    clist->array[k] = i;
		    break;
		}
	    }
	    clist = &(variable_array[ABS(clause_array[i].literals.array[clause_array[i].w_2_i])].nW);
	    for(k=0;k<clist->size;k++){
		if(clist->array[k] == i+shift){
		    clist->array[k] = i;
		    break;
		}
	    }
	    //Mover clausula.
	    clause_array[i] = clause_array[i+shift];
	}
    }
    T -= shift;
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
	printf("(%d,%d,%d,%d)   ",conflict_clause.literals.array[r], igraph[ABS(conflict_clause.literals.array[r])].decision_level,igraph[ABS(conflict_clause.literals.array[r])].implicant_clause,variable_array[ABS(conflict_clause.literals.array[r])].state);
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

    //No considerar clausulas unitarias de conflicto

    if(conflict_clause.literals.size == 1){
	printf("HEYYYYYYYYYYY!\n");
    }
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
 * que representa el nodo al cual se hizo backjumping. Elimina
 * las clausulas muy costosas.
 *
 * @param blevel El nivel de backjumping escogido por analyze_conflict.
 *
 * @return El indice de la variable
 *
 */
int backtrack(int blevel){
    int i,j,k,w;
    rarray_clause* clist;

    //Eliminar las clausulas costosas.
    for(j=C+1;j<T;j++){
	if(clause_array[j].tag == 1){
	    if(variable_array[ABS(clause_array[j].literals.array[clause_array[j].w_1_i])].state == -1 &&
		    variable_array[ABS(clause_array[j].literals.array[clause_array[j].w_2_i])].state == -1 ){
		//Actualizar watchers.
		clist = &(variable_array[ABS(clause_array[j].literals.array[clause_array[j].w_1_i])].pW);
		for(k=0;k<clist->size;k++){
		    if(clist->array[k] == j){
			clist->array[k] = 0;
			break;
		    }
		}
		clist = &(variable_array[ABS(clause_array[j].literals.array[clause_array[j].w_1_i])].nW);
		for(k=0;k<clist->size;k++){
		    if(clist->array[k] == j){
			clist->array[k] = 0;
			break;
		    }
		}
		clist = &(variable_array[ABS(clause_array[j].literals.array[clause_array[j].w_2_i])].pW);
		for(k=0;k<clist->size;k++){
		    if(clist->array[k] == j){
			clist->array[k] = 0;
			break;
		    }
		}
		clist = &(variable_array[ABS(clause_array[j].literals.array[clause_array[j].w_2_i])].nW);
		for(k=0;k<clist->size;k++){
		    if(clist->array[k] == j){
			clist->array[k] = 0;
			break;
		    }
		}
		//Borrar clausula.
		clause_array[j].literals.size = 0;
	    }
	}
    }

    compaq();

    //Fail Driven Assertions
    if(blevel == d){
	//Borrar implicados de nivel de decision d.
	for(i = 1; i<= V;i++){
	    if(igraph[i].decision_level == d && igraph[i].implicant_clause != -1){
		//igraph[i].decision_level = -1;
		//-2 quiere decir que no tiene clausula
		//implicante asignada. Esto NO es lo mismo
		//que una variable de decision.
		igraph[i].implicant_clause = -2;
		igraph[i].decision_level = -1;
		variable_array[i].state = -1;
		//los watchers no necesitan ser actualizados.
	    }	
	}
	for(i = 1; i<= V;i++){
	    if(igraph[i].decision_level == d && igraph[i].implicant_clause == -1){
		//Fail driven assertion no es raiz, es implicado por la clausula
		//de conflicto (deducido automaticamente por deduce()).
		//Vamos a probar el manejo explicito de los FDAs.
		//En este caso, clausulas marcadas de rojo no seran eliminados.
		igraph[i].implicant_clause = -2;
		variable_array[i].state = (variable_array[i].state == 1 ? 0 : 1);
		return i;
	    }
	}
    }
    else{
	//Borrar implicados de nivel de decision d.
	for(i = 1; i<= V;i++){
	    if(igraph[i].decision_level == d){
		//igraph[i].decision_level = -1;
		//-2 quiere decir que no tiene clausula
		//implicante asignada. Esto NO es lo mismo
		//que una variable de decision.
		igraph[i].implicant_clause = -2;
		igraph[i].decision_level = -1;
		variable_array[i].state = -1;
		//los watchers no necesitan ser actualizados.
	    }
	}
	//Un nuevo nivel de decision.
	d = blevel;
	//Voy a saltar al nodo que es raiz del arbol (escoge uno)
	//y que tiene el nivel de decision mas alto (blevel).
	for(i = 1; i<= V;i++){
	    if(igraph[i].decision_level == blevel && igraph[i].implicant_clause == -1){
		//Esta variable es toggled.
		//variable_array[i].toggled = 1;
		variable_array[i].state = (variable_array[i].state == 1 ? 0 : 1);
		//Indice de la nueva variable asignada, lista para propagar.
		return i;
	    }    
	}
    }
}

/**
 * Initializa una nueva variable de decision en el grafo
 * de implicacion.
 *
 * @param var Nombre de la nueva variable de decision.
 *
 */
void initializeDecisionVariable(int var){
    igraph[var].decision_level = d;
    igraph[var].implicant_clause = -1;
}

void printAssignments()
{   int i;
    for(i=1;i <= V;i++){
	if(variable_array[i].state ==0){
	    printf("-%d ", i);
	} else if(variable_array[i].state == 1) {
	    printf("%d ", i);
	}
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
	//initializeDecisionVariable(a);
	if(a != -1){
	    printf("Variable de decision, value: %d,%d\n",a,variable_array[a].state);
	    while(deduce(a) == 0){
		blevel = analyze_conflict();
		if(blevel == 0)
		    return 0;
		else{
		    a = backtrack(blevel);
		    printf("blevel: %d dec: %d\nvalue: %d (a,imp_clause):(%d,:%d)\n",blevel,d,variable_array[a].state,a,igraph[a].implicant_clause);
		}
	    }
	}
	else{
	    printAssignments();
	    return 1;
	}
	d += 1;
    }
}
