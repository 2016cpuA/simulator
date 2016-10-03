#include <stdlib.h>
#include "simulator.h"
#include "instructs.h"
#define INSTR_SIZE sizeof(Instruct)
void list_init(Instr_list *instr_l){
  instr_l->instr=(Instr_list*)malloc(INSTR_SIZE);
  instr_l->instr->opcode=INSTR_END;
  int i;
  for(i=0;i<4;i++)
    operands[i]=-1;
  instr_l->next=NULL;
}

int list_isempty(Instr_list *instr_l){
  return (instr_l->next==NULL);
}

void list_push(Instr_list *instr_l,Instruct *new_instr){
  Instr_list *new=(Instr_list*)malloc(sizeof(Instr_list));
  new->instr=new_instr;
  new->next=instr_l;
  instr_l=new;
}

void list_free(Instr_list *instr_l){
  if(!list_isempty(instr_l)){
    list_free(instr_l->next);
  }
  free(instr_l->instr);
  free(instr_l);
}

