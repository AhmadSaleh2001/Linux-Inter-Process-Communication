#pragma once

typedef struct hashtable
{
  int table[102];
} hashtable_t;

void initHashTable(hashtable_t *tbl);
int getMachineForKey(int k);
int getValueForKey(hashtable_t *tbl, int k);