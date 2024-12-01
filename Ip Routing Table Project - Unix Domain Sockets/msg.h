#pragma once

#define NUMBER_OF_ENTRIES 255

typedef enum
{
  CREATE = 1,
  UPDATE,
  DELETE,
  PRINT,
  SYNC

} OPCODE;

typedef struct msg_body
{
  char destination[32];
  char mask;
  char gateway[32];
  char output_interface;
} msg_body_t;

typedef struct sync_msg
{
  OPCODE op;
  msg_body_t msg;

} sync_msg_t;

void handleMsg(msg_body_t **arr, int sz, sync_msg_t request);
void addNewMsg(msg_body_t **arr, int sz, msg_body_t newData);
void updateMsg(msg_body_t **arr, int sz, msg_body_t newData);
void removeMsg(msg_body_t **arr, int sz, msg_body_t msgToRemove);
void printTable(msg_body_t **arr, int sz);
void printEntry(msg_body_t *item);