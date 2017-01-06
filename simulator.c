#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "simulator.h"
#include "instructs.h"
#include "queue.h"
#include "message.h"
#include <sys/time.h>
/*readline.cに定義*/
extern Instruct *load_instruct(int fd,char* output_instr_file_name,int *size);
extern Label *labels;
extern int get_pc(Label *labels,char *name_label);
/*assemble.cに定義*/
extern int make_code(int out_fd,Instruct *instr,int n);
extern int make_code_another(int out_fd,Instruct *instr,int n);
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
#define Sim_fin(sim) do{ free((sim).mem);} while(0);

#define Is_break(opcode) ((opcode)&_BREAK)
#define Clear_break(opcode) opcode=((opcode)&MASK_OP_FUN)
#define Set_break(opcode) opcode=((opcode)|_BREAK)
/*オプションから受け取った変数*/
int debug,execute, statistics;
int binary_output;
long long int iter_max;
union i_f{
  int i;
  float f;
};
extern inline void print_regs(Simulator sim) {
  int i;
  union i_f tmp;
  fprintf(stderr, "GPRs:\n");
  fprintf(stderr, "    %22d %22d %22d %22d\n-----------------------------------------------------------------------------------------------\n",0,1,2,3);
  for (i = 0; i < REGS; i++) {
    if (i % 4 == 0) fprintf(stderr, "%2d:", i);
    fprintf(stderr, " \e[35m0x%08x \e[m%11d", sim.reg[i],sim.reg[i]);
    if (i % 4 == 3) fprintf(stderr,"\n");
  }
  fprintf(stderr, "\nFPRs:\n");
  fprintf(stderr, "    %22d %22d %22d %22d\n-----------------------------------------------------------------------------------------------\n",0,1,2,3);
  for (i = 0; i < FREGS; i++) {
    if (i % 4 == 0) fprintf(stderr, "%2d:", i);
    tmp.f=sim.freg[i];
    fprintf(stderr, " \e[35m0x%08x \e[m%11.4e", tmp.i,tmp.f);
    if(i % 4 == 3) fprintf(stderr, "\n");
  }

  fprintf(stderr, "PC = %d\n", sim.pc);
}

static inline double subtime(struct timeval t,struct timeval t0){
  return (double)(t.tv_sec-t0.tv_sec)+(double)(t.tv_usec-t0.tv_usec)*0.000001;
}
extern inline void print_time(double t_sec){
  int hour,min,sec,usec;
  fprintf(stderr,"time:");
  if(t_sec>=0){
    hour=((int)t_sec)/3600;
    min=(((int)t_sec)/60)%60;
    sec=((int)t_sec)%60;
    usec=(int)(t_sec*1000000)%1000000;
    fprintf(stderr," %02dh %02dm %02d.%06ds\n",hour,min,sec,usec);
  }else{
    fprintf(stderr," Could not measure.\n");
  }
}

#define NUM_INSTR 35
#define NUM_INSTR_LIBFUN NUM_INSTR+NUM_SIM_SYMBOLS

