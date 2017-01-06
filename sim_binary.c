#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "simulator.h"
#include "instructs.h"
#include "message.h"
#include <sys/time.h>
static inline void Sim_Init(Simulator *sim) {
  int _i;
  (*sim).mem=(int*)malloc(MEMSIZE*sizeof(int));
  do{
    for(_i=0;_i<MEMSIZE;_i++)
      (*sim).mem[_i]=0;
    for(_i=0;_i<REGS;_i++){
      (*sim).reg[_i]=0;
      (*sim).freg[_i]=0;
    }
    (*sim).pc=0;
  } while(0);
}
static inline double subtime(struct timeval t,struct timeval t0){
  return (double)(t.tv_sec-t0.tv_sec)+(double)(t.tv_usec-t0.tv_usec)*0.000001;
}
#define Sim_fin(sim) do{ free((sim).mem);} while(0);

#define Is_break(opcode) ((opcode)&_BREAK)
#define Clear_break(opcode) opcode=((opcode)&MASK_OP_FUN)
#define Set_break(opcode) opcode=((opcode)|_BREAK)
/*オプションから受け取った変数*/
/*simulation.c*/
extern int debug,execute;
extern long long int iter_max;
extern void print_regs(Simulator sim);
extern void print_time(double t_sec);
extern void console_help();
/*readline.c*/
Label *labels;
int n_label;
void print_code(FILE *output_instr_file, unsigned int *bin, int n){
  int i,op[4]={0,0,0,0};
  Instruct ins;
  fprintf(output_instr_file,"pc\tcode    \tinstr\top1\top2\top3\top4\n");
  for(i=0;i<n;i++){
    op[0]=0;
    op[1]=0;
    op[2]=0;
    op[3]=0;
    fprintf(output_instr_file,"%d\t%08X\t",i,bin[i]);
    code_fetch(bin[i],&(ins.opcode),op);
    print_instr(ins,output_instr_file);
    fprintf(output_instr_file,"\t%d\t%d\t%d\t%d\n",op[0],op[1],op[2],op[3]);
  } 
}
int simulation_bin(unsigned int *bin, int n){
  Simulator sim;
  int now,i;
  Instruct ins,tmp;
  int (*instr_r)(Simulator*,int,int,int,int),(*instr_i)(Simulator*,int,int,int),(*instr_j)(Simulator*,int);
  int flag=0;
  int instr_type;
  long long int clocks=0;
  Instruct *instr=(Instruct*)malloc(n*sizeof(Instruct));
  struct timeval t0,t;
  for(i=0;i<n;i++){
    instr[i].opcode=0xffffffff;
  }
  Sim_Init(&sim);
  /*simulator実行部分*/
  print_msg_sim_start();
  gettimeofday(&t0,NULL);
  while(sim.pc<n&&clocks<iter_max){
    /*FETCH*/
    if(instr[sim.pc].opcode==0xffffffff){
      now=bin[sim.pc];
      if(!(now^0xffffffff))
	break;
      code_fetch(now,&(ins.opcode),ins.operands);
      instr[sim.pc]=ins;
    }else{
      ins=instr[sim.pc];
    }
    tmp.opcode=ins.opcode;
    instr_type=judge_type(tmp.opcode);
    switch(instr_type){
    case TYPE_R: 
      fetch_r(&instr_r,NULL,tmp);
      break;
    case TYPE_I:
      fetch_i(&instr_i,NULL,tmp);
      break;
    case TYPE_J:
      fetch_j(&instr_j,NULL,tmp);
      break;
    }

    /*EXECUTE*/
    switch(instr_type){
    case TYPE_R: 
      flag=(*instr_r)(&sim,ins.operands[0],ins.operands[1],ins.operands[2],ins.operands[3]);
      break;
    case TYPE_I:
      flag=(*instr_i)(&sim,ins.operands[0],ins.operands[1],ins.operands[2]);
      break;
    case TYPE_J:
      flag=(*instr_j)(&sim,ins.operands[0]);
      break;
    }
    clocks++;
    if(flag<0||flag==65536){
      break;
    }
  }
  fflush(stdout);
  gettimeofday(&t,NULL);
  if(clocks>=iter_max){
    print_msg_sim_stop();
  }else if(flag<0){
    print_msg_sim_fatal();
  }else{
    print_msg_sim_finish();
  }
  print_regs(sim);
  fprintf(stderr,"clocks: %lld\n",clocks);
  print_time(subtime(t,t0));
  free(bin);
  free(instr);
  Sim_fin(sim);
  return 0;
}    


