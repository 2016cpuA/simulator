#ifndef SIM_H

#define SIM_H
#define MEMSIZE 65536
#define REGS 32

struct simulator{
  char mem[MEMSIZE];
  int reg[REGS];
  int hi,lo;
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
  int no;
  Instruct *instr;  
  struct instr_list *next;
};

typedef struct instr_list Instr_list;

Instruct instruct_get(Instr_list l);
Instr_list instruct_load(int fd);
#endif
