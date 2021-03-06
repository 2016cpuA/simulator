#include <stdio.h>
#include <string.h>
#include <math.h>
#include "instructs.h"
#include "message.h"
#define Print(text) fprintf(out_file,text)
#define Inc(pc) ((pc)++)

FILE *input_file;
FILE *output_file;

int max_addr=0;

unsigned int get_instr(char *name){
  long long int x=0;
  int i;
  int max = strlen(name);
  if(max>8){
    return UNKNOWN;
  }
  for(i=0;i<max;i++){
    x|=name[i];
    if(i!=max-1)
      x<<=8;
  }
  if(!(x^0x61646469L)|!(x^0x41444449L)){
    return ADDI;
  }else if(!(x^0x616464L)|!(x^0x414444L)){
    return ADD;
  }else if(!(x^0x737562L)|!(x^0x535542L)){
    return SUB;
  }else if(!(x^0x736c74L)|!(x^0x534c54L)){
    return SLT;
  }else if(!(x^0x626571L)|!(x^0x424551L)){
    return BEQ;
  }else if(!(x^0x626e65L)|!(x^0x424e45L)){
    return BNE;
  }else if(!(x^0x6aL)|!(x^0x4aL)){
    return J;
  }else if(!(x^0x6a616cL)|!(x^0x4a414cL)){
    return JAL;
  }else if(!(x^0x6a72L)|!(x^0x4a52L)){
    return JR;
  }else if(!(x^0x6a616c72L)|!(x^0x4a414c52L)){
    return JALR;
  }else if(!(x^0x6c77L)|!(x^0x4c57L)){
    return LW;
  }else if(!(x^0x6c61L)|!(x^0x4c41L)){
    return LA;
  }else if(!(x^0x6c69L)|!(x^0x4c49L)){
    return LI;
  }else if(!(x^0x7377L)|!(x^0x5357L)){
    return SW;
  }else if(!(x^0x616e64L)|!(x^0x414e44L)){
    return AND;
  }else if(!(x^0x616e6469L)|!(x^0x414e4449L)){
    return ANDI;
  }else if(!(x^0x6f72L)|!(x^0x4f52L)){
    return OR;
  }else if(!(x^0x6f7269L)|!(x^0x4f5249L)){
    return ORI;
  }else if(!(x^0x786f72L)|!(x^0x584f52L)){
    return XOR;
  }else if(!(x^0x736c6cL)|!(x^0x534c4cL)){
    return SLL;
  }else if(!(x^0x737261L)|!(x^0x535241L)){
    return SRA;
  }else if(!(x^0x73726cL)|!(x^0x53524cL)){
    return SRL;
  }else if(!(x^0x696eL)|!(x^0x494eL)){
    return IN;
  }else if(!(x^0x6f7574L)|!(x^0x4f5554L)){
    return OUT;
  }else if(!(x^0x6f757462L)|!(x^0x4f555442L)){
    return OUTB;
  }else if(!(x^0x73776331L)|!(x^0x53574331L)){
    return SWC1;
  }else if(!(x^0x6c776331L)|!(x^0x4c574331L)){
    return LWC1;
  }else if(!(x^0x6164642e73L)|!(x^0x4144442e53L)|!(x^0x4144442e73L)){
    return ADD_S;
  }else if(!(x^0x7375622e73L)|!(x^0x5355422e53L)|!(x^0x5355422e73L)){
    return SUB_S;  
  }else if(!(x^0x6d756c2e73L)|!(x^0x4d554c2e53L)|!(x^0x4d554c2e73L)){
    return MUL_S;
  }else if(!(x^0x6469762e73L)|!(x^0x4449562e53L)|!(x^0x4449562e73L)){
    return DIV_S;
  }else if(!(x^0x632e65712e73L)|!(x^0x432e45512e53L)|!(x^0x432e65712e73L)){
    return C_EQ_S;
  }else if(!(x^0x632e6c652e73L)|!(x^0x432e4c452e53L)|!(x^0x432e6c652e73L)){
    return C_LE_S;
  }else if(!(x^0x632e6c742e73L)|!(x^0x432e4c542e53L)|!(x^0x432e6c742e73L)){
    return C_LT_S;

  }else if(!(x^0x6d756c74L)|!(x^0x4d554c54L)){
    return MULT;
  }else if(!(x^0x646976L)|!(x^0x444956L)){
    return DIV;
  }else if(!(x^0x636c656172L)|!(x^0x434c454152L)){ /* clear */
    return 0;
  }else if(!(x^0x6e6f70L)|!(x^0x4e4f50L)){
    return NOP;
  }else if(!(x^0x6d6f7665L)|!(x^0x4d4f5645L)){
    return MOVE;
  }else if(!(x^0x68616c74L)|!(x^0x48414c54L)){
    return HALT;
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
  case LA: Print("LA");break;
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
  case OUTB: Print("OUTB");break;
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

#ifndef _HP
#define _HP 28
#endif

void sim_libs(Simulator *sim,int label){
  int i,int_init,max,hp;
  float fl_init;
  switch(label){
  case LIB_WBYTE: 
    putc((sim->reg[1]&0xff),output_file);
    break;
  case LIB_WINT: 
    fprintf(output_file,"%d",(sim->reg[1]));
    break;
  case LIB_WFLOAT: 
    fprintf(output_file,"%f",(sim->freg[1]));
    break;
  case LIB_WFLOATE: 
    fprintf(output_file,"%e",(sim->freg[1]));
    break;
  case LIB_NLINE: 
    fprintf(output_file,"\n");
    break;
  case LIB_RBYTE: 
    sim->reg[1]=fgetc(input_file);
    break;
  case LIB_RINT: 
    fscanf(input_file,"%d",&(sim->reg[1]));
    break;
  case LIB_RFLOAT: 
    fscanf(input_file,"%f",&(sim->freg[1]));
    break;
  case LIB_SIN: 
    sim->freg[1]=sinf(sim->freg[1]);
    break;
  case LIB_COS: 
    sim->freg[1]=cosf(sim->freg[1]);
    break;
  case LIB_ATAN: 
    sim->freg[1]=atanf(sim->freg[1]);
    break;
  case LIB_ITOF:
    sim->freg[1]=(float)(sim->reg[1]);
    break;
  case LIB_FTOI:
    sim->reg[1]=(int)(sim->freg[1]);
    break;
  case LIB_FLOOR:
    sim->freg[1]=floorf(sim->freg[1]);
    break;
  case LIB_SQRT:
    sim->freg[1]=sqrtf(sim->freg[1]);
    break;
  case LIB_FABS:
    sim->freg[1]=fabsf(sim->freg[1]);
    break;
  case LIB_CR_ARRAY:
    /*
    hp=sim->reg[_HP];
    max=sim->reg[1];
    int_init=sim->reg[2];
    for(i=0;i<max;i++){
      *(int*)&(sim->mem[i*4+hp])=int_init;
    }
    sim->reg[_HP]+=(sim->reg[1])*4;
    sim->reg[1]=hp;
    break;
    */
    hp=sim->reg[_HP];
    max=sim->reg[1];
    int_init=sim->reg[2];
    for(i=0;i<max;i++){
      *(int*)&(sim->mem[i+hp])=int_init;
    }
    sim->reg[_HP]+=(sim->reg[1]);
    sim->reg[1]=hp;
    break;
  case LIB_CR_ARRAY_F:
    /*
    hp=sim->reg[_HP];
    max=sim->reg[1];
    fl_init=sim->freg[1];
    for(i=0;i<max;i++){
      *(float*)&(sim->mem[i*4+hp])=fl_init;
    }
    sim->reg[_HP]+=max*4; 
    sim->reg[1]=hp;
    break;
    */
    hp=sim->reg[_HP];
    max=sim->reg[1];
    fl_init=sim->freg[1];
    for(i=0;i<max;i++){
      *(float*)&(sim->mem[i+hp])=fl_init;
    }
    sim->reg[_HP]+=max; 
    sim->reg[1]=hp;
    break;
  case LIB_F_IS_0:
    sim->reg[1]=(sim->freg[1]==0.0);
    break;
  case LIB_F_IS_POS:
    sim->reg[1]=(sim->freg[1]>0.0);
    break;
  case LIB_F_IS_NEG:
    sim->reg[1]=(sim->freg[1]<0.0);
    break;
  case LIB_F_NEG:
    sim->freg[1]=-(sim->freg[1]);
    break;
  case LIB_F_SQR:
    sim->freg[1]=(sim->freg[1])*(sim->freg[1]);
    break;
  case LIB_F_LESS:
    sim->reg[1]=(sim->freg[1]<sim->freg[2]);
    break;
  case LIB_F_HALF:
    sim->freg[1]=(sim->freg[1])*0.5;
    break;
  }
}

int instr_nop(Simulator *sim,int rs,int rt,int rd,int sa) {
  (void)(rs);
  (void)(rt);
  (void)(rd);
  (void)(sa);
  Inc(sim->pc);
  return 0;
}

int instr_add(Simulator *sim,int rs,int rt,int rd,int sa) {
  (void)(sa);
  sim->reg[rd] = sim->reg[rs] + sim->reg[rt];
  Inc(sim->pc);
  return 0;
}

int instr_sub(Simulator *sim,int rs,int rt,int rd,int sa) {
  (void)(sa);
  sim->reg[rd] = sim->reg[rs] - sim->reg[rt];
  Inc(sim->pc);
  return 0;
}

int instr_mult(Simulator *sim,int rs,int rt,int rd,int sa) {
  (void)(sa);
  sim->reg[rd] = sim->reg[rs] * sim->reg[rt];
  Inc(sim->pc);
  return 0;
}

int instr_div(Simulator *sim,int rs,int rt,int rd,int sa) {
  (void)(sa);
  sim->reg[rd] = sim->reg[rs] / sim->reg[rt];
  Inc(sim->pc);
  return 0;
}

int instr_slt(Simulator *sim,int rs,int rt,int rd,int sa) {
  (void)(sa);
  if (sim->reg[rs] < sim->reg[rt]) sim->reg[rd] = 1;
  else sim->reg[rd] = 0;
  Inc(sim->pc);
  return 0; 
}
int instr_jr(Simulator *sim,int rs,int rt,int rd,int sa) {
  (void)(rs);
  (void)(rd);
  (void)(sa);
  if(sim->reg[rt]<0){
    print_err_sim("jr",sim->pc,"invalid instruction index",sim->reg[rt]);
    return rt-64;
  }else{
    sim->pc = sim->reg[rt];
  }
  return 0;
}

int instr_and(Simulator *sim,int rs,int rt,int rd,int sa) {
  (void)(sa);
  sim->reg[rd] = sim->reg[rs] & sim->reg[rt];
  Inc(sim->pc);
  return 0;
}

int instr_or(Simulator *sim,int rs,int rt,int rd,int sa) {
  (void)(sa);
  sim->reg[rd] = sim->reg[rs] | sim->reg[rt];
  Inc(sim->pc);
  return 0;
}

int instr_xor(Simulator *sim,int rs,int rt,int rd,int sa) {
  (void)(sa);
  sim->reg[rd] = sim->reg[rs] ^ sim->reg[rt];
  Inc(sim->pc);
  return 0;
}
 
int instr_sll(Simulator *sim,int rs,int rt,int rd,int sa) {
  if(rs!=0||rt!=0||rd!=0||sa!=0){
    sim->reg[rd] =(int)((unsigned int)(sim->reg[rt])) << sa;
  }
  Inc(sim->pc);
  return 0;
}
#define Sgn(x) ((x)<0?-2147483648:0)

int instr_sra(Simulator *sim,int rs,int rt,int rd,int sa) {
  (void)(rs);
  sim->reg[rd] = (sim->reg[rt]) >> sa;
  Inc(sim->pc);
  return 0;
}
union _int{
  int i;
  unsigned int u;
};
int instr_srl(Simulator *sim,int rs,int rt,int rd,int sa) {
  (void)(rs);
  union _int i;
  i.i=sim->reg[rt];
  i.u=i.u>>sa;
  sim->reg[rd] = i.i;
  Inc(sim->pc);
  return 0;
}

int instr_in(Simulator *sim,int rs,int rt,int rd,int sa){
  (void)(rs);
  (void)(rt);
  (void)(sa);
  unsigned char buf[4];
  int size;
  (sim->pc)++;
  size=(int)fread(buf, 1, 4, input_file);
  if(size<4){
    print_err_sim("in",sim->pc,"input file reached EOF",0);
    return -1;
  }
  sim->reg[rd]=((int)(buf[0])<<24)|((int)(buf[1])<<16)|((int)(buf[2])<<8)|((int)(buf[3]));
  return 0;
}

int instr_out(Simulator *sim,int rs,int rt,int rd,int sa){
  (void)(rd);
  (void)(rt);
  (void)(sa);
  char buf[4];
  int target = sim->reg[rs];
  buf[0]=(char)(target>>24);
  buf[1]=(char)((target>>16)&0xff);
  buf[2]=(char)((target>>8)&0xff);
  buf[3]=(char)((target)&0xff);
  (sim->pc)++;
  fwrite(buf, 4, 1, output_file);
  return 0;
}

int instr_outb(Simulator *sim,int rs,int rt,int rd,int sa){
  (void)(rd);
  (void)(rt);
  (void)(sa);
  char buf;
  int target = sim->reg[rs];
  buf=(char)(target&0xff);
  (sim->pc)++;
  fwrite(&buf, 1, 1, output_file);
  return 0;
}

/*形式Iの命令*/

int instr_move(Simulator *sim,int rs,int rt,int imm){
  (void)(imm);
  (sim->reg[rt])=(sim->reg[rs]);
  Inc(sim->pc);
  return 0;
}

int instr_addi(Simulator *sim,int rs,int rt,int imm) {
  
  int _imm = (int)((short)(imm&0xFFFF));
  sim->reg[rt] = sim->reg[rs] + _imm;
  (sim->pc)++;
  return 0;
} 

int instr_andi(Simulator *sim,int rs,int rt,int imm) {
  int _imm = (int)((short)(imm&0xFFFF));

  sim->reg[rt] = sim->reg[rs] & _imm;
  Inc(sim->pc);
  return 0;
}

int instr_ori(Simulator *sim,int rs,int rt,int imm) {
  int _imm = (int)((short)(imm&0xFFFF));

  sim->reg[rt] = sim->reg[rs] | _imm;
  Inc(sim->pc);
  return 0;
}

int instr_beq(Simulator *sim,int rs,int rt,int offset) {
  if (sim->reg[rs] == sim->reg[rt]){
    if (sim->pc+offset<0){
      print_err_sim("beq",sim->pc,"invalid instruction index",sim->pc+offset);
      return -1;
    }else
      sim->pc += offset;
  }else Inc(sim->pc);
  return 0;
}

int instr_bne(Simulator *sim,int rs,int rt,int offset) {
  if (sim->reg[rs] != sim->reg[rt])
    if (sim->pc+offset<0){
      print_err_sim("bne",sim->pc,"invalid instruction index",sim->pc+offset);
      return -1;
    }else
      sim->pc += offset;
  else Inc(sim->pc);
  return 0;
}

int instr_lw(Simulator *sim,int rbase,int rt,int offset) {
  int addr=sim->reg[rbase] + offset;
  if(0<=addr&&addr<MEMSIZE){
    sim->reg[rt] = sim->mem[addr];
    Inc(sim->pc);
    return 0;
  }else{
    print_err_sim("lw",sim->pc,"invalid address",addr);
    return -1;
  }
}

int instr_sw(Simulator *sim,int rbase,int rt,int offset) {
  int addr=sim->reg[rbase] + offset;
  /*for debug*/
  max_addr=(max_addr>sim->reg[rbase])?max_addr:sim->reg[rbase];
  /*sim->reg[16]=max_addr;*/
  /**/
  if(addr>=0&&addr<MEMSIZE){
    sim->mem[addr]=sim->reg[rt];
    Inc(sim->pc);
  }else{
    print_err_sim("sw",sim->pc,"invalid address",addr);
    return -1;
  }
  return 0;
}


/*形式Jの命令*/
int instr_jal(Simulator *sim,int instr_index) {
  if(instr_index>0x1000000){
    sim_libs(sim,instr_index);
    Inc(sim->pc);
  }else if(instr_index<0){
    print_err_sim("jal",sim->pc,"invalid instruction index",instr_index);
    return -1;
  }else{
    sim->reg[31] = (sim->pc) +1;
    sim->pc =  instr_index ;
  }
  return 0;
}

int instr_j(Simulator *sim,int instr_index) {
  if(instr_index>0x1000000){
    sim_libs(sim,instr_index);
    sim->pc=sim->reg[31];
    return 0;
  }else if(instr_index<0){
    print_err_sim("j",sim->pc,"invalid instruction index",instr_index);
    return -1;
  }else{
    if(sim->pc==instr_index)
      return 65536;
    else
      sim->pc =  instr_index ;
  }
  return 0;
}

union i_f {
  int i;
  float f;
};
/*浮動小数点・R形式*/
int instr_fadds(Simulator *sim,int fmt,int ft,int fs,int fd){
  (void)(fmt);
  sim->freg[fd]=(sim->freg[fs])+(sim->freg[ft]);
  Inc(sim->pc);
  return 0;
}
int instr_fsubs(Simulator *sim,int fmt,int ft,int fs,int fd){
  (void)(fmt);
  sim->freg[fd]=(sim->freg[fs])-(sim->freg[ft]);
  Inc(sim->pc);
  return 0;
}
int instr_fmuls(Simulator *sim,int fmt,int ft,int fs,int fd){
  (void)(fmt);
  sim->freg[fd]=(sim->freg[fs])*(sim->freg[ft]);
  Inc(sim->pc);
  return 0;
}
int instr_fdivs(Simulator *sim,int fmt,int ft,int fs,int fd){
  (void)(fmt);
  float diver=sim->freg[ft],dived=sim->freg[fs];
  if(diver==0){
    print_warn_sim("div.s",sim->pc,"divided by ",diver);
  } 
  sim->freg[fd]=dived/diver;
  Inc(sim->pc);
  return 0;
}
int instr_fceqs(Simulator *sim,int fmt,int ft,int fs,int rd){
  (void)(fmt);
  sim->reg[rd]=(sim->freg[fs]==sim->freg[ft]);
  Inc(sim->pc);
  return 0;
}
int instr_fcles(Simulator *sim,int fmt,int ft,int fs,int rd){
  (void)(fmt);
  sim->reg[rd]=(sim->freg[fs]<=sim->freg[ft]);
  Inc(sim->pc);
  return 0;
}
int instr_fclts(Simulator *sim,int fmt,int ft,int fs,int rd){
  (void)(fmt);
  sim->reg[rd]=(sim->freg[fs]<sim->freg[ft]);
  Inc(sim->pc);
  return 0;
}
/*浮動小数点・I形式*/
int instr_flwc1(Simulator *sim,int rbase,int ft,int offset){
  int addr=sim->reg[rbase] + offset;
  union i_f dest;
  if(0<=addr&&addr<MEMSIZE){
    dest.i = sim->mem[addr];
    sim->freg[ft] = dest.f;
    if(isnan(dest.f)){
      print_warn_sim("lwc1",sim->pc,"NaN was loaded from address ",addr);
    }
    Inc(sim->pc);
    return 0;
  }else{
    print_err_sim("lwc1",sim->pc,"invalid address",addr);
    return -1;
  }
}  
int instr_fswc1(Simulator *sim,int rbase,int ft,int offset){
  int addr=sim->reg[rbase] + offset;
  union i_f src;
  if(addr>=0&&addr<MEMSIZE){
    src.f=sim->freg[ft];
    sim->mem[addr]=src.i;
    Inc(sim->pc);
  }else{
    print_err_sim("swc1",sim->pc,"invalid address",addr);
    return -1;
  }
  return 0;
}


