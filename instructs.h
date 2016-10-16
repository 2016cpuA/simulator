#ifndef INSTR_H
#include "simulator.h"
#include <stdio.h>
#define INSTR_H

/* ここに命令セット */
/*命令識別用番号記述箇所*/
/*上位6bitをopcode,下位6bitをfunction(形式R以外は0),それ以外は0にする*/
#define ADD 0x20
#define ADDI 0x20000000
#define SUB 0x1A
#define MULT 0x18
#define DIV 0x22
#define SLT 0x2A
#define BEQ 0x10000000
#define BNE 0x14000000
#define J 0x08000000
#define JAL 0x0C000000
#define JR 8
#define LW 0x8C000000
#define SW 0xAC000000
#define AND 0x24
#define ANDI 0x30000000
#define OR 0x25
#define ORI 0x34000000
#define XOR 0x26
#define SLL 0
#define SRA 3
#define SRL 2
#define IN 0xEC000000
#define OUT 0xF0000000
#define NOP 0xFC00003F
#define MOVE 0xFE000000
/*疑似命令,デバッグ用*/
#define MASK_PSEUDO 0x02000000

/*バイナリコード(4byte)から要素を抜き出すためのマクロ*/
#define Fetch_opcode(code) (((code)&0xFC000000)>>26)
#define Fetch_rs(code) (((code)&0x3E00000)>>21)
#define Fetch_rt(code) (((code)&0x1F0000)>>16)
#define Fetch_rd(code) (((code)&0xF800)>>11)
#define Fetch_sa(code) (((code)&0x7C0)>>6)
#define Fetch_function(code) ((code)&0x3F)
#define Fetch_immediate(code) ((code)&0xFFFF)
#define Fetch_instr_index(code) ((code)&0x3FFFFFF)



#define INSTR_END -2147483648

/*オペランド指定用のマクロ;今のところreadline.cのみで使用;構文チェック用*/
#define UNKNOWN -1
#define ERROR -2
#define REG 1
#define FREG 2
#define IMMIDIATE 4
#define NONE 0

/*命令実行用の関数のプロトタイプ宣言(まだ仮置きのまま)
  実体はinstructs.cに記述*/

/*形式Rの命令*/
int instr_clear(Simulator *sim,int rs,int rt,int rd,int sa);
/*レジスタの初期化; pc++*/
int instr_nop(Simulator *sim,int rs,int rt,int rd,int sa);
/*pc++*/
int instr_add(Simulator *sim,int rs,int rt,int rd,int sa);
/*rd <- rs+rt; pc++*/
int instr_sub(Simulator *sim,int rs,int rt,int rd,int sa);
/*rd <- rs-rt; pc++*/
int instr_mult(Simulator *sim,int rs,int rt,int rd,int sa);
/*rd <- (rs*rt)[31:0]; pc++*/
int instr_div(Simulator *sim,int rs,int rt,int rd,int sa);
/*rd <- rs/rt; pc++*/
int instr_slt(Simulator *sim,int rs,int rt,int rd,int sa);
/*rd <- (if rs<rt then 1 else 0); pc++*/
int instr_jr(Simulator *sim,int rs,int rt,int rd,int sa);
/*pc <- rs*/
int instr_and(Simulator *sim,int rs,int rt,int rd,int sa);
/*rd <- rs&rt; pc++*/
int instr_or(Simulator *sim,int rs,int rt,int rd,int sa);
/*rd <- rs|rt; pc++*/
int instr_xor(Simulator *sim,int rs,int rt,int rd,int sa);
/*rd <- rs^rt; pc++*/
int instr_sll(Simulator *sim,int rs,int rt,int rd,int sa);
/*rd <- rt<<sa; pc++*/
int instr_sra(Simulator *sim,int rs,int rt,int rd,int sa);
/*rd <- rs>>sa (arithmetic); pc++*/
int instr_srl(Simulator *sim,int rs,int rt,int rd,int sa);
/*rd <- rs>>rt; pc++*/
int instr_in(Simulator *sim,int rs,int rt,int rd,int sa);
/*実際の動作について説明求む*/
int instr_out(Simulator *sim,int rs,int rt,int rd,int sa);
/*実際の動作について説明求む*/

/*形式Iの命令*/
int instr_move(Simulator *sim,int rs,int rt,int imm);
/*rt < -rs;pc++*/
int instr_addi(Simulator *sim,int rs,int rt,int imm);
/*rt <- rs+imm; pc++*/
int instr_andi(Simulator *sim,int rs,int rt,int imm);
/*rt <- rs&imm; pc++*/
int instr_ori(Simulator *sim,int rs,int rt,int imm);
/*rt <- rs|imm; pc++*/
int instr_beq(Simulator *sim,int rs,int rt,int offset);
/*if rs=rt then pc<-pc+offset else pc++*/
int instr_bne(Simulator *sim,int rs,int rt,int offset);
/*if rs!=rt then pc<-pc+offset else pc++*/
int instr_lw(Simulator *sim,int rbase,int rt,int offset);
/*rt <- mem[rbase+offset]; pc++*/
int instr_sw(Simulator *sim,int rbase,int rt,int offset);
/*mem[rbase+offset] <- rt; pc++*/

/*形式Jの命令*/
int instr_jal(Simulator *sim,int instr_index);
/*r31<-pc+8; pc<-pc[31:28]|(instr_index<<2)*/
int instr_j(Simulator *sim,int instr_index);
/*pc<-pc[31:28]|(instr_index<<2)*/


/*アセンブラの命令名を読み取るための関数
  実体はinstructs.cに記述*/
int get_instr(char *name);
/*命令の識別番号からアセンブラの命令名を読み取り、指定したファイルストリームに出力*/
void print_instr(Instruct instr,FILE* out_file);

/*以下はfetch.cに定義されている*/
/*命令の形式を取得する関数*/
int judge_type(int opcode);
#define TYPE_R 1
#define TYPE_I 2
#define TYPE_J 3
#define UNKNOWN_OP -1

/*Instruct型のデータから、実行する関数とその引数を返す関数*/
int fetch_r(int (**instr)(Simulator*,int,int,int,int),int op[4],Instruct ins);
int fetch_i(int (**instr)(Simulator*,int,int,int),int op[4],Instruct ins);
int fetch_j(int (**instr)(Simulator*,int),int op[4],Instruct ins);
/*バイナリコードから、命令の種類と引数を取り出す関数*/
int code_fetch(int code,int *opcode,int op[4]);

/*命令をバイナリに変換する関数*/
int make_code_r(int opcode,int rs,int rt,int rd,int sa);
int make_code_i(int opcode,int rs,int rt,int imm);
int make_code_j(int opcode,int instr_index);
#endif
