#ifndef LIST_H

#define LIST_H

#include "simulator.h"
#include "instructs.h"

Instr_list *list_init();
int list_isempty(Instr_list *instr_l);
void list_push(Instr_list *instr_l,Instruct *new_instr);
void list_free(Instr_list *instr_l);
#endif
