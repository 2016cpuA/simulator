#include <stdio.h>
#include <string.h>
#include "instructs.h"

#define Print printf

int get_instr(char *name){
  if(!strcmp(name,"ADD")){
    return ADD;
  }else if(!strcmp(name,"ADDI")){
    return ADDI;
  }else if(!strcmp(name,"SUB")){
    return SUB;
  }else if(!strcmp(name,"MULT")){
    return MULT;
  }else if(!strcmp(name,"DIV")){
    return DIV;
  }else if(!strcmp(name,"DIV")){
    return DIV;
  }else if(!strcmp(name,"BEQ")){
    return BEQ;
  }else if(!strcmp(name,"BNE")){
    return BNE;
  }else if(!strcmp(name,"J")){
    return J;
  }else if(!strcmp(name,"JAL")){
    return JAL;
  }else if(!strcmp(name,"JR")){
    return JR;
  }else if(!strcmp(name,"LW")){
    return LW;
  }else if(!strcmp(name,"SW")){
    return SW;
  }else if(!strcmp(name,"AND")){
    return AND;
  }else if(!strcmp(name,"ANDI")){
    return ANDI;
  }else if(!strcmp(name,"OR")){
    return OR;
  }else if(!strcmp(name,"ORI")){
    return ORI;
  }else if(!strcmp(name,"XOR")){
    return XOR;
  }else if(!strcmp(name,"SLL")){
    return SLL;
  }else if(!strcmp(name,"SRA")){
    return SRA;
  }else if(!strcmp(name,"SRL")){
    return SRL;
  }else if(!strcmp(name,"IN")){
    return IN;
  }else if(!strcmp(name,"OUT")){
    return OUT;
  }else{
    printf("Warning: unknown instruction '%s'\n",name);
    return UNKNOWN;
  }
  return UNKNOWN;
}

void print_instr(Instruct instr){
  switch (instr.opcode){
  case ADD: Print("ADD");break;
  case ADDI: Print("ADDI");break;
  case SUB: Print("SUB");break;
  case MULT: Print("MULT");break;
  case DIV: Print("DIV");break;
  case SLT: Print("SLT");break;
  case BEQ: Print("BEQ");break;
  case BNE: Print("BNE");break;
  case J: Print("J");break;
  case JAL: Print("JAL");break;
  case JR: Print("JR");break;
  case LW: Print("LW");break;
  case SW: Print("SW");break;
  case AND: Print("AND");break;
  case ANDI: Print("ANDI");break;
  case OR: Print("OR");break;
  case ORI: Print("ORI");break;
  case XOR: Print("XOR");break;
  case SLL: Print("SLL");break;
  case SRA: Print("SRA");break;
  case SRL: Print("SRL");break;
  case IN: Print("IN");break;
  case OUT: Print("OUT");break;
  default: Print("#UNKNOWN#");
  }
}

<<<<<<< HEAD
=======
int instr_add(Simulator *sim,int rs,int rt,int rd,int sa) {
  reg[rd] = reg[rs] + reg[rt];
  sim->pc++;
  return reg[rd];
}

int instr_sub(Simulator *sim,int rs,int rt,int rd,int sa) {
  reg[rd] = reg[rs] - reg[rt];
  sim->pc++;
  return reg[rd];
}

int instr_mult(Simulator *sim,int rs,int rt,int rd,int sa) {
  reg[rd] = reg[rs] * reg[rt];
  sim->pc++;
  return reg[rd];
}

int instr_div(Simulator *sim,int rs,int rt,int rd,int sa) {
  reg[rd] = reg[rs] / reg[rt];
  sim->pc++;
  return reg[rd];
}

int instr_slt(Simulator *sim,int rs,int rt,int rd,int sa) {
  if (reg[rs] < reg[rt]) reg[rd] = 1;
  else reg[rd] = 0;
  sim->pc++;
  return reg[rd]; 
}
int instr_jr(Simulator *sim,int rs,int rt,int rd,int sa) {
  sim->pc = reg[rs];
  return reg[rd];
}

int instr_and(Simulator *sim,int rs,int rt,int rd,int sa) {
  reg[rd] = reg[rs] & reg[rt];
  sim->pc++;
  return reg[rd];
}

int instr_or(Simulator *sim,int rs,int rt,int rd,int sa) {
  reg[rd] = reg[rs] | reg[rt];
  sim->pc++
  return reg[rd];
}

int instr_xor(Simulator *sim,int rs,int rt,int rd,int sa) {
  reg[rd] = reg[rs] ^ reg[rt];
  sim->pc++;
  return reg[rd];
}

int instr_sll(Simulator *sim,int rs,int rt,int rd,int sa) {
  reg[rd] = (unsigned int)reg[rt] << sa;
  sim->pc++;
  return reg[rd];
}

int instr_sra(Simulator *sim,int rs,int rt,int rd,int sa) {
  reg[rd] = reg[rs] >> sa;
  sim->pc++;
  return reg[rd];
}

int instr_srl(Simulator *sim,int rs,int rt,int rd,int sa) {
  reg[rd] = (unsigned int)reg[rs] >> reg[rt];
  sim->pc++;
  return reg[rd];
}

int instr_in(Simulator *sim,int rs,int rt,int rd,int sa);
/*実際の動作について説明求む*/
int instr_out(Simulator *sim,int rs,int rt,int rd,int sa);
/*実際の動作について説明求む*/

/*形式Iの命令*/
int instr_addi(Simulator *sim,int rs,int rt,int imm) {
  reg[rt] = reg[rs] + imm;
  sim->pc++;
  return reg[rd];
} 

int instr_andi(Simulator *sim,int rs,int rt,int imm) {
  reg[rt] = reg[rs] & imm;
  sim->pc++;
  return reg[rd];
}

int instr_ori(Simulator *sim,int rs,int rt,int imm) {
  reg[rt] = reg[rs] | imm;
  sim->pc++;
  return reg[rd];
}

int instr_beq(Simulator *sim,int rs,int rt,int offset) {
  if (reg[rs] = reg[rt]) sim->pc += offset;
  else sim->pc++;
  return reg[rd];
}

int instr_bne(Simulator *sim,int rs,int rt,int offset) {
  if (reg[rs] != reg[rt]) sim->pc += offset;
  else sim->pc++;
  return reg[rd];
}

int instr_lw(Simulator *sim,int rbase,int rt,int offset) {
  reg[rt] = sim->mem[rbase + offset];
  sim->pc++;
  return reg[rd];
}

int instr_sw(Simulator *sim,int rbase,int rt,int offset) {
  sim->mem[rbase + offset] = reg[rt];
  sim->pc++;
  return reg[rd];
}


/*形式Jの命令*/
int instr_jal(Simulator *sim,int instr_index) {
  sim->mem[31] = sim->pc + 8;
  sim->pc = sim->pc | (instr_index << 2);
  return reg[rd];
}

int instr_j(Simulator *sim,int instr_index) {
  sim->pc = sim->pc | (instr_index << 2);
  return reg[rd];
}


/*その命令で行う操作を記述する関数 未定義*/
int execute_instr(Simulator* sim,Instruct instr);

>>>>>>> 6b14d28547250bed0fb7f2af1d854058f1d92982
