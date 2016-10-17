#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "simulator.h"
#include "instructs.h"

extern int _sim(int program_fd,char* output_instr_file_name,int out_binary_fd,int execute,int iter_max);
extern int make_code(int out_fd,Instruct *instr,int n);

int main(int argc,char* argv[]){
  int program_fd,out_binary_fd=-1;
  int input_binary=0;
  int binary_output=0;
  int execute=1;
  int debug=0;
  int iter_max=0x7fffffff;
  int ch;
  char _ch;
  extern char *optarg;
  extern int optind,opterr;
  
  char *binary_file_name,*output_instr_file_name=NULL;
  int name_len;

  while((ch=getopt(argc,argv,"bdnl:a:I:"))!=-1){
    switch(ch){
    case 'b': /* read binary */
      input_binary=1;
      break;
    case 'd': /* debug mode */
      debug=1;
      fprintf(stderr,"Warning: Unknown option 'd'\n");
      break;
    case 'n': /* no simulation */
      execute=0;
      break;
    case 'l': /* output instructs */
      name_len=strlen(optarg);
      output_instr_file_name=(char*)malloc((name_len+1)*sizeof(char));
      strcpy(output_instr_file_name,optarg);
      break;
    case 'a': /* assembler */
      binary_output=1;
      name_len=strlen(optarg);
      binary_file_name=(char*)malloc((name_len+1)*sizeof(char));
      strcpy(binary_file_name,optarg);
      break;
    case 'I':
      iter_max=atoi(optarg);
      break;
    default: /* unknown option */
      _ch=(char)ch;
      fprintf(stderr,"Warning: Unknown option '%s'\n",&_ch);
    }
  }
  if (optind>=argc){
    fprintf(stderr,"Error: no input file\n");
  }else{
    if((program_fd=open(argv[optind],O_RDONLY))<0){
      fprintf(stderr,"Error: file '%s' not found\n",argv[optind]);
    }else{
      if(input_binary){
	fprintf(stderr,"Error: Read-Binary mode NOT IMPLEMENTED\n");
      }else{
	if(binary_output){
	  if((out_binary_fd=open(binary_file_name,O_WRONLY | O_CREAT,00666))<0){
	    fprintf(stderr,"Error: file '%s' not found\n",argv[1]);
	  }else{
	    fprintf(stderr,"writing code into file '%s'\n","a.out");
	  }
	  free(binary_file_name);
	}
	_sim(program_fd,output_instr_file_name,out_binary_fd,execute,iter_max);
      }
    }
    close(program_fd);
    if(out_binary_fd>0)
      close(out_binary_fd);
  }
  if(output_instr_file_name!=NULL){
    free(output_instr_file_name);
  }
  return 0;
}
