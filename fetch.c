#include "instructs.h"
#include "simulator.h"

#include <stdlib.h>

#define TYPE_R 1
#define TYPE_I 2
#define TYPE_J 3
#define UNKNOWN_OP -1

int judge_type(int opcode){
  switch(opcode){
  case J:
  case JAL: return TYPE_J;break;
  case MOVE:
  case ADDI:
  case BEQ:
  case BNE:
  case LW:
  case SW:
  case ANDI:
  case ORI: return TYPE_I;break;
  case NOP:
  case ADD:
  case SUB:
  case MULT:
  case DIV:
  case SLT:
  case JR:
  case AND:
  case OR:
  case XOR:
  case SLL:
  case SRA:
  case SRL:
  case IN:
  case OUT: return TYPE_R;break;
  }
  return UNKNOWN;
}
    
int fetch_r(int (**instr)(Simulator*,int,int,int,int),int op[4],Instruct ins){
  switch(ins.opcode){
    /* <op> rd,rs,rt  ... rd <- rs <op> rt*/
  case ADD: if(instr!=NULL) *instr=instr_add;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[2];op[2]=ins.operands[0];op[3]=0;break;
  case SUB: if(instr!=NULL) *instr=instr_sub;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[2];op[2]=ins.operands[0];op[3]=0;break;
  case MULT: if(instr!=NULL) *instr=instr_mult;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[2];op[2]=ins.operands[0];op[3]=0;break;
  case DIV: if(instr!=NULL) *instr=instr_div;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[2];op[2]=ins.operands[0];op[3]=0;break;
  case SLT: if(instr!=NULL) *instr=instr_slt;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[2];op[2]=ins.operands[0];op[3]=0;break;
  case AND: if(instr!=NULL) *instr=instr_and;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[2];op[2]=ins.operands[0];op[3]=0;break;
  case OR: if(instr!=NULL) *instr=instr_or;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[2];op[2]=ins.operands[0];op[3]=0;break;
  case XOR: if(instr!=NULL) *instr=instr_xor;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[2];op[2]=ins.operands[0];op[3]=0;break;
    /* <op> rd,rt,sa  ... rd <- rt <op> sa*/
  case SLL: if(instr!=NULL) *instr=instr_sll;
    if(op!=NULL)
      op[0]=0;op[1]=ins.operands[1];op[2]=ins.operands[0];op[3]=ins.operands[2];break;
  case SRA: if(instr!=NULL) *instr=instr_sra;
    if(op!=NULL)
      op[0]=0;op[1]=ins.operands[1];op[2]=ins.operands[0];op[3]=ins.operands[2];break;
  case SRL: if(instr!=NULL) *instr=instr_srl;
    if(op!=NULL)
      op[0]=0;op[1]=ins.operands[1];op[2]=ins.operands[0];op[3]=ins.operands[2];break;
    /* JR rs*/
  case JR: if(instr!=NULL) *instr=instr_jr;
    if(op!=NULL)
      op[0]=ins.operands[0];op[1]=0;op[2]=0;op[3]=0;break;
    /* IN rd*/
  case IN: if(instr!=NULL) *instr=instr_in;
    if(op!=NULL)
      op[0]=0;op[1]=0;op[2]=ins.operands[0];op[3]=0;break;
    /* OUT rs*/
  case OUT: if(instr!=NULL) *instr=instr_out;
    if(op!=NULL)
      op[0]=ins.operands[0];op[1]=0;op[2]=0;op[3]=0;break;
    /*NOP*/
  case NOP: if(instr!=NULL) *instr=instr_out;
    if(op!=NULL)
      op[0]=0;op[1]=0;op[2]=0;op[3]=0;break;
  }
  return 0;
}

int fetch_i(int (**instr)(Simulator*,int,int,int),int op[4],Instruct ins){
  switch(ins.opcode){
  case MOVE: if(instr!=NULL) *instr=instr_addi;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[0];op[2]=0;break;
  case ADDI: if(instr!=NULL) *instr=instr_addi;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[0];op[2]=ins.operands[2];break;
  case ANDI: if(instr!=NULL) *instr=instr_andi;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[0];op[2]=ins.operands[2];break;
  case ORI: if(instr!=NULL) *instr=instr_ori;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[0];op[2]=ins.operands[2];break;
  case BEQ: if(instr!=NULL) *instr=instr_beq;
    if(op!=NULL)
      op[0]=ins.operands[0];op[1]=ins.operands[1];op[2]=ins.operands[2];break;
  case BNE: if(instr!=NULL) *instr=instr_bne;
    if(op!=NULL)
      op[0]=ins.operands[0];op[1]=ins.operands[1];op[2]=ins.operands[2];break;
  case LW: if(instr!=NULL) *instr=instr_lw;
    if(op!=NULL)
      op[0]=ins.operands[2];op[1]=ins.operands[0];op[2]=ins.operands[1];break;
  case SW: if(instr!=NULL) *instr=instr_sw;
    if(op!=NULL)
      op[0]=ins.operands[2];op[1]=ins.operands[0];op[2]=ins.operands[1];break;
  }
  return 0;
}

int fetch_j(int (**instr)(Simulator*,int),int op[4],Instruct ins){
  switch(ins.opcode){
  case J: if(instr!=NULL) *instr=instr_j;
    if(op!=NULL)
      op[0]=ins.operands[0];break;
  case JAL: if(instr!=NULL) *instr=instr_jal;
    if(op!=NULL)
      op[0]=ins.operands[0];break;
  }
  return 0;
}

int code_fetch(int code,int *opcode,int op[4]){
  int type;
  if((type=judge_type(Fetch_opcode(code)<<26))==UNKNOWN_OP)
    if((type=judge_type((Fetch_opcode(code)<<26)|Fetch_function(code)))==UNKNOWN_OP)
      return -1;
    else 
      *opcode=(Fetch_opcode(code)<<26)|Fetch_function(code);
  else
    *opcode=(Fetch_opcode(code)<<26);
  if(type==TYPE_R){
    op[0]=Fetch_rs(code);
    op[1]=Fetch_rt(code);
    op[2]=Fetch_rd(code);
    op[3]=Fetch_sa(code);
  }else if(type==TYPE_I){
    op[0]=Fetch_rs(code);
    op[1]=Fetch_rt(code);
    op[2]=Fetch_immediate(code);
  }else{
    op[0]=Fetch_instr_index(code);
  }
  return 0;
}

