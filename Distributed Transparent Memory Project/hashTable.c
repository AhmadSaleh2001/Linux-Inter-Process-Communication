#include "hashTable.h"

void initHashTable(hashtable_t *tbl)
{
  memset(tbl->table, -1, sizeof(tbl->table));
}
int getMachineForKey(int k)
{
  // we are assume, we have servers, their tcp ports from 2000 to 2004
  return k % 2000 + 2000;
}
int getValueForKey(hashtable_t *tbl, int k)
{
  return tbl->table[k];
}