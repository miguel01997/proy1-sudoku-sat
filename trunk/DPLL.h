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

//Returns position of first unit clause.If no unit 
//clause is found, return 0.
int unitClause(clause* clause_array,int carray_size)
{	int i;
	for(i=1;i<=carray_size;i++)
	{	if(clause_array[i].w_1_i == clause[i].w_2_i)
			return i;	
	}
	return 0;
}


//Returns first literal(variable) if it is pure.
//If no pure literal is found, zero is returned.
int pureLiteral(clause* clause_array,int carray_size)
{	int i,j,k,l,m,p;
	int not_pure;
	//Grab a clause
	for(i=1;i<clause_array[i];i++)
	{//Grab a literal
		int size_clause_i = clause_array[i].literals.size;
		for(j=0;j<size_clause_i;j++)
		{	not_pure = 0;
			l = clause_array[i].literals.array[j];
			//Compare with other clauses
			for(k=i+1;k<carray_size;k++)
			{//Grab another literal from another clause
				int size_clause_k = clause.array[k].literals.size;
				for(m=0;m<size_clause_k;m++)
				{	p = clause_array[k].literals.array[m];
					if(p == -l)
						not_pure = 1;
				}
			}
			if(not_pure == 0)
				return p;
		}
	}
	return 0;
}
