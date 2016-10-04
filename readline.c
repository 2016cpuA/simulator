#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "simulator.h"
#include "instructs.h"
#include "list.h"

typedef struct label{
  char name[100];
  int pc;
} Label;

int get_pc(Label *labels,int n_label,char *name_label){
  int i;
  for(i=0;i<n_label;i++){
    if(!strcmp(labels[i].name,name_label)){
      return labels[i].pc;
    }
  }
  return -1;
}

void print_labels(Label *labels,int n_label){
  int i;
  printf("label\tpc\n");
  for(i=0;i<n_label;i++){
    printf("%s\t%d\n",labels[i].name,labels[i].pc);
  }
}

int isnum(char ch){
  return ((ch>='0')&&(ch<='9'))||(ch=='-');
}

int search(char t,char buf[],int bufsize){
  int i;
  for(i=0;i<bufsize;i++){
    if(buf[i]==t)
      return i;
  }
  return -1;
}

int count(char t,char buf[], int bufsize){
  int i,c=0;
  for(i=0;i<bufsize;i++){
    if(buf[i]==t)
      c++;
  }
  return c;
}

int get_operand(char *op,int type_op,int no,Label *labels,int n_label,int opcode){
  int pc;
  if(isnum(op[0])){
    if (!(type_op&&IMMIDIATE))
      printf("Warning(line %d): wrong operand type\n",no); 
    return atoi(op);
  }else if(op[0]=='%'){
    if(op[1]=='r'){
      if (!(type_op&&REG))
	printf("Warning(line %d): wrong operand type\n",no);  
      return atoi(op+2);
    }else if(op[1]=='f'){
      if(!(type_op&&FREG))
        printf("Warning(line %d): wrong operand type\n",no); 
      return atoi(op+2);
    }else{
      printf("Error(line %d): unkown operand '%s'\n",no,op);
    }
  }else if(op[0]=='$'){
    if (!(type_op&&IMMIDIATE))
      printf("Warning(line %d): wrong operand type\n",no); 
    return atoi(op+1);
  }else if((pc=get_pc(labels,n_label,op))>=0){
    if((opcode==J)||(opcode==JAL))
      return (pc>>2);
    else
      return (pc>>2)-no;
  }else{
    printf("Error(line %d): unkown operand '%s'\n",no,op);
  }
  return ERROR;
}

#define EMPTY_LINE 1
#define NO_ERROR 0
#define SYNTAX_ERROR -1

int search_space(char* buf,int bufsize){
  int i;
  for(i=0;i<bufsize;i++){
    if(buf[i]==' '||buf[i]=='\t')
      return i;
  }
  return -1;
}

int search_delim(char* buf,int bufsize){
  int i;
  for(i=0;i<bufsize;i++){
    if(buf[i]==','||buf[i]=='(')
      return i;
  }
  return -1;
}

int interpret(Instr_list *instr_l,char *buf,int bufsize,int no,Label *labels,int n_label){
  char *buf_cp=(char*)malloc(sizeof(char)*(bufsize+1));
  char *now=buf_cp;
  Instruct *instr_read=(Instruct*)malloc(sizeof(Instruct));
  int i,rest=bufsize-1,pos_delim,err=NO_ERROR;
  char opcode[16],operands[4][32];
  Instr_list *last=instr_l;
  strcpy(buf_cp,buf);
  for(i=0;i<4;i++)
    instr_read->operands[i]=0;
  /*'!' から先はコメント*/
  if((pos_delim=search('!',now,rest))>=0){
    now[pos_delim]=0;
    rest=pos_delim;
  }
  while((*now==' '||*now=='\t')&&rest>0){
    rest--;
    now++;
  }
  /*|[label:] <op> a1,a2,a3
    |[label:] <op> a1,a2(a3) */
  /*ここではラベルは読み飛ばしてよい*/
  if((pos_delim=search(':',now,rest))>=0){
    now+=pos_delim+1;
    rest-=pos_delim+1;
  }
  while((*now==' '||*now=='\t')&&rest>0){
    rest--;
    now++;
  }
  if(rest>0){
    pos_delim=search_space(now,rest);
    strncpy(opcode,now,pos_delim);
    opcode[pos_delim]=0;
    instr_read->opcode=get_instr(opcode);
    now+=pos_delim+1;
    rest-=pos_delim+1;
    while((*now==' '||*now=='\t')&&rest>0){
      rest--;
      now++;
    }
    /* <op> |a1,a2,a3
       <op> |a1,a2(a3) */
    i=0;
    while(i<4&&rest>0){
      pos_delim=search_delim(now,rest);
      if (pos_delim<0){
	if(rest==1) break;
	pos_delim=rest;
      }
      strncpy(operands[i],now,pos_delim);
      operands[i][pos_delim]=0;
      instr_read->operands[i]=get_operand(operands[i],7,no,labels,n_label,instr_read->opcode);
      if(instr_read->operands[i]==-2){
	err=SYNTAX_ERROR;
      }
      i++;
      now+=pos_delim+1;
      rest-=pos_delim+1;
      if(now[pos_delim]=='('){
	while((*now==' '||*now=='\t')&&rest>0){
	  rest--;
	  now++;
	}
	if(rest<=0){
	  printf("Error(line %d): expected ')'",no);
	  err=SYNTAX_ERROR;
	  break;
	}
	pos_delim=search(')',now,rest);
	strncpy(operands[i],now,pos_delim);
	instr_read->operands[i]=get_operand(operands[i],7,no,labels,n_label,instr_read->opcode);
	if(instr_read->operands[i]==-2){
	  err=SYNTAX_ERROR;
	}
	i++;
	now+=pos_delim+1;
	rest-=pos_delim+1;
      }
      while((*now==' '||*now=='\t')&&rest>0){
	rest--;
	now++;
      }
    }
    while(!list_isempty(last))
      last=last->next;
    last->no = no;
    list_push(last,instr_read);
    free(buf_cp);
  }else{
    free(buf_cp);
    free(instr_read);
    err=EMPTY_LINE;
  }
  return err;
}

