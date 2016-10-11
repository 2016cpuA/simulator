#include <stdio.h>
#include <string.h>
#include "instructs.h"

#define Print(text) fprintf(stderr,text)
#define Inc(pc) ((pc)++)

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
  }else if(!strcmp(name,"SLT")){
    return SLT;
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
    printf("Error: unknown instruction '%s'\n",name);
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

int make_code_r(int opcode,int rs,int rt,int rd,int sa){
  return (opcode&0xfc00003f)&|(rs<<21)|(rt<<16)|(rd<<11)|(sa<<6);
}
int make_code_i(int opcode,int rs,int rt,int imm){
  return (opcode&0xfc00003f)|(rs<<21)|(rt<<16)|(imm&0xffff);
}

int make_code_j(int opcode,int instr_index){
  return (opcode&0xfc00003f)|instr_index;
}

int instr_add(Simulator *sim,int rs,int rt,int rd,int sa) {
  sim->reg[rd] = sim->reg[rs] + sim->reg[rt];
  Inc(sim->pc);
  return 0;
}

int instr_sub(Simulator *sim,int rs,int rt,int rd,int sa) {
  sim->reg[rd] = sim->reg[rs] - sim->reg[rt];
  Inc(sim->pc);
  return 0;
}

int instr_mult(Simulator *sim,int rs,int rt,int rd,int sa) {
  sim->reg[rd] = sim->reg[rs] * sim->reg[rt];
  Inc(sim->pc);
  return 0;
}

int instr_div(Simulator *sim,int rs,int rt,int rd,int sa) {
  sim->reg[rd] = sim->reg[rs] / sim->reg[rt];
  Inc(sim->pc);
  return 0;
}

int instr_slt(Simulator *sim,int rs,int rt,int rd,int sa) {
  if (sim->reg[rs] < sim->reg[rt]) sim->reg[rd] = 1;
  else sim->reg[rd] = 0;
  Inc(sim->pc);
  return 0; 
}
int instr_jr(Simulator *sim,int rs,int rt,int rd,int sa) {
  sim->pc = sim->reg[rs];
  return 0;
}

int instr_and(Simulator *sim,int rs,int rt,int rd,int sa) {
  sim->reg[rd] = sim->reg[rs] & sim->reg[rt];
  Inc(sim->pc);
  return 0;
}

int instr_or(Simulator *sim,int rs,int rt,int rd,int sa) {
  sim->reg[rd] = sim->reg[rs] | sim->reg[rt];
  Inc(sim->pc);
  return 0;
}

int instr_xor(Simulator *sim,int rs,int rt,int rd,int sa) {
  sim->reg[rd] = sim->reg[rs] ^ sim->reg[rt];
  Inc(sim->pc);
  return 0;
}
 
int instr_sll(Simulator *sim,int rs,int rt,int rd,int sa) {
  sim->reg[rd] = (int)((unsigned int)(sim->reg[rt]) << sa);
  Inc(sim->pc);
  return 0;
}

int instr_sra(Simulator *sim,int rs,int rt,int rd,int sa) {
  sim->reg[rd] = sim->reg[rs] >> sa;
  Inc(sim->pc);
  return 0;
}

int instr_srl(Simulator *sim,int rs,int rt,int rd,int sa) {
  sim->reg[rd] = (int)((unsigned int)(sim->reg[rs]) >> sa);
  Inc(sim->pc);
  return 0;
}

/*入力方法が未定義*/
int instr_in(Simulator *sim,int rs,int rt,int rd,int sa){
  (sim->pc)++;
  fread(&sim->reg[rd], 4, 1, stdin);
  return 0;
}

/*出力方法が未定義*/
int instr_out(Simulator *sim,int rs,int rt,int rd,int sa){
  (sim->pc)++;
  fwrite(&sim->reg[rs], 4, 1, stdout);
  return 0;
}


/*形式Iの命令*/
int instr_addi(Simulator *sim,int rs,int rt,int imm) {
  sim->reg[rt] = sim->reg[rs] + imm;
  (sim->pc)++;
  return 0;
} 

int instr_andi(Simulator *sim,int rs,int rt,int imm) {
  sim->reg[rt] = sim->reg[rs] & imm;
  Inc(sim->pc);
  return 0;
}

int instr_ori(Simulator *sim,int rs,int rt,int imm) {
  sim->reg[rt] = sim->reg[rs] | imm;
  Inc(sim->pc);
  return 0;
}

int instr_beq(Simulator *sim,int rs,int rt,int offset) {
  if (sim->reg[rs] == sim->reg[rt]) sim->pc += offset;
  else Inc(sim->pc);
  return 0;
}

int instr_bne(Simulator *sim,int rs,int rt,int offset) {
  if (sim->reg[rs] != sim->reg[rt]) sim->pc += offset;
  else Inc(sim->pc);
  return 0;
}

int instr_lw(Simulator *sim,int rbase,int rt,int offset) {
  int addr=sim->reg[rbase] + offset;
  char *memory = sim->mem;
  sim->reg[rt] = (((int)memory[addr]&0xff)|(((int)memory[addr+1]&0xff)<<8)|(((int)memory[addr+2]&0xff)<<16)|(((int)memory[addr+3]&0xff)<<24));
  Inc(sim->pc);
  return 0;
}

int instr_sw(Simulator *sim,int rbase,int rt,int offset) {
  int addr=sim->reg[rbase] + offset;
  char *memory = sim->mem;
  memory[addr]=(char)((sim->reg[rt])&0xff);
  memory[addr+1]=(char)(((sim->reg[rt])&0xff00)>>8);
  memory[addr+2]=(char)(((sim->reg[rt])&0xff0000)>>16);
  memory[addr+3]=(char)(((sim->reg[rt])&0xff000000)>>24);
  Inc(sim->pc);
  return 0;
}


/*形式Jの命令*/
int instr_jal(Simulator *sim,int instr_index) {
  sim->reg[31] = (sim->pc) +1;
  sim->pc =  instr_index ;
  return 0;
}

int instr_j(Simulator *sim,int instr_index) {
  sim->pc = instr_index ;
  return 0;
}