enum Flag {STEP,CONTINUE};
int step_simulation_bin(unsigned int *bin, int n) {
  Simulator sim;
  int now,i;
  Instruct ins,tmp;
  int (*instr_r)(Simulator*,int,int,int,int),(*instr_i)(Simulator*,int,int,int),(*instr_j)(Simulator*,int);
  int flag=0;
  int instr_type,stop=0;
  int ch;
  long long int clocks=0;
  Instruct *instr=(Instruct*)malloc(n*sizeof(Instruct));
  struct timeval t0,t;
  int l = 0;//何行先にブレークポイントをセットしたいか
  int signal_go=0;
  enum Flag mode=CONTINUE;
  for(i=0;i<n;i++){
    instr[i].opcode=0xfc0000ff;
  }
  Sim_Init(&sim);
  /*simulator実行部分*/
  print_msg_sim_start();
  gettimeofday(&t0,NULL);
  while(sim.pc<n&&clocks<iter_max){
    /*FETCH*/
    if(sim.pc<0){
      print_msg_sim_irraddr(sim.pc);
      flag=-1;
      break;
    }
    if((instr[sim.pc].opcode&0xfc0000ff)==0xfc0000ff){
      now=bin[sim.pc];
      if(!(now^0xffffffff))
	break;
      code_fetch(now,&(ins.opcode),ins.operands);
      if(Is_break(instr[sim.pc].opcode))
	Set_break(ins.opcode);
      instr[sim.pc]=ins;
    }else{
      ins=instr[sim.pc];
    }
    stop = Is_break(ins.opcode);
    if (stop || (mode == STEP)||clocks==0) {
      fprintf(stderr, "STEP No.%d.\n", sim.pc);
      print_regs(sim);
      fprintf(stderr, "clocks: %lld\n", clocks);
      fprintf(stderr, "next instruct: ");
      print_instr(instr[sim.pc], stderr);
      fprintf(stderr,"\n");
      signal_go=0;
      while ((ch = getchar()) != EOF&&!signal_go) {
        if (ch == '\n'); 
        else if (ch == 's') {
	  mode = STEP;
	  signal_go=1;
	}
        else if (ch == 'c'){
	  mode = CONTINUE;
	  signal_go=1;
	}
        else if (ch == 'l') {//l行先にブレークポイントをセットする（l行分実行する）
          scanf("%d", &l);
          Set_break(instr[sim.pc + l].opcode);
          fprintf(stderr,"Break point set at %d.\n",l);
        }
        else if (ch == 'b') {//l行目にブレークポイントをセットする（第b行まで実行する）
          scanf("%d", &l);
          Set_break(instr[l].opcode);
          fprintf(stderr,"Break point set at %d.\n",l);
        }
	else if (ch == 'd') {//l行目のブレークポイントを消去する
          scanf("%d", &l);
          Clear_break(instr[l].opcode);
          fprintf(stderr,"Break point at %d deleted.\n",l);
        }
	else if (ch == 'q'){ //終了
          flag=1;
	  signal_go=1;
	}
	else if (ch == 'r'){//再実行
	  Sim_fin(sim);
	  Sim_Init(&sim);
	  clocks=0;
	  signal_go=1;
	  ins=instr[0];
	}
	else if (ch == '?'){
	  console_help();
	}
        else fprintf(stderr, "Simulator: Unknown command\n");
      }

      if(ch == EOF) {
       fprintf(stderr,"step simulation cancelled.\n");
       flag=1;
      } 
    }
    if(flag!=0) break;
    tmp.opcode=ins.opcode;
    instr_type=judge_type(tmp.opcode);
    switch(instr_type){
    case TYPE_R: 
      fetch_r(&instr_r,NULL,tmp);
      break;
    case TYPE_I:
      fetch_i(&instr_i,NULL,tmp);
      break;
    case TYPE_J:
      fetch_j(&instr_j,NULL,tmp);
      break;
    }

    /*EXECUTE*/
    switch(instr_type){
    case TYPE_R: 
      flag=(*instr_r)(&sim,ins.operands[0],ins.operands[1],ins.operands[2],ins.operands[3]);
      break;
    case TYPE_I:
      flag=(*instr_i)(&sim,ins.operands[0],ins.operands[1],ins.operands[2]);
      break;
    case TYPE_J:
      flag=(*instr_j)(&sim,ins.operands[0]);
      break;
    }
    clocks++;
    if(flag<0||flag==65536){
      break;
    }
  }
  fflush(stdout);
  gettimeofday(&t,NULL);
  if(clocks>=iter_max){
    print_msg_sim_stop();
  }else if(flag<0){
    print_msg_sim_fatal();
  }else{
    print_msg_sim_finish();
  }
  print_regs(sim);
  fprintf(stderr,"clocks: %lld\n",clocks);
  print_time(subtime(t,t0));
  free(bin);
  free(instr);
  Sim_fin(sim);
  return 0;
}

