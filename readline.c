#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "simulator.h"
#include "instructs.h"
#include "list.h"


#define EMPTY_LINE 1
#define NO_ERROR 0
#define SYNTAX_ERROR -65536-1
#define UNKNOWN_SYMBOL -65536-2
#define UNKNOWN_INSTRUCT -65536-3


/*空白文字の定義*/
#define Is_Space(ch) (ch==' '||ch=='\t'||ch=='\r')

/*ラベル構造体の定義*/
typedef struct label{
  char name[100];
  int pc;
} Label;
/*ラベルの数*/
int n_label;
/*ラベルの配列*/
Label *labels;
/*読み込み中の行数*/
int d_lines;
/*命令列出力用のファイルストリーム*/
FILE *output_instr_file;

/*Utilities*/
int get_pc(Label *labels,char *name_label){
  int i;
  for(i=0;i<n_label;i++){
    if(!strcmp(labels[i].name,name_label)){
      return labels[i].pc;
    }
  }
  return -1;
}

int list_to_align(Instruct *instr_a,Instr_list *instr_l,int n){
  int i;
  Instr_list *now=instr_l;
  for(i=0;i<n;i++){
    instr_a[i]=*(now->instr);
    if(i!=n-1)
      now=now->next;
  }
  return 0;
}

void print_align(Instruct *instr_a,int n,FILE* out_file){
  int i;
  fprintf(out_file,"no\tinstr\top1\top2\top3\top4\n");
  for(i=0;i<n;i++){
    fprintf(out_file,"%d\t",i);
    print_instr(instr_a[i],out_file);
    fprintf(out_file,"\t%d\t%d\t%d\t%d\n",instr_a[i].operands[0],instr_a[i].operands[1],instr_a[i].operands[2],instr_a[i].operands[3]);
  }
}
		
