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
int d;
//Variable de decision actual
variable x;

int dpll (clause* clause_array, variable* variable_array,
	    igraph_node* igraph, int C, int V){
	
	int status;
	while(1){
	    if(decide_next_branch(d) == 1)
		return 1;
	    while(1){
		
	    }  
	}
}



