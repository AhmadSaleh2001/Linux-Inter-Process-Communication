#pragma once
#include "message.h"

void send_udp_msg(char *SERVER_IP_ADDRESS, int DEST_PORT,
                  char *LOCAL_IP_ADDRESS, int SRC_PORT, msg_t client_data);