#include <stdio.h>
#include <signal.h>

static void handleSigUsr1(int signal)
{
  printf("gotcha !, signal: %d\n", signal);
  exit(0);
}

int main()
{

  signal(SIGUSR1, handleSigUsr1);
  scanf("\n");
  return 0;
}