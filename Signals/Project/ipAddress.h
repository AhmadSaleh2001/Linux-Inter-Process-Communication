#pragma once
#include "mac.h"

typedef struct ipAddress
{
  char macAddress[50];
  char ipAddress[32];
} ipAddress_t;

void saveItOnFile(ipAddress_t **ipAddresses, int sz, char *filePath);
ipAddress_t *getIpAddress(mac_t mac, int sz, char *filePath);
void addNewIpEntry(ipAddress_t **arr, int sz, ipAddress_t newData);
void enter_ipAddress(ipAddress_t *newToAdd);