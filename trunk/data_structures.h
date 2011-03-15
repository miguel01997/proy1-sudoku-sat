/**********************************************************
data_structures.h

@author Alex Tough (07-41604)
@author Daniel Bruzual (07-40681)

Implementa las estructuras de datos siguientes:
    - Arreglo de variables redimensional (rarray_variable)
    - Arreglo de clausulas redimensional (rarray_clause)
    - Variable (variable)
    - Clausula (clause)
    - Nodo en el grafo de implicacion (igraph_node)

************************************************************/

typedef struct{
	int* array;
	int size;
} rarray_variable;

//Estructura de clausula
typedef struct{
    //Etiqueta para el manejo de clausulas aprendidas
    //Tag = 0 -> Verde
    //Tag = 1 -> Rojo
    int tag;
    //Indice del primer literal observado
    int w_1_i;
    //Index del segundo literal observado
    int w_2_i;
    //Lista de literales contenidas en la clausula
    rarray_variable literals;
} clause;

//Arreglo redimensional de clausulas
typedef struct{
    //Lista de indices de clausulas
    int* array;
    //Tamano de la lista
    int size;
} rarray_clause;

//Variable structure
typedef struct{
    //Valor de la variable: 0 -> false, 1 -> true, -1 -> no determinado
    short state;
    //Indica si la variable ha sido cambiada de valor.
    // toggled = 1 -> true
    // si no -> false
    int toggled;
    //Lista de clausulas donde la variable esta siendo vista de manera positiva
    rarray_clause pW; 
    //Lista de clausulas donde la variable esta siendo vista de manera positiva
    rarray_clause nW;
} variable;

//Nodo del grafo de implicacion
typedef struct{
    //Nivel de decision donde la variable fue asignada
    //-1 cuando la variable esta inicializada
    int decision_level;
    //Numero de clausula que implico la variable
    //-1 cuando la variable esta inicializada
    int implicant_clause; 
} igraph_node;
