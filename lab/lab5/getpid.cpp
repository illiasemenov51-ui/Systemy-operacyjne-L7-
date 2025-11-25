#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <stdlib.h>
int main ()
{
  int j = getpid();
  printf( "to ja %d\n",j); 
  for (int k=0; k<2; k++)
  {
    int i = fork();
    j = getpid();
    printf( "to ja %d\n",j); 
  }
  exit(EXIT_SUCCESS);
}
// ile x fork?
// ile procesów i dlaczego?
// ile printf i dlaczego?