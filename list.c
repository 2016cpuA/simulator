#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"
#include "instructs.h"
#define INSTR_SIZE sizeof(Instruct)
Instr_list *list_init(){
  Instr_list *instr_l;
  instr_l=(Instr_list*)malloc(sizeof(Instr_list));
  instr_l->instr=(Instruct*)malloc(INSTR_SIZE);
  instr_l->instr->opcode=INSTR_END;
  instr_l->no = 0;
  int i;
  for(i=0;i<4;i++)
    instr_l->instr->operands[i]=-1;
  instr_l->next=NULL;
  return instr_l;
}

int list_isempty(Instr_list *instr_l){
  return (instr_l->next==NULL);
}

void list_push(Instr_list *instr_l,Instruct *new_instr){
  Instr_list *new=(Instr_list*)malloc(sizeof(Instr_list));
  *new=*instr_l;
  instr_l->instr=new_instr;
  instr_l->next=new;
}

void list_display(Instr_list *instr_l){
  Instruct target;
  Instr_list *now = instr_l;
  printf("no\tinstr\top1\top2\top3\top4\n");
  if(!list_isempty(instr_l)){
    while(!list_isempty(now)){
      target=*(now->instr);
      printf("%d\t",now->no);
      print_instr(target);
      printf("\t%d\t%d\t%d\t%d\n",target.operands[0],target.operands[1],target.operands[2],target.operands[3]);
      now=now->next;
    }
  }
}

void list_free(Instr_list *instr_l){
  if(!list_isempty(instr_l)){
    list_free(instr_l->next);
  }
  free(instr_l->instr);
  free(instr_l);
}

