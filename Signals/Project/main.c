#include <stdio.h>

typedef enum
{
  CREATE = 1,
  UPDATE,
  DELETE,
  PRINT,
  SYNC

} OPCODE;

const int SIZE = sizeof(OPCODE) + 100;

int main()
{
  char BUFFER[SIZE];

  (*BUFFER) = SYNC;

  return 0;
}