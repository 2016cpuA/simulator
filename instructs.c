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
  case BNE: Print("SUB");break;
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