void print_labels(Label *labels,int n_label,FILE* out_file){
  int i;
  fprintf(out_file,"label\tpc\n");
  for(i=0;i<n_label;i++){
    fprintf(out_file,"%s\t%d\n",labels[i].name,labels[i].pc);
  }
  fprintf(out_file,"\n");
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

int search_space(char* buf,int bufsize){
  int i;
  for(i=0;i<bufsize;i++){
    if(Is_Space(buf[i]))
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

/*Parser本体*/
/*オペランド部分のみ*/
int get_operand(char *op,int type_op,int pc,int opcode){
  int dest_pc,pos_space;
  char buf[32];
  int op_fun;
  if(isnum(op[0])){
    if (!(type_op&&IMMIDIATE))
      printf("Warning(line %d): wrong operand type\n",d_lines); 
    return strtol(op,NULL,0);
  }else if(op[0]=='%'){
    if(op[1]=='r'){
      if (!(type_op&&REG))
	printf("Warning(line %d): wrong operand type\n",d_lines);  
      return atoi(op+2);
    }else if(op[1]=='f'){
      if(!(type_op&&FREG))
        printf("Warning(line %d): wrong operand type\n",d_lines); 
      return atoi(op+2);
    }else{
      printf("Error(line %d): unkown operand '%s'\n",d_lines,op);
    }
  }else if(op[0]=='$'){
    if (!(type_op&&IMMIDIATE))
      printf("Warning(line %d): wrong operand type\n",d_lines); 
    return strtol(op+1,NULL,0);
  }else{
    if((pos_space=search_space(op,strlen(op)))>=0){
      strncpy(buf,op,pos_space);
    }else{
      strcpy(buf,op);
    }
    if((dest_pc=get_pc(labels,buf))>=0){
      op_fun=opcode&MASK_OP_FUN;
      if((op_fun==J)||(op_fun==JAL))
	return dest_pc;
      else
	return dest_pc-pc;
    }else{
      printf("Error(line %d): unkown operand '%s'\n",d_lines,buf);
    }
  }
  return SYNTAX_ERROR;
}
/*行単位のパース*/
/*ブレークポイントのフラグ*/
int is_break=0;
/*テキストセクションかデータセクションか*/
int section=1;
#define SECTION_TEXT 1
#define SECTION_DATA 0
int interpret(Instr_list *instr_l,char *buf,int bufsize,int pc){
  char *buf_cp=(char*)malloc(sizeof(char)*(bufsize+1));
  char *now=buf_cp;
  Instruct *instr_read=(Instruct*)malloc(sizeof(Instruct));
  int i,rest=bufsize-1,pos_delim,err=NO_ERROR;
  char opcode[16],operands[4][32];
  Instr_list *last=instr_l;
  strcpy(buf_cp,buf);
  for(i=0;i<4;i++)
    instr_read->operands[i]=0;
  /*'#' から先はコメント*/
  if((pos_delim=search('#',now,rest))>=0){
    now[pos_delim]=0;
    rest=pos_delim;
  }
  while((Is_Space(*now)||*now=='!')&&rest>0){
    if(*now=='!')
      is_break=1;
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
  while((Is_Space(*now)||*now=='!')&&rest>0){
    if(*now=='!')
      is_break=1;
    rest--;
    now++;
  }  
  if(rest>0){
    pos_delim=search_space(now,rest);
    if(pos_delim<0){
      strcpy(opcode,now);
      pos_delim=strlen(opcode);
    }else{
      strncpy(opcode,now,pos_delim);
      opcode[pos_delim]=0;
    }
    if((instr_read->opcode=get_instr(opcode))==-1) err=UNKNOWN_INSTRUCT;
    if(is_break){
      (instr_read->opcode)|= _BREAK ;
      is_break=0;
    }
    now+=pos_delim+1;
    rest-=pos_delim+1;
    while(Is_Space(*now)&&rest>0){
      rest--;
      now++;
    }
    /* <op> |a1,a2,a3
       <op> |a1,a2(a3) */
    i=0;
    while(i<4&&rest>0){
      pos_delim=search_delim(now,rest);
      if (pos_delim<0){
	if(rest==0) break;
	pos_delim=rest;
      }
      strncpy(operands[i],now,pos_delim);
      operands[i][pos_delim]=0;
      instr_read->operands[i]=get_operand(operands[i],7,pc,instr_read->opcode);
      if(instr_read->operands[i]==SYNTAX_ERROR){
	err=SYNTAX_ERROR;
      }
      i++;
      now+=pos_delim+1;
      rest-=pos_delim+1;
      if(now[pos_delim]=='('){
	while(Is_Space(*now)&&rest>0){
	  rest--;
	  now++;
	}
	if(rest<=0){
	  printf("Error(line %d): expected ')'",d_lines);
	  err=SYNTAX_ERROR;
	  break;
	}
	pos_delim=search(')',now,rest);
	strncpy(operands[i],now,pos_delim);
	instr_read->operands[i]=get_operand(operands[i],7,pc,instr_read->opcode);
	if(instr_read->operands[i]==SYNTAX_ERROR){
	  err=SYNTAX_ERROR;
	}
	i++;
	now+=pos_delim+1;
	rest-=pos_delim+1;
      }
      while(Is_Space(*now)&&rest>0){
	rest--;
	now++;
      }
    }
    while(!list_isempty(last))
      last=last->next;
    last->no = pc;
    list_push(last,instr_read);
    free(buf_cp);
  }else{
    free(buf_cp);
    free(instr_read);
    err=EMPTY_LINE;
  }
  return err;
}

int readline(int fd,Instr_list *instr_l){
  char buf[1024],tmp[65536];
  char text[65536];
  int c=0,i=0,l;
  int pos_lf;
  int step=0;
  /*実行に必要な変数の定義*/
  /*step 0*/
  int colons=0;
  /*step 1*/
  char *now;
  int pos_colon;
  /*step 2*/
  int interpret_status,ret_status=0;
  /*ここまで*/
  for(step=0;step<3;step++){
    /*初期化*/
    text[0]=0;c=0;i=0;l=0;
    /*ここまで*/
    while((c=read(fd,buf,1023))>0){
      buf[c]=0;
      strcat(text,buf);
      i+=c;
      while((pos_lf=search('\n',text,i))>=0){
	text[pos_lf]=0;
	/*各行に対する処理*/
	if(step==0){
	  /*step 0: ラベルの数のカウント*/
	  if(search(':',text,pos_lf)>=0)
	    colons++;
	}else if(step==1){
	  /*step 1: ラベルの配列の作成*/
	  strcpy(tmp,text);
	  if((pos_colon=search(':',tmp,pos_lf))>0){
	    tmp[pos_colon]=0;
	    strcpy(labels[colons].name,tmp);
	    if(!strcmp(labels[colons].name,"SYS_EXIT")){
	      fprintf(stderr,"Error: Name 'SYS_EXIT' could not be used as label name\n");
	      ret_status=-1;
	    }
	    labels[colons].pc=l;
	    colons++;
	  }
	  if(pos_colon>=0){
	    now=tmp+pos_colon+1;
	  }else{
	    now=tmp;
	  }
	  /*命令の無い行か?*/
	  while(Is_Space(*now)||*now=='!'){
	    now++;
	  }
	  /*行の最後まで空白or疑似命令('.'で始まる命令)があるor単行コメントがあったら行数をカウントしない*/
	  if (*now!=0&&*now!='#'&&*now!='.'){
	    l++;
	  }
	}else{
	  /*step 2: 各行の解釈*/
	  if((interpret_status=interpret(instr_l,text,pos_lf+1,l))<0)
	    ret_status=-1;
	  if(interpret_status!=1) l++; 
	  d_lines++;
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
    /*実行終了時の処理*/
    if(step==0){
      /*step 0*/
      lseek(fd,0,SEEK_SET);
      labels=(Label*)malloc((colons+1)*sizeof(Label));
      n_label=colons+1;
      colons=0;
    }else if(step==1){
      /*step 1*/
      lseek(fd,0,SEEK_SET);
      strcpy(labels[colons].name,"SYS_EXIT");
      labels[colons].pc=l;
      if(output_instr_file!=NULL){
	print_labels(labels,n_label,output_instr_file);
      }
      d_lines=1;
    }else{
      /*step 2*/
      free(labels);
      if(ret_status==0){
	ret_status=l;
      }
    }
    /*ここまで*/
  }
  return ret_status;
}

Instruct *load_instruct(int fd,char* output_instr_file_name,int *size){
  Instr_list *instr_l=list_init();
  Instruct *instr_a;
  int l;
  /*命令列出力用のファイルのオープン*/
  output_instr_file=NULL;
  if(output_instr_file_name!=NULL){
    output_instr_file=fopen(output_instr_file_name,"w");
    if(output_instr_file_name==NULL){
      fprintf(stderr,"Warning: Could not open file '%s'\n",output_instr_file_name);
    }
  }
  l=readline(fd,instr_l);
  if(l>0){
    instr_a=(Instruct*)malloc(l*sizeof(Instruct));
    list_to_align(instr_a,instr_l,l);
    list_free(instr_l);
    if(output_instr_file!=NULL)
      print_align(instr_a,l,output_instr_file);
  }else{
    printf("Failed to load.\n");
    list_free(instr_l);
  }
  *size=l;
  if(output_instr_file!=NULL){
    fclose(output_instr_file);
  }
  return instr_a;
}
