/**********************************************************
data_structures.h

@author Alex Tough (07-41604)
@author Daniel Bruzual ()

Implements the following data structures:
    - Variable resizeable array (rarray_variable)
    - Clause resizeable array (rarray_clause)
    - Variable (variable)
    - Clause (clause)
    - Node in implication graph (igraph_node)
************************************************************/

typedef struct{
	int* array;
	int size;
} rarray_variable;

//Clause structure
typedef struct{
    //Tag for management heuristic of learned clauses.
    //Tag = 0 -> Green.
    //Tag = 1 -> Red.
    int tag;
    //Index of first watched literal
    int w_1_i;
    //Index of second watched literal
    int w_2_i;
    //List of literals contained in the clause
    rarray_variable literals;
} clause;

typedef struct{
    int* array;
    int size;
} rarray_clause;

//Variable structure
typedef struct{
    //Value of the variable: 0 = false; 1 = true, -1 = undetermined.
    short state;
    //List of clauses where the variable is positively watched.
    rarray_clause pW;
    //List of clauses where the variable is negatively watched.
    rarray_clause nW;
} variable;

//Node of implicit backtracking graph
typedef struct{
    //Index of variable in variable array
    int variable;
    //Indicates whether the variable has been toggled.
    // toggled = 1 -> true
    // else -> false
    int toggled;
    //Decision level associated with variable.
    int decision_level;
    //List of implied variables.
    rarray_variable implied_vars;
} node;

//Node of implication graph.
typedef struct{
     //Decision level where the variable was assigned.
    //-1 when the node is initialized.
    int decision_level;
    //Number of clause that implied the variable.
    //-1 when the node is initialized.
    int implicant_clause; 
} igraph_node;
