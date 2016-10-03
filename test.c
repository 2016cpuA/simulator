#include <stdio.h>

#define Print1(str1) printf("%s is a lie.\n",str1)
#define Print2(s,o) printf("%s is %s.\n",s,o)
#define Print3(s,o,c) printf("%s makes %s %s.\n",s,o,c)
#define Print4(adv,person,v,o) printf("It is %s for %s to %s %s.\n",adv,person,v,o);
#define Print5(a,b,c,d,e) printf("%s %s %s %s %s.\n",a,b,c,d,e);
#define Print10(a,b,c,d,e,f,g,h,i,j) printf("%s%s%s%s%s%s%s%s%s%s\n",a,b,c,d,e,f,g,h,i,j);

/*
int main (){
  Print1("The cake");
  Print2("This","a pen");
  Print3("A drag","you","unhappy");
  Print4("tough","me","speak","English");
  Print5("The","cake","is","a","lie");
  Print10("Th","e ","ca" "ke"," i","s ","a ","li","e","."," ");
  return 0;
}
*/

int main(){
  char buf[5][100];
  scanf("%s %s %s %s %s",buf[0],buf[1],buf[2],buf[3],buf[4]);
  int i;
  for(i=0;i<5;i++){
    printf("buf[%d] = %99s\n",i,buf[i]);
  }
  return 0;
}
