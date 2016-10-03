#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "simulator.h"
#include "instructs.h"
#include "list.h"

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

void print_linenum(char *text,int l){
  printf("%d:\t%s\n",l,text);
}

int get_operand(char *op,int type_op){
  if(isnum(ch[0])){
    if (!(type_op&&IMMIDIATE))
      printf("Warning: wrong operand type\n"); 
    return atoi(op);
  }else if(ch[0]=='%'){
    if(ch[1]=='r'){
      if (!(type_op&&REG))
	printf("Warning: wrong operand type\n"); 
      return atoi(op+2);
    }else if(ch[1]=='f'){
      if(!(type_op&&FREG))
	printf("Warning: wrong operand type\n");
      return atoi(op+2);
    }else{
      printf("Error: unkown operand '%s'\n",op);
    }
  }else{
    printf("Error: unkown operand '%s'\n",op);
  }
  return ERROR;
}

void interpret(Instr_list *instr_l,char *buf,int bufsize){
  int args = count(' ',buf,bufsize);
  Instruct *instr_read=(Instruct*)malloc(sizeof(Instruct));
  int pos_delim=search(' ',buf,bufsize),i;
  char *buf_cp=(char*)malloc(sizeof(char)*(bufsize+1));
  char *now=buf_cp;
  strcpy(buf_cp,buf);
  buf_cp[pos_delim]=0;
  instr_read->opcode=get_instr(now);
  now+=pos_delim+1;
  bufsize-=pos_delim+1;
  for(i=0;i<4;i++){
    if(i==args){
      pos_delim=search('\n',now,bufsize);
      now[pos_delim]=0;
      instr_read->operands[i]=get_operand(now,0);
    }else if(i<args){
      pos_delim=search(',',now,bufsize);
      now[pos_delim]=0;
      instr_read->operands[i]=get_operand(now,0);
      now+=pos_delim+1;
      bufsize-=pos_delim+1;
    }else{
      instr_read->operands[i]=-1;
    }
  }
  Instr_list *last=instr_l;
  while(!list_isempty(instr_l->next))
    last=instr_l->next;
  list_push(last,instr_read);
  free(buf_cp);

}

int readline(int fd){
  char buf[1024],tmp[65536];
  char text[65536];
  int c=0,i=0,l=1;
  int pos_lf;
  Instr_list *instr_l;
  list_init(instr_l);
  text[0]=0;
  while((c=read(fd,buf,1023))>0){
    buf[c]=0;
    strcat(text,buf);
    i+=c;
    while((pos_lf=search('\n',text,i))>=0){
      text[pos_lf]=0;
      interpret(instr_l,text,pos_lf+1);
      l++;
      if(i>pos_lf)
	strcpy(tmp,text+pos_lf+1);
      else
	tmp[0]=0;
      strcpy(text,tmp);
      i=i-pos_lf-1;
    }
  }
  printf("%s",text);
  return 0;
}

int main(){
  readline(0);
  return 0;
}
