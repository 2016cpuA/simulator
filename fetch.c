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
  case ADDI:
  case BEQ:
  case BNE:
  case LW:
  case SW:
  case ANDI:
  case ORI: return TYPE_I;break;
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
  default: 
  }
  return UNKNOWN;
}
    
int fetch_r(int (*instr)(Simulator*,int,int,int,int),int op[4],Instruct ins){
  switch(ins.opcode){
    /* <op> rd,rs,rt  ... rd <- rs <op> rt*/
  case ADD: (*instr)=instr_add;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[2];op[2]=ins.operands[0];op[3]=0;break;
  case SUB: (*instr)=instr_sub;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[2];op[2]=ins.operands[0];op[3]=0;break;
  case MULT:(*instr)=instr_mult;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[2];op[2]=ins.operands[0];op[3]=0;break;
  case DIV:(*instr)=instr_div;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[2];op[2]=ins.operands[0];op[3]=0;break;
  case SLT:(*instr)=instr_slt;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[2];op[2]=ins.operands[0];op[3]=0;break;
  case AND:(*instr)=instr_and;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[2];op[2]=ins.operands[0];op[3]=0;break;
  case OR:(*instr)=instr_or;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[2];op[2]=ins.operands[0];op[3]=0;break;
  case XOR: (*instr)=instr_xor;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[2];op[2]=ins.operands[0];op[3]=0;break;
    /* <op> rd,rt,sa  ... rd <- rt <op> sa*/
  case SLL: (*instr)=instr_sll;
    if(op!=NULL)
      op[0]=0;op[1]=ins.operands[1];op[2]=ins.operands[0];op[3]=ins.operands[2];break;
  case SRA: (*instr)=instr_sra;
    if(op!=NULL)
      op[0]=0;op[1]=ins.operands[1];op[2]=ins.operands[0];op[3]=ins.operands[2];break;
  case SRL: (*instr)=instr_srl;
    if(op!=NULL)
      op[0]=0;op[1]=ins.operands[1];op[2]=ins.operands[0];op[3]=ins.operands[2];break;
    /* JR rs*/
  case JR:  (*instr)=instr_jr;
    if(op!=NULL)
      op[0]=ins.operands[0];op[1]=0;op[2]=0;op[3]=0;break;
    /* IN rd*/
  case IN: (*instr)=instr_in;
    if(op!=NULL)
      op[0]=0;op[1]=0;op[2]=ins.operands[2];op[3]=0;break;
    /* OUT rs*/
  case OUT: (*instr)=instr_out;
    if(op!=NULL)
      op[0]=ins.operands[2];op[1]=0;op[2]=0;op[3]=0;break;
  default:
  }
  return 0;
}

int fetch_i(int (*instr)(Simulator*,int,int,int),int op[4],Instruct ins){
  switch(ins.opcode){
  case ADDI: (*instr)=instr_addi;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[0];op[2]=ins.operands[2];break;
  case ANDI: (*instr)=instr_andi;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[0];op[2]=ins.operands[2];break;
  case ORI:  (*instr)=instr_ori;
    if(op!=NULL)
      op[0]=ins.operands[1];op[1]=ins.operands[0];op[2]=ins.operands[2];break;
  case BEQ:  (*instr)=instr_beq;
    if(op!=NULL)
      op[0]=ins.operands[0];op[1]=ins.operands[1];op[2]=ins.operands[2];break;
  case BNE:  (*instr)=instr_bne;
    if(op!=NULL)
      op[0]=ins.operands[0];op[1]=ins.operands[1];op[2]=ins.operands[2];break;
  case LW:   (*instr)=instr_lw;
    if(op!=NULL)
      op[0]=ins.operands[2];op[1]=ins.operands[0];op[2]=ins.operands[1];break;
  case SW:   (*instr)=instr_sw;
    if(op!=NULL)
      op[0]=ins.operands[2];op[1]=ins.operands[0];op[2]=ins.operands[1];break;
  default:
  }
  return 0;
}

int fetch_j(int (*instr)(Simulator*,int),int op[4],Instruct ins){
  switch(ins.opcode){
  case J: (*instr)=instr_j;
    if(op!=NULL)
      op[0]=ins.operands[0];break;
  case JAL:(*instr)=instr_j;
    if(op!=NULL)
      op[0]=ins.operands[0];break;
  default:
  }
  return 0;
}

int code_fetch(int code,int *opcode,int op[4]){
  Instruct ins;
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
