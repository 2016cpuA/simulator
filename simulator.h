#ifndef SIM_H

#define SIM_H
#define MEMSIZE 65536
#define REGS 32
#define FREGS 32
struct simulator{
  char mem[MEMSIZE];
  int reg[REGS];
  float freg[FREGS];
  int pc;
};

struct instruct{
  int opcode;
  int operands[4];
};

typedef struct simulator Simulator;
typedef struct instruct Instruct;

struct instr_list{
  int no;
  Instruct instr;  
  struct instr_list *next;
  struct instr_list *back;
};

/*ラベル構造体の定義*/
typedef struct label{
  char name[100];
  int pc;
  int type;
} Label;

typedef struct instr_list Instr_list;

Instruct instruct_get(Instr_list l);
Instr_list instruct_load(int fd);

#define Rev_bits(i) ((((i)&1)<<7)|(((i)&2)<<5)|(((i)&4)<<3)|(((i)&8)<<1)|(((i)&16)>>1)|(((i)&32)>>3)|(((i)&64)>>5)|(((i)&128)>>7))

#endif
