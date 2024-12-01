#pragma once

#define NUMBER_OF_ENTRIES 2

typedef enum
{
  CREATE = 1,
  UPDATE,
  DELETE,
  PRINT,
  PRINT_WITH_IP

} OPCODE;

typedef struct mac
{
  char macAddress[50];
} mac_t;

typedef struct sync_mac
{
  OPCODE op;
  mac_t mac;
} sync_mac_t;

void handleMsg(mac_t **arr, int sz, sync_mac_t request);
void addNewMac(mac_t **arr, int sz, mac_t newData);
void updateMsg(mac_t **arr, int sz, mac_t newData);
void removeMsg(mac_t **arr, int sz, mac_t msgToRemove);
void printTable(mac_t **arr, int sz);
void printEntry(mac_t *item);
void enter_mac(mac_t *newToAdd);
void flushInputBuffer();