static inline void stat_init(int index[NUM_INSTR_LIBFUN],char opcodes[NUM_INSTR_LIBFUN][32]){
  int i;
  for (i = 0; i < NUM_INSTR_LIBFUN; i++) index[i] = i;
  strcpy(opcodes[0], "J");
  strcpy(opcodes[1], "JAL");
  strcpy(opcodes[2], "MOVE");
  strcpy(opcodes[3], "ADDI");
  strcpy(opcodes[4], "BEQ");
  strcpy(opcodes[5], "BNE");
  strcpy(opcodes[6], "LA");
  strcpy(opcodes[7], "LW");
  strcpy(opcodes[8], "SW");
  strcpy(opcodes[9], "ANDI");
  strcpy(opcodes[10], "ORI");
  strcpy(opcodes[11], "LWC1");
  strcpy(opcodes[12], "SWC1");
  strcpy(opcodes[13], "NOP");
  strcpy(opcodes[14], "ADD");
  strcpy(opcodes[15], "SUB");
  strcpy(opcodes[16], "MULT");
  strcpy(opcodes[17], "DIV");
  strcpy(opcodes[18], "SLT");
  strcpy(opcodes[19], "JR");
  strcpy(opcodes[20], "AND");
  strcpy(opcodes[21], "OR");
  strcpy(opcodes[22], "XOR");
  strcpy(opcodes[23], "SLL");
  strcpy(opcodes[24], "SRA");
  strcpy(opcodes[25], "SRL");
  strcpy(opcodes[26], "ADD_S");
  strcpy(opcodes[27], "SUB_S");
  strcpy(opcodes[28], "MUL_S");
  strcpy(opcodes[29], "DIV_S");
  strcpy(opcodes[30], "C_EQ_S");
  strcpy(opcodes[31], "C_LE_S");
  strcpy(opcodes[32], "C_LT_S");
  strcpy(opcodes[33], "IN");
  strcpy(opcodes[34], "OUT");
  strcpy(opcodes[NUM_INSTR], "print_char");
  strcpy(opcodes[NUM_INSTR+1], "print_int");
  strcpy(opcodes[NUM_INSTR+2], "print_float");
  strcpy(opcodes[NUM_INSTR+3], "print_newline");
  strcpy(opcodes[NUM_INSTR+4], "read_byte");
  strcpy(opcodes[NUM_INSTR+5], "read_int");
  strcpy(opcodes[NUM_INSTR+6], "read_float");
  strcpy(opcodes[NUM_INSTR+7], "sin");
  strcpy(opcodes[NUM_INSTR+8], "cos");
  strcpy(opcodes[NUM_INSTR+9], "atan");
  strcpy(opcodes[NUM_INSTR+10], "float_of_int");
  strcpy(opcodes[NUM_INSTR+11], "int_of_float");
  strcpy(opcodes[NUM_INSTR+12], "floor");
  strcpy(opcodes[NUM_INSTR+13], "sqrt");
  strcpy(opcodes[NUM_INSTR+14], "fabs");
  strcpy(opcodes[NUM_INSTR+15], "create_array");
  strcpy(opcodes[NUM_INSTR+16], "create_array_float");
  strcpy(opcodes[NUM_INSTR+17], "fiszero");
  strcpy(opcodes[NUM_INSTR+18], "fispos");
  strcpy(opcodes[NUM_INSTR+19], "fisneg");
  strcpy(opcodes[NUM_INSTR+20], "fneg");
  strcpy(opcodes[NUM_INSTR+21], "fsqr");
  strcpy(opcodes[NUM_INSTR+22], "fless");
  strcpy(opcodes[NUM_INSTR+23], "fhalf");
}
static inline void stat_libfun(int libfun,long long int occur[NUM_INSTR_LIBFUN]){
  switch (libfun){
  case LIB_WBYTE:
    occur[NUM_INSTR+0]++;break;
  case LIB_WINT:
    occur[NUM_INSTR+1]++;break;
  case LIB_WFLOAT:
    occur[NUM_INSTR+2]++;break;
  case LIB_NLINE:
    occur[NUM_INSTR+3]++;break;
  case LIB_RBYTE:
    occur[NUM_INSTR+4]++;break;
  case LIB_RINT:
    occur[NUM_INSTR+5]++;break;
  case LIB_RFLOAT:
    occur[NUM_INSTR+6]++;break;
  case LIB_SIN:
    occur[NUM_INSTR+7]++;break;
  case LIB_COS:
    occur[NUM_INSTR+8]++;break;
  case LIB_ATAN:
    occur[NUM_INSTR+9]++;break;
  case LIB_ITOF:
    occur[NUM_INSTR+10]++;break;
  case LIB_FTOI:
    occur[NUM_INSTR+11]++;break;
  case LIB_FLOOR:
    occur[NUM_INSTR+12]++;break;
  case LIB_SQRT:
    occur[NUM_INSTR+13]++;break;
  case LIB_FABS:
    occur[NUM_INSTR+14]++;break;
  case LIB_CR_ARRAY:
    occur[NUM_INSTR+15]++;break;
  case LIB_CR_ARRAY_F:
    occur[NUM_INSTR+16]++;break;
  case LIB_F_IS_0:
    occur[NUM_INSTR+17]++;break;
  case LIB_F_IS_POS:
    occur[NUM_INSTR+18]++;break;
  case LIB_F_IS_NEG:
    occur[NUM_INSTR+19]++;break;
  case LIB_F_NEG:
    occur[NUM_INSTR+20]++;break;
  case LIB_F_SQR:
    occur[NUM_INSTR+21]++;break;
  case LIB_F_LESS:
    occur[NUM_INSTR+22]++;break;
  case LIB_F_HALF:
    occur[NUM_INSTR+23]++;break;
  }
}
static inline void stat_do(Instruct ins,long long int occur[NUM_INSTR_LIBFUN]){ 
  switch (ins.opcode){
  case J:
    occur[0]++; if(ins.operands[0]>0x1000000) stat_libfun(ins.operands[0],occur);break;
  case JAL: 
    occur[1]++;if(ins.operands[0]>0x1000000) stat_libfun(ins.operands[0],occur);break;
  case MOVE:
    occur[2]++;break;
  case ADDI:
    occur[3]++;break;
  case BEQ:
    occur[4]++;break;
  case BNE:
    occur[5]++;break;
  case LA:
    occur[6]++;break;
  case LW:
    occur[7]++;break;
  case SW:
    occur[8]++;break;
  case ANDI:
    occur[9]++;break;
  case ORI: 
    occur[10]++;break;
  case LWC1: 
    occur[11]++;break;
  case SWC1: 
    occur[12]++;break;
  case NOP:
    occur[13]++;break;
  case ADD:
    occur[14]++;break;
  case SUB:
    occur[15]++;break;
  case MULT:
    occur[16]++;break;
  case DIV:
    occur[17]++;break;
  case SLT:
    occur[18]++;break;
  case JR:
    occur[19]++;break;
  case AND:
    occur[20]++;break;
  case OR:
    occur[21]++;break;
  case XOR:
    occur[22]++;break;
  case SLL:
    occur[23]++;break;
  case SRA:
    occur[24]++;break;
  case SRL:
    occur[25]++;break;
  case ADD_S:
    occur[26]++;break;
  case SUB_S:
    occur[27]++;break;
  case MUL_S:
    occur[28]++;break;
  case DIV_S:
    occur[29]++;break;
  case C_EQ_S:
    occur[30]++;break;
  case C_LE_S:
    occur[31]++;break;
  case C_LT_S:
    occur[32]++;break;
  case IN:
    occur[33]++;break;
  case OUT: 
    occur[34]++;break;
  default:
    break;
  }
}

