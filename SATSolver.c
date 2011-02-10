#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 65536

typedef struct{
	int* array;
	int size;
} rarray_variable;

typedef struct{
    int w_1_i;
    int w_2_i;
    rarray_variable literals;
} clause;

typedef struct{
    clause** array;
    int size;
} rarray_clause;

typedef struct{
    rarray_clause pW;
    rarray_clause nW;
} variable;


int updateVariableArray(rarray_clause* W,clause* n)
{
    W->size += 1;
    clause **_tmp = (clause**) realloc(W->array,
		(W->size)*sizeof(clause*));
    if(!_tmp)
    {
	printf("Ran out of memory.\n");
	return -1;
    }
    W->array = _tmp;
    W->array[W->size-1] = n;
    return 0;
}

 int main(void){
    /*FILE* fp;
    char header[50];
    char line_buffer[MAX_LINE_LENGTH];
    char word_buffer[MAX_WORD_LENGTH];
    char* var, wp;
    int *clause_buffer;
    int i, num_var;
    int so_far = 0;*/

    int V,C;
    scanf("p cnf %d%d",&V,&C);
    //To count from 1 to N.
    clause clause_array[C+1];
    variable variable_array[V+1];
    int v;
    int i;
    int I;
    clause* n;
    if((n = malloc(sizeof(clause))) == NULL)
    {	printf("Out of heap memory.\n");
	return -1;
    }
    I = 1;
    n->literals.array = NULL;
    n->literals.size = 0;
    //Variable array is initialized by default.
    //Clause construction.
     
    while(scanf("%d",&v) != EOF)
    {	if(v == 0) 
	{   //Actualize variable array.
	    
	    int w1 = n->literals.array[0];
	    int w2 = n->literals.array[n->literals.size-1];

	    n->w_1_i = w1;
	    n->w_2_i = w2;	    	

	    if(w1 > 0)
	    {	if(updateVariableArray(&variable_array[w1].pW,n) == -1)
		    return -1;
	    }
	    else
	    {	if(updateVariableArray(&variable_array[-w1].nW,n) == -1)
		    return -1;
	    }
	    if(w2 > 0)
	    {	if(updateVariableArray(&variable_array[w2].pW,n) == -1)
		    return -1;
	    }
	    else
	    {	if(updateVariableArray(&variable_array[-w2].nW,n) == -1)
		    return -1;
	    }
	    //insert clause into array. Start new clause.
	    
	    clause_array[I] = *n;
	    I++;
	    clause* p;
	    if((p = malloc(sizeof(clause))) == NULL)
	    {	printf("Out of heap memory\n");
		return -1;
	    }	
	    p->w_1_i = 0;
	    p->w_2_i = 0;
	    p->literals.array = NULL;
	    p->literals.size = 0;
	    n = p;
	}  
	// v is a value of a variable in a clause.
	// I must add it to the current clause.			
	else
	{   n->literals.size += 1; 
	    int *_tmp = (int*) realloc(n->literals.array,
			n->literals.size*sizeof(int));
	    if(!_tmp)
	    {
		printf("Ran out of memory.\n");
		return -1;
	    }
	    n->literals.array = _tmp;
	    n->literals.array[n->literals.size-1] = v;

	}
    } 

int j;
for(j=1;j<=C;j++)
{   printf("Clause %d:\n",j);
    printf("\tw_1_i: %d w_2_i: %d\n",clause_array[j].w_1_i,clause_array[j].w_2_i);
    int k;
    printf("Literals: ");
    for(k=0;k<clause_array[j].literals.size;k++)
	printf("%d ",clause_array[j].literals.array[k]);	
    printf("Number of literals %d\n",clause_array[j].literals.size);
    printf("\n");
}	


    //create the first clause structure
    
    /*so_far = 1;          
    
    while(fgets(buffer, MAX_LINE_LENGTH, fp)){

      
      if(buffer[0]=='c'){ //ignore comments in input file
        continue;
      }
      
      var = buffer;
      
      //skip through spaces and such
      while(*var != 0 && (*var == '\t' || *var == '\r' || *var == ' ')){
        var++;
      }  
      
      if(*var == '0'){
        //this marks the end of a clause
        
        //create a new empty clause
	
      }

      wp = word_buffer;

      //copy all the digits (and negation sign) to the word buffer
      while(*var != 0 && *var != '\t' && *var != '\r' && *var != ' '){
        *wp = *var;
        wp++;
        var++;
      }
      wp++;
      *wp = '\0';

      sscanf(word_buffer, "%d", &num_var); //convert the number to binary

      //add literal to clause

      //create variable and add pointer to the clause.
      //if int value is < 0 then it is negated
      //otherwise it is positive

      
      
    }

    //the EOF has been reached, all clauses and variables are ready
    */
    return 0;
 }


      
