#ifndef SIM_H

#define SIM_H
#define MEMSIZE 65536
#define REGS 16
#define FREGS 16
struct simulator{
  char mem[MEMSIZE];
  int reg[REGS];
  float freg[FREGS];
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
