#include "msg.h"
#include <string.h>

void handleMsg(msg_body_t **arr, int sz, sync_msg_t request)
{
  if (request.op == CREATE)
  {
    addNewMsg(arr, sz, request.msg);
  }
  else if (request.op == UPDATE)
  {
    updateMsg(arr, sz, request.msg);
  }
  else if (request.op == DELETE)
  {
    removeMsg(arr, sz, request.msg);
  }
}

void addNewMsg(msg_body_t **arr, int sz, msg_body_t newData)
{
  for (int i = 0; i < sz; i++)
  {

    if ((*arr)[i].mask == 0)
    {
      (*arr)[i] = newData;
      break;
    }
  }
}
void updateMsg(msg_body_t **arr, int sz, msg_body_t newData)
{
  for (int i = 0; i < sz; i++)
  {
    if ((*arr)[i].mask == 0)
      continue;
    if (strcmp((*arr)[i].destination, newData.destination) == 0)
    {
      strcpy((*arr)[i].destination, newData.destination);
      (*arr)[i].mask = newData.mask;
      strcpy((*arr)[i].gateway, newData.gateway);
      (*arr)[i].output_interface = newData.output_interface;
    }
  }
}
void removeMsg(msg_body_t **arr, int sz, msg_body_t msgToRemove)
{
  for (int i = 0; i < sz; i++)
  {
    if ((*arr)[i].mask == 0)
      continue;
    if (strcmp((*arr)[i].destination, msgToRemove.destination) == 0)
    {
      (*arr)[i].mask = 0;
    }
  }
}

void printEntry(msg_body_t *item)
{
  printf("destination: %s\n", item->destination);
  printf("mask: %d\n", item->mask);
  printf("gateway: %s\n", item->gateway);
  printf("output interface: %c\n", item->output_interface);
}

void printTable(msg_body_t **arr, int sz)
{
  for (int i = 0; i < sz; i++)
  {
    if ((*arr)[i].mask == 0)
      continue;

    printEntry(&((*arr)[i]));
    printf("\n");
  }
  printf("\n#####################\n");
}