int count_labels(int fd){
  char buf[1024],tmp[65536];
  char text[65536];
  int c=0,i=0,l=1;
  int pos_lf;
  /*実行に必要な変数の定義*/
  int colons=0;
  /*ここまで*/
  text[0]=0;
  while((c=read(fd,buf,1023))>0){
    buf[c]=0;
    strcat(text,buf);
    i+=c;
    while((pos_lf=search('\n',text,i))>=0){
      text[pos_lf]=0;
      /*各行に対し実行する関数*/
      if(search(':',text,pos_lf)>=0)
	colons++;
      /*ここまで*/
      if(i>pos_lf)
	strcpy(tmp,text+pos_lf+1);
      else
	tmp[0]=0;
      strcpy(text,tmp);
      i=i-pos_lf-1;
      l++;
    }
  }
  /*実行の最後の処理*/
  lseek(fd,0,SEEK_SET);
  /*ここまで*/
  return colons;
}

void search_labels(int fd,Label* labels){
  char buf[1024],tmp[65536];
  char text[65536];
  int c=0,i=0,l=0;
  int pos_lf;
  /*実行に必要な変数の定義*/
  char *now;
  int pos_colon;
  int c_label=0;
  /*ここまで*/
  text[0]=0;
  while((c=read(fd,buf,1023))>0){
    buf[c]=0;
    strcat(text,buf);
    i+=c;
    while((pos_lf=search('\n',text,i))>=0){
      text[pos_lf]=0;
      /*各行に対し実行する処理*/
      strcpy(tmp,text);
      if((pos_colon=search(':',tmp,pos_lf))>0){
	tmp[pos_colon]=0;
	strcpy(labels[c_label].name,tmp);
	labels[c_label].pc=l<<2;
	c_label++;
      }
      if(pos_colon>=0){
	now=tmp+pos_colon+1;
      }else{
	now=tmp;
      }
      /*命令の無い行か?*/
      while(*now==' '||*now=='\t'){
	now++;
      }
      if (*now!=0){
	l++;
      }
      /*ここまで*/
      if(i>pos_lf)
	strcpy(tmp,text+pos_lf+1);
      else
	tmp[0]=0;
      strcpy(text,tmp);
      i=i-pos_lf-1;
    }
  }
  /*実行の最後の処理*/
  print_labels(labels,c_label);
  lseek(fd,0,SEEK_SET);
  /*ここまで*/
}

int readline(int fd,Label *labels,int n_label,Instr_list *instr_l){
  char buf[1024],tmp[65536];
  char text[65536];
  int c=0,i=0,l=0;
  int pos_lf;
  /*実行に必要な変数の定義*/
  int interpret_status,ret_status=0;
 
  /*ここまで*/
  text[0]=0;
  while((c=read(fd,buf,1023))>0){
    buf[c]=0;
    strcat(text,buf);
    i+=c;
    while((pos_lf=search('\n',text,i))>=0){
      text[pos_lf]=0;
      /*各行に対する処理*/
      if((interpret_status=interpret(instr_l,text,pos_lf+1,l,labels,n_label))<0)
	ret_status=-1;
      if(interpret_status!=1) l++; 
      /*ここまで*/
      if(i>pos_lf)
	strcpy(tmp,text+pos_lf+1);
      else
	tmp[0]=0;
      strcpy(text,tmp);
      i=i-pos_lf-1;
      
    }
  }
  /*実行終了時の処理*/

  /*ここまで*/
  return ret_status;
}

int main(){
  Instr_list *instr_l=list_init();
  Label *labels;
  int fd=0;
  int n_label = count_labels(fd);
  labels=(Label*)malloc(n_label*sizeof(Label));
  search_labels(fd,labels);
  printf("return:%d\n",get_pc(labels,n_label,"return"));
  readline(fd,labels,n_label,instr_l);
  free(labels);
  list_display(instr_l);
  list_free(instr_l);
  return 0;
}
