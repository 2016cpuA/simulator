#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "simulator.h"
#include "instructs.h"

/*readline.cに定義*/
extern Instruct *load_instruct(int fd,char* output_instr_file_name,int *size);

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

void conv(int code,char buf[4]){
  buf[0]=(char)(code&0xFF);
  buf[1]=(char)((code&0xFF00)>>8);
  buf[2]=(char)((code&0xFF0000)>>16);
  buf[3]=(char)((code%0xFF000000)>>24);
}

int make_code(int out_fd, Instruct *instr, int n) {
  int i, j, op[4], instr_type, code, written = 0;
  char buf[4];

  for(i = 0; i < n; i++) {
    if((instr_type = judge_type(instr[i].opcode)) > 0) {
      switch(instr_type) {
        case TYPE_R:
        fetch_r(NULL, op, instr[i]);
        code = make_code_r(instr[i].opcode, op[0], op[1], op[2], op[3]);
        conv(code, buf);
        for(j = 0; j < 4; j++)
          written += write(out_fd, (void*)(buf+j), 1);
        break;
       
       case TYPE_I:
       fetch_i(NULL, op, instr[i]);
       code = make_code_i(instr[i].opcode, op[0], op[1], op[2]);
       conv(code, buf);
       for(j = 0; j < 4; j++)
          written += write(out_fd, (void*)(buf+j), 1);
       break;

       case TYPE_J:
       fetch_j(NULL, op, instr[i]);
       code = make_code_j(instr[i].opcode, op[0]);
       conv(code, buf);
       for(j = 0; j < 4; j++)
          written+=write(out_fd,(void*)(buf+j),1);
       break;
     }
   }
 }

 return written;
}

int simulation(Instruct *instr, int n) {
  Simulator sim;
  Instruct now;
  int (*instr_r)(Simulator*,int,int,int,int),(*instr_i)(Simulator*,int,int,int),(*instr_j)(Simulator*,int),op[4];
  int instr_type,clocks=0;
  Sim_Init(sim);
  /*simulator実行部分*/
  fprintf(stderr,"Execution started.\n");
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
  return 0;
} 

int step_simulation(Instruct *instr, int n) {
  Simulator sim;
  Instruct now;
  int (*instr_r)(Simulator*,int,int,int,int),(*instr_i)(Simulator*,int,int,int),(*instr_j)(Simulator*,int),op[4];
  int instr_type,clocks=0;
  int ch;
  Sim_Init(sim);
  /*simulator実行部分*/
  fprintf(stderr,"Execution started.\n");
  while(sim.pc<n && ((ch = getchar()) == '\n')){
    if(ch == EOF) {
      fprintf(stderr,"step simulation cancelled.\n");
      break;
    }
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

    fprintf(stderr,"STEP No.%d.\n", sim.pc);
    print_regs(sim);
    fprintf(stderr,"clocks: %d\n\n",clocks);
  }

  fprintf(stderr,"Execution finished.\n");
  print_regs(sim);
  fprintf(stderr,"clocks: %d\n",clocks);
  free(instr);
  return 0;
}

int _sim(int program_fd,char *output_instr_file_name,int out_binary_fd,int execute, int debug){
  int n,written_bytes;
  Instruct *instr;
  /*命令のロード*/
  instr=load_instruct(program_fd,output_instr_file_name,&n);
  if(out_binary_fd>0){
    written_bytes=make_code(out_binary_fd,instr,n);
    fprintf(stderr,"%d byte written\n",written_bytes);
  }
  if(execute&&n>=0){
    if(debug) step_simulation(instr, n);
    else simulation(instr, n);
  }
  return 0;
}
