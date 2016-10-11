#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "simulator.h"
#include "instructs.h"

extern int simulation(int program_fd,int out_instr_fd,int out_binary_fd,int execute);
extern int make_code(int out_fd,Instruct *instr,int n);

int main(int argc,char* argv[]){
  int program_fd,out_binary_fd=-1;
  int input_binary=0;
  int binary_output=1;
  int execute=1;
  int debug=0;
  if (argc<1){
    fprintf(stderr,"Error: no input file\n");
  }else{
    if((program_fd=open(argv[1],O_RDONLY))<0){
      fprintf(stderr,"Error: file '%s' not found\n",argv[1]);
    }else{
      if(input_binary){
	fprintf(stderr,"NOT IMPLEMENTED\n");
      }else{
	if(binary_output){
	  if((out_binary_fd=open("a.out",O_WRONLY | O_CREAT,00666))<0){
	    fprintf(stderr,"Error: file '%s' not found\n",argv[1]);
	  }else{
	    fprintf(stderr,"writing code into file '%s'\n","a.out");
	  }
	}
	simulation(program_fd,-1,out_binary_fd,execute);
      }
    }
    close(program_fd);
    if(out_binary_fd>0)
      close(out_binary_fd);
  }
  
  return 0;
}
