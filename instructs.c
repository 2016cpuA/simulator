#include <stdio.h>
#include <string.h>
#include "instructs.h"

#define Print(text) fprintf(out_file,text)
#define Inc(pc) ((pc)++)

FILE *input_file;
FILE *output_file;

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
  }else if(!strcmp(name,"CLEAR")){
    return 0;
  }else if(!strcmp(name,"NOP")){
    return NOP;
  }else if(!strcmp(name,"MOVE")){
    return MOVE;
  }else if(!strcmp(name,"SWC1")){
    return SWC1;
  }else if(!strcmp(name,"LWC1")){
    return LWC1;
  }else if(!strcmp(name,"ADD.s")){
    return ADD_S;
  }else if(!strcmp(name,"SUB.s")){
    return SUB_S;  
  }else if(!strcmp(name,"MUL.s")){
    return MUL_S;
  }else if(!strcmp(name,"DIV.s")){
    return DIV_S;
  }else if(!strcmp(name,"C.eq.s")){
    return C_EQ_S;
  }else if(!strcmp(name,"C.le.s")){
    return C_LE_S;
  }else if(!strcmp(name,"C.lt.s")){
    return C_LT_S;
  }else{
    printf("Error: unknown instruction '%s'\n",name);
    return UNKNOWN;
  }
  return UNKNOWN;
}

void print_instr(Instruct instr,FILE* out_file){
  switch (instr.opcode&MASK_OP_FUN){
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
  case NOP: Print("NOP");break;  
  case MOVE: Print("MOVE");break;  
  case LWC1: Print("LWC1");break;  
  case SWC1: Print("SWC1");break;  
  case ADD_S: Print("ADD.s");break;  
  case SUB_S: Print("SUB.s");break;  
  case MUL_S: Print("MUL.s");break;  
  case DIV_S: Print("DIV.s");break;  
  case C_EQ_S: Print("C.eq.s");break;  
  case C_LE_S: Print("C.le.s");break;  
  case C_LT_S: Print("C.lt.s");break;  
  default: Print("#UNKNOWN#");
  }
}

int make_code_r(int opcode,int rs,int rt,int rd,int sa) {
  return (opcode&0xfc00003f)|((rs&0x1f)<<21)|((rt&0x1f)<<16)|((rd&0x1f)<<11)|((sa&0x1f)<<6);
}
int make_code_i(int opcode,int rs,int rt,int imm) {
  return (opcode&0xfc000000)|((rs&0x1f)<<21)|((rt&0x1f)<<16)|(imm&0xffff);
}

int make_code_j(int opcode,int instr_index) {
  return (opcode&0xfc000000)|(instr_index&0x03ffffff);
}

int instr_nop(Simulator *sim,int rs,int rt,int rd,int sa) {
  Inc(sim->pc);
  return 0;
}

int instr_clear(Simulator *sim,int rs,int rt,int rd,int sa) {
  int i;
  for (i=0;i<REGS;i++) (sim->reg[i])=0;
  for (i=0;i<FREGS;i++) (sim->freg[i])=0;
  Inc(sim->pc);
  return 0;
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
  if(rs!=0||rt!=0||rd!=0||sa!=0){
    sim->reg[rd] = (sim->reg[rt]) << sa;
    Inc(sim->pc);
  }else{
    instr_clear(sim,0,0,0,0);
  }
  return 0;
}
#define Sgn(x) ((x)<0?-2147483648:0)

int instr_sra(Simulator *sim,int rs,int rt,int rd,int sa) {
  sim->reg[rd] = (sim->reg[rt]) >> sa;
  Inc(sim->pc);
  return 0;
}
union _int{
  int i;
  unsigned int u;
};
int instr_srl(Simulator *sim,int rs,int rt,int rd,int sa) {
  union _int i;
  i.i=sim->reg[rt];
  i.u=i.u>>sa;
  sim->reg[rd] = i.i;
  Inc(sim->pc);
  return 0;
}

int instr_in(Simulator *sim,int rs,int rt,int rd,int sa){
  (sim->pc)++;
  fread(&sim->reg[rd], 4, 1, input_file);
  return 0;
}

int instr_out(Simulator *sim,int rs,int rt,int rd,int sa){
  (sim->pc)++;
  fwrite(&sim->reg[rs], 4, 1, output_file);
  return 0;
}


/*形式Iの命令*/

int instr_move(Simulator *sim,int rs,int rt,int imm){
  (sim->reg[rt])=(sim->reg[rs]);
  Inc(sim->pc);
  return 0;
}

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

union i_f {
  int i;
  float f;
};
/*浮動小数点・R形式*/
int instr_fadds(Simulator *sim,int fmt,int ft,int fs,int fd){
  sim->freg[fd]=(sim->freg[fs])+(sim->freg[ft]);
  Inc(sim->pc);
  return 0;
}
int instr_fsubs(Simulator *sim,int fmt,int ft,int fs,int fd){
  sim->freg[fd]=(sim->freg[fs])-(sim->freg[ft]);
  Inc(sim->pc);
  return 0;
}
int instr_fmuls(Simulator *sim,int fmt,int ft,int fs,int fd){
  sim->freg[fd]=(sim->freg[fs])*(sim->freg[ft]);
  Inc(sim->pc);
  return 0;
}
int instr_fdivs(Simulator *sim,int fmt,int ft,int fs,int fd){
  float diver=sim->freg[ft],dived=sim->freg[fs];
  sim->freg[fd]=dived/diver;
  Inc(sim->pc);
  return 0;
}
int instr_fceqs(Simulator *sim,int fmt,int ft,int fs,int rd){
  sim->reg[rd]=(sim->freg[fs]==sim->freg[ft]);
  Inc(sim->pc);
  return 0;
}
int instr_fcles(Simulator *sim,int fmt,int ft,int fs,int rd){
  sim->reg[rd]=(sim->freg[fs]<=sim->freg[ft]);
  Inc(sim->pc);
  return 0;
}
int instr_fclts(Simulator *sim,int fmt,int ft,int fs,int rd){
  sim->reg[rd]=(sim->freg[fs]<sim->freg[ft]);
  Inc(sim->pc);
  return 0;
}
/*浮動小数点・I形式*/
int instr_flwc1(Simulator *sim,int rbase,int ft,int offset){
  int addr=sim->reg[rbase] + offset;
  char *memory = sim->mem;
  union i_f dest;
  dest.i = (((int)memory[addr]&0xff)|(((int)memory[addr+1]&0xff)<<8)|(((int)memory[addr+2]&0xff)<<16)|(((int)memory[addr+3]&0xff)<<24));
  sim->freg[ft] = dest.f;
  Inc(sim->pc);
  return 0;
}  
int instr_fswc1(Simulator *sim,int rbase,int ft,int offset){
  int addr=sim->reg[rbase] + offset;
  char *memory = sim->mem;
  union i_f src;
  src.f=sim->freg[ft];
  memory[addr]=(char)(src.i&0xff);
  memory[addr+1]=(char)((src.i&0xff00)>>8);
  memory[addr+2]=(char)((src.i&0xff0000)>>16);
  memory[addr+3]=(char)((src.i&0xff000000)>>24);
  Inc(sim->pc);
  return 0;
}


/*浮動小数点数用命令(仕様未定義)*/
int instr_finv(Simulator *sim,int rs,int rt,int rd,int sa);
int instr_sqrt(Simulator *sim,int rs,int rt,int rd,int sa);
int instr_sin(Simulator *sim,int rs,int rt,int rd,int sa);
int instr_cos(Simulator *sim,int rs,int rt,int rd,int sa);
