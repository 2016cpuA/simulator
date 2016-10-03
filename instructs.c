#include <stdio.h>
#include <string.h>
#include "instructs.h"
#define UNKNOWN -1
#define ERROR -2
#define REG 1
#define FREG 2
#define IMMIDIATE 4
#define NONE 0

int isnum(char ch){
  return (ch>='0')&&(ch<='9');
}

int get_instr(char *name){
  if(!strcmp(name,"nop")){
    return NOP;
  }else if(!strcmp(name,"add")){
    return ADD;
  }else if(!strcmp(name,"addi")){
    return ADDI;
  }else if(!strcmp(name,"sub")){
    return SUB;
  }else if(!strcmp(name,"mul")){
    return MUL;
  }else if(!strcmp(name,"muli")){
    return MULI;
  }else if(!strcmp(name,"div")){
    return DIV;
  }else if(!strcmp(name,"load")){
    return LOAD;
  }else if(!strcmp(name,"loadi")){
    return LOADI;
  }else if(!strcmp(name,"str")){
    return STR;
  }else if(!strcmp(name,"stri")){
    return STRI;
  }else{
    printf("Warning: unknown instruction '%s'\n",name);
    return UNKNOWN;
  }
  return UNKNOWN;
}

int get_operand(char *op,int type_op){
  if(isnum(ch[0])){
    if (!(type_op&&IMMIDIATE))
      printf("Warning: wrong operand type\n"); 
    return atoi(op);
  }else if(ch[0]=='%'){
    if(ch[1]=='r'){
      if (!(type_op&&REG))
	printf("Warning: wrong operand type\n"); 
      return atoi(op+2);
    }else if(ch[1]=='f'){
      if(!(type_op&&FREG))
	printf("Warning: wrong operand type\n");
      return atoi(op+2);
    }else{
      printf("Error: unkown operand '%s'\n",op);
    }
  }else{
    printf("Error: unkown operand '%s'\n",op);
  }
  return ERROR;
}
