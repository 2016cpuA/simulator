#include <stdio.h>

/*汎用メッセージ*/
extern inline void print_warn(const char *host,const char *argument){
  fprintf(stderr,"%s:\e[35;1mWarning\e[m: %s\n",host,argument);
}

extern inline void print_err(const char *host, const char *argument){
  fprintf(stderr,"%s:\e[31;1mError\e[m: %s\n",host,argument);
}


/*シミュレータ用*/
extern inline void print_warn_sim(const char *info,int pc,const char *argument,int no){
  fprintf(stderr,"Simulator:\e[35;1mWarning\e[m(%s; pc=%d): %s ",info,pc,argument);
  fprintf(stderr,"%d\n",no);
}
extern inline void print_err_sim(const char *info,int pc,const char *argument,int no){
  fprintf(stderr,"Simulator:\e[31;1mError\e[m(%s; pc=%d): %s ",info,pc,argument);
  fprintf(stderr,"%d\n",no);
}
extern inline void print_msg_sim_start(){
  fprintf(stderr,"Simulator:\e[36mExecution started.\e[m\n");
}
extern inline void print_msg_sim_finish(){
  fprintf(stderr,"Simulator:\e[36mExecution finished.\e[m\n");
}
extern inline void print_msg_sim_fatal(){
  fprintf(stderr,"Simulator:\e[31;1mFatal error occurred.\e[m\n");
}
extern inline void print_msg_sim_stop(){
  fprintf(stderr,"Simulator:\e[33mExecution stopped; too long operation.\e[m\n");
}
extern inline void print_msg_sim_irraddr(int pc){
  fprintf(stderr,"Simulator:\e[31;1mError\e[m(pc=%d): invalid instruction address %d.\n",pc,pc);
}
