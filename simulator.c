#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "simulator.h"
#include "instructs.h"

/*readline.cに定義*/
extern Instruct *load_instruct(int fd,int out_instr_fd,int *size);

#define Sim_Init(sim) int _i; do{ for(_i=0;_i<MEMSIZE;_i++) sim.mem[_i]=0; for(_i=0;_i<REGS;_i++) (sim).reg[_i]=0;(sim).pc=0;} while(0);

void print_regs(Simulator sim){
  int i;
  fprintf(stderr,"GPRs:\n");
  fprintf(stderr,"   %11d %11d %11d %11d %11d %11d %11d %11d\n--------------------------------------------------------------------------------------------------\n",0,1,2,3,4,5,6,7);
  for (i=0;i<REGS;i++){
    if(i%8==0)
      fprintf(stderr,"%2d:",i);
    fprintf(stderr,"%11d ",sim.reg[i]);
    if(i%8==7){
      fprintf(stderr,"\n");
    }else{
    }
  }
  fprintf(stderr,"PC=%d\n",sim.pc);
}

int simulation(int program_fd,int out_instr_fd){
  Simulator sim;
  Instruct *instr,now;
  int (*instr_r)(Simulator*,int,int,int,int),(*instr_i)(Simulator*,int,int,int),(*instr_j)(Simulator*,int),op[4];
  int n,instr_type,clocks=0;
  Sim_Init(sim);
  /*命令のロード*/
  instr=load_instruct(program_fd,out_instr_fd,&n);
  
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
      clocks++;
    }
    fprintf(stderr,"Execution finished.\n");
    print_regs(sim);
    fprintf(stderr,"clocks: %d\n",clocks);
    free(instr);
  }
  return 0;
}    

int main(int argc,char* argv[]){
  int program_fd;
  if (argc<1){
    fprintf(stderr,"Error: no input file\n");
  }else{
    if((program_fd=open(argv[1],O_RDONLY))<0){
      fprintf(stderr,"Error: file '%s' not found\n",argv[1]);
    }else{
      simulation(program_fd,-1);
    }
  }
  return 0;
}
