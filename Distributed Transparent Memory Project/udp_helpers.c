#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <memory.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "message.h"

void send_udp_msg(char *SERVER_IP_ADDRESS, int DEST_PORT,
                  char *LOCAL_IP_ADDRESS, int SRC_PORT, msg_t client_data)
{

  /*Step 1 : Initialization*/
  /*Socket handle*/
  int sockfd = 0,
      sent_recv_bytes = 0;

  int addr_len = 0;

  addr_len = sizeof(struct sockaddr);

  /*to store socket addesses : ip address and port*/
  struct sockaddr_in dest;

  /*Step 2: specify server information*/
  /*Ipv4 sockets, Other values are IPv6*/
  dest.sin_family = AF_INET;

  /*Client wants to send data to server process which is running on server machine, and listening on
   * port on DEST_PORT, server IP address SERVER_IP_ADDRESS.
   * Inform client about which server to send data to : All we need is port number, and server ip address. Pls note that
   * there can be many processes running on the server listening on different no of ports,
   * our client is interested in sending data to server process which is lisetning on PORT = DEST_PORT*/
  dest.sin_port = htons(DEST_PORT);
  struct hostent *host = (struct hostent *)gethostbyname(SERVER_IP_ADDRESS);
  dest.sin_addr = *((struct in_addr *)host->h_addr);

  /*Step 3 : Create a TCP socket*/
  /*Create a socket finally. socket() is a system call, which asks for three paramemeters*/
  sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  struct sockaddr_in localaddr;
  localaddr.sin_family = AF_INET;
  localaddr.sin_addr.s_addr = inet_addr(LOCAL_IP_ADDRESS);
  localaddr.sin_port = htons(SRC_PORT); // Any local port will do
  bind(sockfd, (struct sockaddr *)&localaddr, sizeof(localaddr));

  /*step 5 : send the data to server*/

  sent_recv_bytes = sendto(sockfd,
                           &client_data,
                           sizeof(msg_t),
                           0,
                           (struct sockaddr *)&dest,
                           sizeof(struct sockaddr));

  printf("No of bytes sent = %d\n", sent_recv_bytes);
}
