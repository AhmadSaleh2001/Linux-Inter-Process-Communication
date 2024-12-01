#include "message.h"
#include <stdio.h>

void flushInputBuffer()
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
  {
    // Consume characters in the buffer
  }
}

void readKey(msg_t *msg)
{
  printf("enter key: ");
  scanf("%d", &msg->k);

  flushInputBuffer();
}

void readValue(msg_t *msg)
{
  printf("enter value: ");
  scanf("%d", &msg->v);
  flushInputBuffer();
}

void printMessage(msg_t *msg)
{
  printf("\n----------\n");
  printf("type: %d\n", msg->msg_type);
  printf("k: %d\n", msg->k);
  printf("v: %d\n", msg->v);
  printf("port: %d\n", msg->portNumber);
  printf("\n----------\n");
}