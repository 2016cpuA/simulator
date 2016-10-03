#ifndef INSTR_H
#include "simulator.h"

#define INSTR_H

/* ここに命令セット */
/*opcode+function記述箇所*/
#define NOP 0
#define ADD 0x20
#define ADDI 0x2000003F
#define SUB 0x1A
#define MULT 0x18
#define DIV 0x22
#define SLT 0x2A
#define BEQ 0x1000003F
#define BNE 0x1400003F
#define J 0x0800003F
#define JAL 0x0C00003F
#define JR 8
#define LW 0x8C00003F
#define SW 0xAC00003F
#define AND 0x24

#define INSTR_END -2147483648

/*オペランド指定用;今のところreadline.cのみで使用;構文チェック用*/
#define UNKNOWN -1
#define ERROR -2
#define REG 1
#define FREG 2
#define IMMIDIATE 4
#define NONE 0

/*命令実行用の関数のプロトタイプ宣言(まだ仮置きのまま)
  実体はinstructs.cに記述*/
int instr_nop(Simulator *sim);
/*pc <- pc+1*/
int instr_add(Simulator *sim,int ra,int rb,int rc);
/*Ra <- Rb+Rc; pc<-pc+1*/
int instr_addi(Simulator *sim,int ra,int rb,int i);
/*Ra <- Rb+i; pc<-pc+1*/
int instr_sub(Simulator *sim,int ra,int rb,int rc);
/*Ra <- Rb-Rc; pc<-pc+1*/
int instr_mul(Simulator *sim,int ra,int rb,int rc);
/*Ra <- Rb*Rc; pc<-pc+1*/
int instr_muli(Simulator *sim,int ra,int rb,int i);
/*Ra <- Rb*i; pc<-pc+1*/
int instr_div(Simulator *sim,int ra,int rb,int rc);
/*Ra <- Rb/Rc; pc<-pc+1*/
int instr_store(Simulator *sim,int rs,int addr);
/*mem[addr] <- rs; pc<-pc+1;*/
int instr_store_indir(Simulator *sim,int rs, int raddr);
/*mem[Raddr] <- rs; pc<-pc+1*/
int instr_load(Simulator *sim,int rd,int addr);
/*Rd <- mem[addr]; pc<-pc+1*/
int instr_load_indir(Simulator *sim,int rd,int raddr);
/*Rd <- mem[Raddr]; pc<-pc+1*/

/*命令名を読み取るための関数
  実体はinstructs.cに記述*/
int get_instr(char *name);


/*その命令で行う操作を記述する関数*/
int execute_instr(Simulator* sim,Instruct instr);

#endif
