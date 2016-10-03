#ifndef INSTR_H
#include "simulator.h"

#define INSTR_H

#define NOP 0
#define ADD 1
#define ADDI 2
#define SUB 3
#define MUL 4
#define MULI 5
#define DIV 6
#define STR 16
#define STRI 17
#define LOAD 20
#define LOADI 21
#define INSTR_END -2147483648
int op_nop(Simulator *sim);
/*pc <- pc+1*/
int op_add(Simulator *sim,int ra,int rb,int rc);
/*Ra <- Rb+Rc; pc<-pc+1*/
int op_addi(Simulator *sim,int ra,int rb,int i);
/*Ra <- Rb+i; pc<-pc+1*/
int op_sub(Simulator *sim,int ra,int rb,int rc);
/*Ra <- Rb-Rc; pc<-pc+1*/
int op_mul(Simulator *sim,int ra,int rb,int rc);
/*Ra <- Rb*Rc; pc<-pc+1*/
int op_muli(Simulator *sim,int ra,int rb,int i);
/*Ra <- Rb*i; pc<-pc+1*/
int op_div(Simulator *sim,int ra,int rb,int rc);
/*Ra <- Rb/Rc; pc<-pc+1*/
int op_store(Simulator *sim,int rs,int addr);
/*mem[addr] <- rs; pc<-pc+1;*/
int op_store_indir(Simulator *sim,int rs, int raddr);
/*mem[Raddr] <- rs; pc<-pc+1*/
int op_load(Simulator *sim,int rd,int addr);
/*Rd <- mem[addr]; pc<-pc+1*/
int op_load_indir(Simulator *sim,int rd,int raddr);
/*Rd <- mem[Raddr]; pc<-pc+1*/

int interpret_instr(Instruct* instr,char *name,char *op1,char *op2,char *op3,char *op4);

int execute_instr(Simulator* sim,Instruct instr);

#endif