static inline unsigned char bit_reverse(unsigned char a){
  return ((a&1)<<7)|((a&2)<<5)|((a&4)<<3)|((a&8)<<1)|((a&16)>>1)|((a&32)>>3)|((a&64)>>5)|((a&128)>>7);
}

int _sim_binary(int program_fd,char *output_instr_file_name){
  int n,pc=0;
  unsigned int *bin;
  register int i,j;
  register unsigned int tmp;
  FILE *output_instr_file;
  /*命令のロード*/
  n=lseek(program_fd,0,SEEK_END)>>2;
  lseek(program_fd,0,SEEK_SET);
    /*命令列出力用のファイルのオープン*/
  output_instr_file=NULL;
  if(output_instr_file_name!=NULL){
    output_instr_file=fopen(output_instr_file_name,"w");
    if(output_instr_file_name==NULL){
      fprintf(stderr,"Warning: Could not open file '%s'\n",output_instr_file_name);
    }
  }
  if(n>0){
    bin=(unsigned int*)malloc(n*sizeof(unsigned int));
    read(program_fd,bin,n<<2);
    for(i=0;i<n;i++){
      if(bin[i]^0xffffffff){
	tmp=bin[i];
	bin[i]=((unsigned int)bit_reverse((unsigned char)((tmp&0xFF000000)>>24)))|((unsigned int)bit_reverse((unsigned char)((tmp&0xFF0000)>>16))<<8)|((unsigned int)bit_reverse((unsigned char)((tmp&0xFF00)>>8))<<16)|((unsigned int)bit_reverse((unsigned char)(tmp&0xFF))<<24);
      }else{
	pc=i;
	break;
      }
    }
    if(pc==0) pc=n;
    else{
      i++;
      n_label=bin[i];
      labels=(Label*)malloc(n_label*sizeof(Label));
      i++;
      for(j=0;j<n_label;j++){
	strncpy(labels[j].name,(char*)((void*)(bin+i+1)),bin[i]);
	i+=((bin[i])>>2)+2;
	labels[j].pc=bin[i];
	i++;
      }
    }
    if(output_instr_file!=NULL){
      print_code(output_instr_file,bin,n);
      fclose(output_instr_file);
    }
    if(execute&&n>=0){
      if(debug) step_simulation_bin(bin,pc);
	else
	  simulation_bin(bin,pc);
    }    
    if(labels!=NULL)
      free(labels);
    return 0;
  }else{
    fprintf(stderr,"Error: failed to load; could not open binary file\n");
    return -1;
  }
}