static inline void stat_fin(long long int occur[NUM_INSTR_LIBFUN],int index[NUM_INSTR_LIBFUN],char opcodes[NUM_INSTR_LIBFUN][32]){
  int i,j,tmp1;
  long long int n_instr=0,tmp2;
  /*並び替え*/
  for (i = 0; i < NUM_INSTR; i++) {
    for (j = i + 1; j < NUM_INSTR; j++) {
      if (occur[i] <= occur[j]) {
        tmp1 = index[i];
        tmp2 = occur[i];
        index[i] = index[j];
        occur[i] = occur[j];
        index[j] = tmp1;
        occur[j] = tmp2;
      }
    }
  }
  for (i=NUM_INSTR; i < NUM_INSTR_LIBFUN; i++) {
    for (j = i + 1; j < NUM_INSTR_LIBFUN; j++) {
      if (occur[i] <= occur[j]) {
        tmp1 = index[i];
        tmp2 = occur[i];
        index[i] = index[j];
        occur[i] = occur[j];
        index[j] = tmp1;
        occur[j] = tmp2;
      }
    }
  }
  /*出力*/
  fprintf(stderr,"\n---------------------\n   statistical info\n---------------------\n");
  fprintf(stderr,"instructs::\n");
  for (i = 0; i < NUM_INSTR; i++) {
    if(occur[i]!=0)
      fprintf(stderr,"%8s: %10lld  times\n", opcodes[index[i]], occur[i]);
    else
      break;
    n_instr+=occur[i];
  }
  fprintf(stderr,"total: %10lld instructs\n\n",n_instr);
  fprintf(stderr,"library functions:\n");
  n_instr=0;
  for (i = NUM_INSTR; i < NUM_INSTR_LIBFUN; i++) {
    if(occur[i]!=0)
      fprintf(stderr,"%20s: %10lld  times\n", opcodes[index[i]], occur[i]);
    else
      break;
     n_instr+=occur[i];
  }
  fprintf(stderr,"total: %10lld functions\n\n",n_instr);
}
		    
