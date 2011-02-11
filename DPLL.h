#include <stdlib.h>

int satisfiable(clause* clause_array,variable* variable_array,
		int carray_size)
{	int i,j;
	for(i=1;i<=carray_size;i++)
	{	clause c = clause_array[i];
		for(j=0;j<c.literals.size;c++)
		{	int var = c.literals.array[j];
			if(variable_array[var].state == 0)
				return 0;
		}		
	}
	return 1;
}

//Returns position of unit clause.If no unit clause is found,
//return 0.
int unitClause(clause* clause_array,int carray_size)
{	int i;
	for(i=1;i<=carray_size;i++)
	{	if(clause_array[i].w_1_i == clause[i].w_2_i)
			return i;	
	}
	return 0;
}

int pureLiteral(clause* clause_array,int carray_size)
{	int i,j,k,l,m,p;
	//Grab a clause
	for(i=1;i<clause_array[i];i++)
	{//Grab a literal
		int size_clause_i = clause_array[i].array.size;
		for(j=0;j<size_clause_i;j++)		
	}
}
