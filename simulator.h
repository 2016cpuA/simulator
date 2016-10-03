#ifndef SIM_H

#define SIM_H
#define MEMSIZE 65536

struct simulator{
  char mem[65536];
  int reg[16];
  float freg[16];
  int cond;
  int link;
  int pc;
};

struct instruct{
  int opcode;
  int operands[4];
};

typedef struct simulator Simulator;
typedef struct instruct Instruct;

struct label{
  int label_no;
  int line;
};

struct instr_list{
  Instruct *instr;  
  struct instr_list *next;
};

typedef struct instr_list Instr_list;

Instruct instr_get(Instr_list l);
Instr_list instr_load(int fd);



#endif