static inline int is_float(unsigned int opcode){
  return ((opcode&0xFC000000)==0x44000000)||((opcode&0xFC000000)==0xc4000000);
}
int simulation(Instruct *instr, int n){
  Simulator sim;
  Instruct now;
  int (*instr_r)(Simulator*,int,int,int,int),(*instr_i)(Simulator*,int,int,int),(*instr_j)(Simulator*,int),op[4];
  int instr_type;
  long long int clocks=0;
  int flag=0;
  long long int occur[NUM_INSTR_LIBFUN] = {};
  char opcodes[NUM_INSTR_LIBFUN][32];
  int index[NUM_INSTR_LIBFUN];
  struct timeval t0,t;
  if(statistics) 
    stat_init(index,opcodes);
  Sim_Init(&sim);
  /*simulator実行部分*/
  print_msg_sim_start();
  gettimeofday(&t0,NULL);
  while(sim.pc<n&&clocks<iter_max){
    /*FETCH*/
    if(sim.pc<0){
      print_msg_sim_irraddr(sim.pc);
      fprintf(stderr,"Maybe caused by instruction");
      print_instr(now, stderr);
      fprintf(stderr,"\n");
      flag=-1;
      break;
    }
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
      flag=(*instr_r)(&sim,op[0],op[1],op[2],op[3]);
      break;
      case TYPE_I:
      flag=(*instr_i)(&sim,op[0],op[1],op[2]);
      break;
      case TYPE_J:
      flag=(*instr_j)(&sim,op[0]);
      break;
    }
    if(statistics) 
      stat_do(now,occur);
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
  if(statistics) 
    stat_fin(occur,index,opcodes);
  print_time(subtime(t,t0));
  free(instr);
  Sim_fin(sim);
  return 0;
}    

extern inline void console_help(){
  fprintf(stderr,"commands\n");
  fprintf(stderr,"c\tContinue; continue execution.\n");
  fprintf(stderr,"s\tStep; execute next instruction.\n");
  fprintf(stderr,"l N\tI don't know what to say;tset breakpoint at 'PC=N+CurrentPC'.\n");
  fprintf(stderr,"b N\tBreak; set breakpoint at 'PC=N'.\n");
  fprintf(stderr,"d N\tDelete breakpoint; Delete breakpoint at 'PC=N'.\n");
  fprintf(stderr,"q\tQuit; quit execution immediately.\n");
  fprintf(stderr,"r\tRestart; Restart from beginning.\n");
  fprintf(stderr,"?\thelp; show this message.\n");
}

