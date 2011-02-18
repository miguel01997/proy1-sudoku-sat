
#include <stdio.h>
#include <stdlib.h>
#define ABS(a) (a>=0?a:-a)

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
    int* array;
    int size;
} rarray_clause;

typedef struct{
    short state;
    rarray_clause pW;
    rarray_clause nW;
} variable;


//basic block of a decision queue
//tells you at which level the assignment of 
//literal occurred.
typedef struct{
  int literal;
  int level;
} decision;

decision* assign_queue;
int index_assign_queue;


int assigned_vars = 0;

int updateVariableArray(rarray_clause* W,int index)
{
    W->size += 1;

    W->array = (int*) realloc(W->array,
		(W->size)*sizeof(int));

    if(W->array == NULL)
    {
	printf("Ran out of memory.\n");
	return -1;
    }
    W->array[W->size-1] = index;
    return 0;
}

int satisfiable(clause* clause_array,variable* variable_array,
		int carray_size)
{	int i,j;
	int true_count;
	int false_count;
	int var_pos;
	true_count = 0;
	false_count = 0;
	for(i=1;i<=carray_size;i++)
	{	clause* c = &clause_array[i];
		false_count = 0;
		for(j=0;j<c->literals.size;j++)
		{   var_pos = ABS(c->literals.array[j]);
		    if(variable_array[var_pos].state == 0 
			&& c->literals.array[j] > 0)
			    false_count += 1;   			
		    if(variable_array[var_pos].state == 1 
			&& c->literals.array[j] < 0)
			    false_count += 1;    
		    //At least one variable is true
		    if(variable_array[var_pos].state == 1
			&& c->literals.array[j] > 0)
			break;
		    if(variable_array[var_pos].state == 0
			&& c->literals.array[j] < 0)
			break;
		}
		if(j < c->literals.size)
		    true_count += 1;
		//one clause is completely false
		if(false_count == c->literals.size)
		    return 0;
		
	}	
	if(true_count == carray_size)
	    return 1;

	return -1;
}


//Returns first literal(variable) if it is pure.
//If no pure literal is found, zero is returned.
int pureLiteral(clause* clause_array,variable* variable_array,int carray_size)
{	int i,j,k,l,m,p;
	int not_pure;
	//Grab a clause
	for(i=1;i<carray_size;i++)
	{//Grab a literal
		int size_clause_i = clause_array[i].literals.size;
		for(j=0;j<size_clause_i;j++)
		{	not_pure = 0;
			l = clause_array[i].literals.array[j];
			//Compare with other clauses
			for(k=i+1;k<carray_size;k++)
			{//Grab another literal from another clause
				int size_clause_k = clause_array[k].literals.size;
				for(m=0;m<size_clause_k;m++)
				{	p = clause_array[k].literals.array[m];
					if(p == -l)
						not_pure = 1;
				}
			}
			if(not_pure == 0)
			{   int pp = ABS(p);
			    if(variable_array[pp].state == -1)
				return p;
			}
		}
	}
	return 0;
}

int pureLiteralAssign(variable* variable_array, int literal)
{   if(literal > 0) 
	variable_array[literal].state = 1;
    else
	variable_array[-literal].state = 0; 
}

int chooseNextLiteral(variable* variable_array,int varray_size)
{   int i;
    for(i=1;i<=varray_size;i++)
	if(variable_array[i].state == -1)
	    return i;
}

void printAssignments(variable* variable_array,int varray_size)
{   int i;
  for(i=1;i <= varray_size;i++){
      if(variable_array[i].state ==0){
        printf("-%d ", i);
      } else if(variable_array[i].state == 1) {
        printf("%d ", i);
      }
  }

}
    
