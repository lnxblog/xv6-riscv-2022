#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char **argv)
{
  if(argc != 2)
  {
    printf("Error: Requires one argument\n");
    exit(1);
  }
  char *ticks_str = argv[1];
  unsigned int ticks = atoi(ticks_str);
  if(sleep(ticks)!=0)
  return 1;



  return 0;


}