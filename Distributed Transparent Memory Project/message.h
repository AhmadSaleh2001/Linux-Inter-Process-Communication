#pragma once

typedef enum
{
  WHAT_X = 1,
  PUT_X,
  GET_FORWARD,
  PUT_FORWARD,
  PUT_REPLY_X,
  GET_REPLY_X,

} MSG;

typedef struct msg
{
  MSG msg_type;
  unsigned int k, v;
  unsigned int originatorIpAddress;
  unsigned int portNumber;
} msg_t;

typedef struct result_struct_
{

  unsigned int c;

} result_struct_t;

void readKey(msg_t *msg);
void readValue(msg_t *msg);
void printMessage(msg_t *msg);
void flushInputBuffer();