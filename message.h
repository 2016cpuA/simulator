/*メッセージ出力*/

/*汎用メッセージ*/
void print_warn(const char *argument);
void print_err(const char *argument);
/*シミュレータ用メッセージ*/
void print_warn_sim(const char *info,int pc,const char *argument,int no);
void print_err_sim(const char *info,int pc,const char *argument,int no);
void print_msg_sim_start();
void print_msg_sim_finish();
void print_msg_sim_fatal();
void print_msg_sim_stop();
void print_msg_sim_irraddr(int pc);
