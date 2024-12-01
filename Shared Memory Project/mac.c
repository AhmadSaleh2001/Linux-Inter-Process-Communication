#include "mac.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void handleMsg(mac_t **arr, int sz, sync_mac_t request)
{
  if (request.op == CREATE)
  {
    addNewMac(arr, sz, request.mac);
  }
  else if (request.op == UPDATE)
  {
    updateMsg(arr, sz, request.mac);
  }
  else if (request.op == DELETE)
  {
    removeMsg(arr, sz, request.mac);
  }
}

void addNewMac(mac_t **arr, int sz, mac_t newData)
{
  for (int i = 0; i < sz; i++)
  {

    if ((*arr)[i].macAddress[0] == 0)
    {
      (*arr)[i] = newData;
      break;
    }
  }
}
void updateMsg(mac_t **arr, int sz, mac_t newData)
{
  for (int i = 0; i < sz; i++)
  {
    if ((*arr)[i].macAddress[0] == 0)
      continue;
    if (strcmp((*arr)[i].macAddress, newData.macAddress) == 0)
    {
      strcpy((*arr)[i].macAddress, newData.macAddress);
    }
  }
}
void removeMsg(mac_t **arr, int sz, mac_t msgToRemove)
{
  for (int i = 0; i < sz; i++)
  {
    if ((*arr)[i].macAddress[0] == 0)
      continue;
    if (strcmp((*arr)[i].macAddress, msgToRemove.macAddress) == 0)
    {
      memset((*arr)[i].macAddress, 0, sizeof((*arr)[i].macAddress));
    }
  }
}

void printEntry(mac_t *item)
{
  printf("Mac address: %s\n", item->macAddress);
}

void printTable(mac_t **arr, int sz)
{
  for (int i = 0; i < sz; i++)
  {
    if ((*arr)[i].macAddress[0] == 0)
      continue;

    printEntry(&((*arr)[i]));
    printf("\n");
  }
  printf("\n#####################\n");
}

void enter_mac(mac_t *newToAdd)
{
  printf("Enter mac address: \n");
  scanf("%50s", newToAdd->macAddress);
  flushInputBuffer();
}

void flushInputBuffer()
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
  {
    // Consume characters in the buffer
  }
}