//pos is always positive.
int updateWatchedLiterals(variable* variable_array, int pos,
			int v,clause* clause_array,int level)
{   rarray_clause* list;
  int i,flag_true,flag_update;


  if(v == 1)   
    list = &variable_array[pos].nW;
  else
    list = &variable_array[pos].pW;
  

  //  printf("Updating %d:\n", pos);
  for(i=0;i<list->size;i++)
    {	
      if(list->array[i] == 0) 
        continue;

      //      printf("\tmov literals in clause: %d\n", i);
      clause* c = &clause_array[list->array[i]];
      int j;
      int fc = 0;
      int state_w1 = variable_array[ABS(c->literals.array[c->w_1_i])].state;
      flag_true = 0;	
      

      for(j=0;j<c->literals.size;j++)
	{   int lit = c->literals.array[j];
          int lit_abs = ABS(lit);
          int state_lit = variable_array[lit_abs].state;
      

          if((lit > 0 && state_lit == 1) || (lit < 0 && state_lit == 0))
	    {	
              flag_true = 1;
              break;
	    } 
          else if(state_lit == -1 && j != c->w_1_i && j != c->w_2_i)
            {
              //          printf("\t\tnew centinel: %d\n", lit);
              if(state_w1 == -1)
                c->w_2_i = j;
              else
                c->w_1_i = j; 
              
              if(lit > 0)
		{  
                   rarray_clause* modpW = &variable_array[lit_abs].pW; 
                   updateVariableArray(modpW,list->array[i]);
		}
              else
		{   
                  rarray_clause* modnW = &variable_array[lit_abs].nW;
                  updateVariableArray(modnW,list->array[i]);
      		}	
              
              //the "pos" literal is no longer watched in the ith clause
              list->array[i] = 0;
              
              //found a replacement to be watched, go on to next clause
              continue;
	    } 
          else if((lit > 0 && state_lit == 0) || (lit < 0 && state_lit == 1))
            {
              fc++;
            }
	} //here ends the loop that cicles through the clause
      
      if(fc == c->literals.size - 1 && flag_true != 1)
        {   int w1 = c->literals.array[c->w_1_i];
          int w2 = c->literals.array[c->w_2_i];
          int abs_w1 = ABS(w1);
          int abs_w2 = ABS(w2);
          
          int which_val;
          int which_watched;
          int which_watched_abs;
          if(state_w1 == -1){
            which_watched = w1;
            which_watched_abs = abs_w1;
          } else {
            which_watched = w2;
            which_watched_abs = abs_w2;
          }
          
          
      
          which_val = (which_watched>0)? 1: 0;
          variable_array[which_watched_abs].state = which_val;
          assign_queue[index_assign_queue].literal= which_watched_abs;
          assign_queue[index_assign_queue].level = level;
          index_assign_queue++;

          assigned_vars++;
          if(updateWatchedLiterals(variable_array,which_watched_abs,which_val,clause_array,level) == 0)
            {
              return 0;   
            }
          
        } 
      else if(fc == c->literals.size && flag_true != 1)
        {
          return 0;
        }
      
    }
  return 1;
}

