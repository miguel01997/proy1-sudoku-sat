#include <stdio.h>

#define MAX_LINE_LENGTH 65536

typedef struct{
    int w_1_i;
    int w_2_i;
    int* literals;
} clause;

typedef struct{
    clause** pW;
    clause** nW;
} variable;

 int main(void){
   FILE* fp;
   char header[50];
   char line_buffer[MAX_LINE_LENGTH];
   char word_buffer[MAX_WORD_LENGTH];
   char* var, wp;
   int *clause_buffer;
   int V,C;
   int i, num_var;
   int so_far = 0;
    variable* VarList;
    
    fp = fopen("result.out","r");
    
    fscanf(fp,"%*s%*s%d%d",&V,&C);
    
    if((VarList = (variable*)malloc(sizeof(variable)*V))==0){
      printf("Out of memory. Too many variables.\n");
      exit(1);
    }
      
    //create the first clause structure
    
    so_far = 1;          
    
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

 }


      
