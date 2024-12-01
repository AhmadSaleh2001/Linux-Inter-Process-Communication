#include <stdio.h>
#include <signal.h>

int main(int argc, char **argv)
{

  if (argc < 2)
  {
    printf("please enter process number !\n");
    exit(1);
  }
  int processNumber = 0;
  for (int i = 0; argv[1][i] != '\0'; i++)
  {
    processNumber = processNumber * 10 + (argv[1][i] - '0');
  }
  printf("Process to be killed: %d\n", processNumber);
  kill(processNumber, SIGUSR1);
  return 0;
}