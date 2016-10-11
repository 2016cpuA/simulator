#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"
#include "instructs.h"

/*readline.cに定義*/
extern Instruct *load_instruct(int fd,int *size);

#define Sim_Init(sim) int _i; do{ for(_i=0;_i<MEMSIZE;_i++) sim.mem[_i]=0; for(_i=0;_i<REGS;_i++) (sim).reg[_i]=0;(sim).pc=0;} while(0);

void print_regs(Simulator sim){
  int i;
  for (i=0;i<REGS;i++){
    fprintf(stderr,"GPR[%d]=%d",i,sim.reg[i]);
    if(i%8==7){
      fprintf(stderr,"\n");
    }else{
      fprintf(stderr,";\t");
    }
  }
  fprintf(stderr,"PC=%d\n",sim.pc);
}

int main(int argc,char* argv[]){
  Simulator sim;
  Instruct *instr,now;
  int (*instr_r)(Simulator*,int,int,int,int),(*instr_i)(Simulator*,int,int,int),(*instr_j)(Simulator*,int),op[4];
  int n,instr_type;
  Sim_Init(sim);
  /*命令のロード*/
  instr=load_instruct(0,&n);
  
  /*simulator実行部分*/
  if(n>=0){
    while(sim.pc<n){
      /*FETCH*/
      now=instr[sim.pc];
      instr_type=judge_type(now.opcode);
      switch(instr_type){
      case TYPE_R: 
	fetch_r(&instr_r,op,now);
	break;
      case TYPE_I:
	fetch_i(&instr_i,op,now);
	break;
      case TYPE_J:
	fetch_j(&instr_j,op,now);
	break;
      }
      /*EXECUTE*/
      switch(instr_type){
      case TYPE_R: 
        (*instr_r)(&sim,op[0],op[1],op[2],op[3]);
	break;
      case TYPE_I:
	(*instr_i)(&sim,op[0],op[1],op[2]);
	break;
      case TYPE_J:
        (*instr_j)(&sim,op[0]);
	break;
      }
    }
    print_regs(sim);
    free(instr);
  }
  return 0;
}    
