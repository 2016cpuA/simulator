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
extern Label *labels;
extern int get_pc(Label *labels,char *name_label);
/*assemble.cに定義*/
extern int make_code(int out_fd,Instruct *instr,int n);

#define Sim_Init(sim) int _i; do{ for(_i=0;_i<MEMSIZE;_i++) sim.mem[_i]=0; for(_i=0;_i<REGS;_i++){ (sim).reg[_i]=0; (sim).freg[_i]=0;}(sim).pc=0;} while(0);
#define Is_break(opcode) ((opcode)&_BREAK)
#define Clear_break(opcode) opcode=((opcode)&MASK_OP_FUN)
#define Set_break(opcode) opcode=((opcode)|_BREAK)
/*オプションから受け取った変数*/
int iter_max,debug,execute, statistics;
int binary_output;

void print_regs(Simulator sim) {
  int i;
  fprintf(stderr, "GPRs:\n");
  fprintf(stderr, "   %11d %11d %11d %11d %11d %11d %11d %11d\n--------------------------------------------------------------------------------------------------\n",0,1,2,3,4,5,6,7);
  for (i = 0; i < REGS; i++) {
    if (i % 8 == 0) fprintf(stderr, "%2d:", i);
    fprintf(stderr, "%11d ", sim.reg[i]);
    if (i % 8 == 7) fprintf(stderr,"\n");
  }
  fprintf(stderr, "\nFPRs:\n");
  fprintf(stderr, "   %11d %11d %11d %11d %11d %11d %11d %11d\n--------------------------------------------------------------------------------------------------\n",0,1,2,3,4,5,6,7);
  for (i = 0; i < FREGS; i++) {
    if (i % 8 == 0) fprintf(stderr, "%2d:", i);
    fprintf(stderr, "%11.4e ", sim.freg[i]);
    if(i % 8 == 7) fprintf(stderr, "\n");
  }

  fprintf(stderr, "PC = %d\n", sim.pc);
}

int simulation(Instruct *instr, int n){
  Simulator sim;
  Instruct now;
  int (*instr_r)(Simulator*,int,int,int,int),(*instr_i)(Simulator*,int,int,int),(*instr_j)(Simulator*,int),op[4];
  int instr_type,clocks=0;
  Sim_Init(sim);
  /*simulator実行部分*/
  fprintf(stderr,"Execution started.\n");
  while(sim.pc<n&&clocks<iter_max){
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
  if(clocks>=iter_max){
    fprintf(stderr,"Execution stopped; too long operation.\n");
  }else{
    fprintf(stderr,"Execution finished.\n");
  }
  print_regs(sim);
  fprintf(stderr,"clocks: %d\n",clocks);
  free(instr);
  return 0;
}    

enum Flag {STEP,CONTINUE};
int step_simulation(Instruct *instr, int n) {
  Simulator sim;
  Instruct now;
  int (*instr_r)(Simulator*,int,int,int,int),(*instr_i)(Simulator*,int,int,int),(*instr_j)(Simulator*,int),op[4];
  int instr_type,clocks=0,stop=0;
  int ch;
  int l = 0;//何行先にブレークポイントをセットしたいか
  int b = 0;//何行目にブレークポイントをセットしたいか
  enum Flag flag=CONTINUE;
  Sim_Init(sim);

  /*simulator実行部分*/
  fprintf(stderr,"Execution started.\n");
  while (sim.pc < n && clocks<iter_max){
    /*FETCH*/
    now=instr[sim.pc];
    stop = Is_break(now.opcode);
    if (stop || (flag == STEP)) {
      fprintf(stderr, "STEP No.%d.\n", sim.pc);
      print_regs(sim);
      fprintf(stderr, "clocks: %d\n", clocks);
      fprintf(stderr, "next instruct: ");
      print_instr(instr[sim.pc], stderr);
      fprintf(stderr,"\n");

      while ((ch = getchar()) != EOF) {
        if (ch == '\n') break;
        if (ch == 's') flag = STEP;
        else if (ch == 'c') flag = CONTINUE;
        else if (ch == 'l') {//l行先にブレークポイントをセットする（l行分実行する）
          scanf("%d", &l);
          Set_break(instr[sim.pc + l].opcode);
          flag = CONTINUE;
        }
        else if (ch == 'b') {//b行目にブレークポイントをセットする（第b行まで実行する）
          scanf("%d", &b);
          Set_break(instr[b].opcode);
          flag = CONTINUE;
        }
        else fprintf(stderr, "sim: Unknown command\n");
      }

      if(ch == EOF) {
       fprintf(stderr,"step simulation cancelled.\n");
       break;
      } 
    }

    /*実行する関数とその引数をfetch*/
    instr_type=judge_type(now.opcode);
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

int analyze(Instruct *instr, int n) {
  int occur[35] = {};
  char opcodes[35][10];
  int i, j, tmp1, tmp2;
  int index[35];

  for (i = 0; i < 35; i++) index[i] = i;


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
 

  i = 0;

  while (i < n) {
    switch (instr[i].opcode) {
      case J:
        occur[0]++;
      case JAL: 
        occur[1]++;
      case MOVE:
        occur[2]++;
      case ADDI:
        occur[3]++;
      case BEQ:
        occur[4]++;
      case BNE:
        occur[5]++;
      case LA:
        occur[6]++;
      case LW:
        occur[7]++;
      case SW:
        occur[8]++;
      case ANDI:
        occur[9]++;
      case ORI: 
        occur[10]++;
      case LWC1: 
        occur[11]++;
      case SWC1: 
        occur[12]++;
      case NOP:
        occur[13]++;
      case ADD:
        occur[14]++;
      case SUB:
        occur[15]++;
      case MULT:
        occur[16]++;
      case DIV:
        occur[17]++;
      case SLT:
        occur[18]++;
      case JR:
        occur[19]++;
      case AND:
        occur[20]++;
      case OR:
        occur[21]++;
      case XOR:
        occur[22]++;
      case SLL:
        occur[23]++;
      case SRA:
        occur[24]++;
      case SRL:
        occur[25]++;
      case ADD_S:
        occur[26]++;
      case SUB_S:
        occur[27]++;
      case MUL_S:
        occur[28]++;
      case DIV_S:
        occur[29]++;
      case C_EQ_S:
        occur[30]++;
      case C_LE_S:
        occur[31]++;
      case C_LT_S:
        occur[32]++;
      case IN:
        occur[33]++;
      case OUT: 
        occur[34]++;
      default:
        break;
    }

    i++;
  }

/*並び替え*/
  for (i = 0; i < 35; i++) {
    for (j = i + 1; j < 35; j++) {
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
  printf("\n\n---------------------\n   statistical info\n---------------------\n");

  for (i = 0; i < 35; i++) {
    printf("%8s: %2d  times\n", opcodes[index[i]], occur[i]);
  }

  return 0;
}

int _sim(int program_fd, char *output_instr_file_name, int out_binary_fd) {
  int n, written_bytes;
  Instruct *instr;
  /*命令のロード*/
  instr = load_instruct(program_fd, output_instr_file_name, &n);

  if (out_binary_fd > 0) {  //アセンブラ機能が有効のとき
    written_bytes = make_code(out_binary_fd, instr, n);
    fprintf(stderr, "%d byte written\n", written_bytes);
  }

  if (execute && n >= 0) {  //シミュレーションを実行
    if (debug) step_simulation(instr,n);  //ステップ実行
    else simulation(instr,n); //通常実行
  }

  if (statistics) analyze(instr, n);

  if (debug) free(labels);

  return 0;
}
