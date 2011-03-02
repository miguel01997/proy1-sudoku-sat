#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
  //remember to check if the argument was specified
  int dim = atoi(argv[0]);
  int num_vars = dim*dim*dim;
  int i, k;

  int row, col;
  while(num_vars > 0 ){
    scan("%d", &i)){;
    num_vars--;

    if(i > 0){
      row = i / (dim*dim);
      col = (i % dim*dim)/dim;
      k = (i % dim*dim)%dim;
      printf("%d  ", k);
      if(col == dim){
	printf("\n");
      }
    }
    
  }

}