void undoAssignment(int level, variable* variable_array){
  int i;
   for(i=index_assign_queue-1; i>=0; i--){
    if(assign_queue[i].level >= level){
      variable_array[assign_queue[i].literal].state = -1;
      index_assign_queue--;
      assigned_vars--;
    }
    
    }

}
int DPLL(clause* clause_array,int carray_size,
         variable* variable_array,int varray_size,int hint, int level)
{	
	int unit_clause,pure_literal,i,j;
	int sat_res;



	sat_res = satisfiable(clause_array, variable_array, carray_size);

          if(sat_res == 1){
		printAssignments(variable_array,varray_size);
            return 1;
          } else if(sat_res == 0)  {
            return 0;
          }
   
	if(updateWatchedLiterals(variable_array,hint,1,clause_array, level) == 0)
          {  

            variable_array[hint].state = 0;

            undoAssignment(level, variable_array);
            
            if(updateWatchedLiterals(variable_array,hint,0,clause_array, level) == 0)
              {	variable_array[hint].state = -1;
                //undo all assignments with level  >= than mine
                assigned_vars--;
                undoAssignment(level, variable_array);

                
                return 0;
              }		
            if(DPLL(clause_array,carray_size,variable_array,varray_size,hint, level+1) == 0)
              {	variable_array[hint].state = -1;
                assigned_vars--;
                //undo all assignments with level >= than mine
                undoAssignment(level, variable_array);
		return 0;
              }
	    return 1;
          }

	i = chooseNextLiteral(variable_array,varray_size);

	variable_array[i].state = 1;
        assigned_vars++;
	
	if(DPLL(clause_array,carray_size,variable_array,varray_size,i, level+1) == 1)
	    return 1;
	else
	{   variable_array[i].state = 0;

          undoAssignment(level, variable_array);
          if(updateWatchedLiterals(variable_array,i,0,clause_array, level) == 0)
	    {   variable_array[i].state = -1;

              //undo all assignments with level >= than mine
              assigned_vars--;
              undoAssignment(level, variable_array);
              return 0;		
	    }
	}
	if(DPLL(clause_array,carray_size,variable_array,varray_size,i, level+1) == 0)
	{   variable_array[i].state = -1;
          assigned_vars--;
          //undo all assignments with level >= than mine
          undoAssignment(level, variable_array);
	    return 0;
	}
	
	return 1;
}


int main(void){

    int V,C;
    scanf("p cnf %d%d",&V,&C);
    //To count from 1 to N.
    clause clause_array[C+1];
    variable variable_array[V+1];
    int buffer_variables[V];
    int v;
    int i;
    int I;
    int varcount;

    int k;
    for(k = 1; k<= V; k++){
      variable_array[k].pW.array = 0;
      variable_array[k].nW.array = 0;
      variable_array[k].pW.size = 0;
      variable_array[k].nW.size = 0;
      variable_array[k].state = -1;
    }

    I = 1;
    varcount = 0;
      
    while(scanf("%d",&v) != EOF)
      {	if(v == 0 && varcount == 1){
          variable_array[ABS(buffer_variables[0])].state = (buffer_variables[0] > 0)? 1 : 0;
          varcount = 0;
          assigned_vars++;
          C--;
        } else if(v==0)  
	{   
	    int k;
	    int w1,w2;
	     
	    clause_array[I].literals.array = malloc(varcount*sizeof(int));
	    if(clause_array[I].literals.array == NULL)
	    {	printf("Oops!\n");
		return 0;
	    }
	    clause_array[I].literals.size = varcount;
	    
	    for(k = 0;k<varcount;k++)
		clause_array[I].literals.array[k] = buffer_variables[k];
	    clause_array[I].w_1_i = 0;
	    clause_array[I].w_2_i = varcount - 1;

	    w1 = clause_array[I].literals.array[clause_array[I].w_1_i];
            w2 = clause_array[I].literals.array[clause_array[I].w_2_i];

	    varcount = 0;

	    
	    if( w1 > 0)
		{   if(updateVariableArray(&(variable_array[w1].pW),
				       I) == -1)
		    return -1;
		}
		else
		{   if(updateVariableArray(&(variable_array[-w1].nW),
				       I) == -1)
		    return -1;
		}
		if(w2 > 0)
		{	if(updateVariableArray(&(variable_array[w2].pW),
				       I) == -1)
			return -1;
		}
		else
		{	if(updateVariableArray(&(variable_array[-w2].nW),
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
   
    assign_queue = malloc((V)*sizeof(decision));
    index_assign_queue = 0;
    if(assign_queue == NULL)
    {	printf("Out of memory.\n");
	return 0;
    }

    variable_array[0].state = 1;
    assigned_vars++;
    printf("\n%d\n",DPLL(clause_array,C,variable_array,V,0,0)); 
    return 0;
}


      