enum Flag {STEP,CONTINUE};
int step_simulation(Instruct *instr, int n) {
  Simulator sim;
  Instruct now;
  int (*instr_r)(Simulator*,int,int,int,int),(*instr_i)(Simulator*,int,int,int),(*instr_j)(Simulator*,int),op[4];
  int instr_type,stop=0,breaker=0;
  long long int clocks=0;
  int ch;
  long long int occur[NUM_INSTR_LIBFUN] = {};
  char opcodes[NUM_INSTR_LIBFUN][32];
  int index[NUM_INSTR_LIBFUN];
  Queue_int prev_access[REGS]={};
  Queue_int prev_access_f[REGS]={};
  Queue_hist prev_access_mem;
  int l = 0;//何行先にブレークポイントをセットしたいか
  int flag_float,signal_go=0;
  enum Flag flag=CONTINUE;
  Sim_Init(&sim);
  int i;
  for(i=0;i<REGS;i++){
    qi_init(&prev_access[i]);
    qi_init(&prev_access[i]);
  }
  qh_init(&prev_access_mem);
  if(statistics) 
    stat_init(index,opcodes);
  /*simulator実行部分*/
  print_msg_sim_start();
  while (sim.pc < n && clocks<iter_max){
    /*FETCH*/
    if(sim.pc<0){
      print_msg_sim_irraddr(sim.pc);
      fprintf(stderr,"Maybe caused by instruction ");
      print_instr(now, stderr);
      fprintf(stderr,"\n");
      flag=-1;
      break;
    }
    now=instr[sim.pc];
    stop = Is_break(now.opcode);
    if (stop || (flag == STEP)||clocks==0) {
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
	  flag = STEP;
	  signal_go=1;
	}
        else if (ch == 'c'){
	  flag = CONTINUE;
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
          breaker=1;
	  signal_go=1;
	}
	else if (ch == 'r'){//再実行
	  Sim_fin(sim);
	  Sim_Init(&sim);
	  for(i=0;i<REGS;i++){
	    qi_init(&prev_access[i]);
	    qi_init(&prev_access[i]);
	  }
	  clocks=0;
	  signal_go=1;
	  now=instr[sim.pc];
	}
	else if (ch == '?'){
	  console_help();
	}
        else fprintf(stderr, "Simulator: Unknown command\n");
      }

      if(ch == EOF) {
       fprintf(stderr,"step simulation cancelled.\n");
       breaker=1;
      } 
    }
    if(breaker!=0) break;
    /*実行する関数とその引数をfetch*/
    instr_type=judge_type(now.opcode);
    flag_float=is_float(now.opcode);
    switch (instr_type) {
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
    
    /*命令を実行*/
    switch (instr_type) {
      case TYPE_R:
	if(flag_float){
	  qi_enqueue(&prev_access_f[op[2]],sim.pc);
	}else{
	  qi_enqueue(&prev_access[op[2]],sim.pc);
	}
        breaker=(*instr_r)(&sim,op[0],op[1],op[2],op[3]);
	break;
      case TYPE_I:
        qi_enqueue(&prev_access[op[1]],sim.pc);
        breaker=(*instr_i)(&sim,op[0],op[1],op[2]);
	break;
      case TYPE_J:
        breaker=(*instr_j)(&sim,op[0]);
	break;
    }
    if(statistics) 
      stat_do(now,occur);
    clocks++;
    if(breaker<0||breaker==65536){
      break;
    }
  }
  if(clocks>=iter_max){
    print_msg_sim_stop();
  }else if(breaker<0){
    print_msg_sim_fatal();
    if(breaker+64<REGS&&flag_float){
      fprintf(stderr,"Maybe caused by instruction ");
      print_instr(instr[qi_gettail(&prev_access_f[breaker+64])], stderr);
      fprintf(stderr," (index %d)\n",qi_gettail(&prev_access_f[breaker+64]));
    }else if(breaker+64<REGS){
      fprintf(stderr,"Maybe caused by instruction ");
      print_instr(instr[qi_gettail(&prev_access[breaker+64])], stderr);
      fprintf(stderr," (index %d)\n",qi_gettail(&prev_access[breaker+64]));
    }
  }else{
    print_msg_sim_finish();
  }
  print_regs(sim);
  fprintf(stderr,"clocks: %lld\n",clocks);
  if(statistics) 
    stat_fin(occur,index,opcodes);
  free(instr);
  Sim_fin(sim);
  return 0;
}


int _sim(int program_fd, char *output_instr_file_name, int out_binary_fd,int system_verilog_fd) {
  int n, written_bytes;
  Instruct *instr;
  /*命令のロード*/
  instr = load_instruct(program_fd, output_instr_file_name, &n);

  if (out_binary_fd > 0) {  //アセンブラ機能が有効のとき
    written_bytes = make_code(out_binary_fd, instr, n);
    fprintf(stderr, "%d byte written\n", written_bytes);
  }
  if (system_verilog_fd > 0) {  //SystemVerilog出力機能が有効のとき
    written_bytes = make_code_another(system_verilog_fd, instr, n);
    fprintf(stderr, "%d byte written\n", written_bytes);
  }
  if (execute && n >= 0) {  //シミュレーションを実行
    if (debug) step_simulation(instr,n);  //ステップ実行
    else simulation(instr,n); //通常実行
  }
  if (debug) free(labels);

  return 0;
}
