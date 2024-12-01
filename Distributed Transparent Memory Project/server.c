#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <memory.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "message.h"
#include "hashTable.h"
#include "udp_helpers.h"

#define TCP_SERVER_MAX_CLIENT_SUPPORTED 32
char data_buffer[1024];

int monitored_fd_set[32];

static void
intitiaze_monitor_fd_set()
{

    int i = 0;
    for (; i < TCP_SERVER_MAX_CLIENT_SUPPORTED; i++)
        monitored_fd_set[i] = -1;
}

static void
add_to_monitored_fd_set(int skt_fd)
{

    int i = 0;
    for (; i < TCP_SERVER_MAX_CLIENT_SUPPORTED; i++)
    {

        if (monitored_fd_set[i] != -1)
            continue;
        monitored_fd_set[i] = skt_fd;
        break;
    }
}

static void
remove_from_monitored_fd_set(int skt_fd)
{

    int i = 0;
    for (; i < TCP_SERVER_MAX_CLIENT_SUPPORTED; i++)
    {

        if (monitored_fd_set[i] != skt_fd)
            continue;

        monitored_fd_set[i] = -1;
        break;
    }
}

static void
re_init_readfds(fd_set *fd_set_ptr)
{

    FD_ZERO(fd_set_ptr);
    int i = 0;
    for (; i < TCP_SERVER_MAX_CLIENT_SUPPORTED; i++)
    {
        if (monitored_fd_set[i] != -1)
        {
            FD_SET(monitored_fd_set[i], fd_set_ptr);
        }
    }
}

static int
get_max_fd()
{

    int i = 0;
    int max = -1;

    for (; i < TCP_SERVER_MAX_CLIENT_SUPPORTED; i++)
    {
        if (monitored_fd_set[i] > max)
            max = monitored_fd_set[i];
    }

    return max;
}

static uint32_t
tcp_ip_covert_ip_p_to_n(char *ip_addr)
{

    uint32_t binary_prefix = 0;
    inet_pton(AF_INET, ip_addr, &binary_prefix);
    binary_prefix = htonl(binary_prefix);
    return binary_prefix;
}

int getInteger(char *arr)
{
    int ans = 0;
    for (int i = 0; arr[i] != '\0'; i++)
    {
        ans = ans * 10 + (arr[i] - '0');
    }

    return ans;
}

void setup_tcp_server_communication(char *SERVER_IP, int SERVER_PORT, int *master_sock_tcp_fd, struct sockaddr_in *server_addr, int *addr_len, int *opt)
{

    /*step 2: tcp master socket creation*/
    if (((*master_sock_tcp_fd) = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        printf("socket creation failed\n");
        exit(1);
    }

    /*Step 3: specify server Information*/
    (*server_addr).sin_family = AF_INET;          /*This socket will process only ipv4 network packets*/
    (*server_addr).sin_port = htons(SERVER_PORT); /*Server will process any data arriving on port no 2000*/
    (*server_addr).sin_addr.s_addr = htonl(tcp_ip_covert_ip_p_to_n(SERVER_IP));

    (*addr_len) = sizeof(struct sockaddr);

    if (setsockopt((*master_sock_tcp_fd), SOL_SOCKET,
                   SO_REUSEADDR, (char *)opt, sizeof((*opt))) < 0)
    {
        printf("setsockopt Failed\n");
        exit(0);
    }

    if (setsockopt((*master_sock_tcp_fd), SOL_SOCKET,
                   SO_REUSEPORT, (char *)opt, sizeof((*opt))) < 0)
    {
        printf("setsockopt Failed\n");
        exit(0);
    }

    /* Bind the server. Binding means, we are telling kernel(OS) that any data
     * you recieve with dest ip address = 192.168.56.101, and tcp port no = 2000, pls send that data to this process
     * bind() is a mechnism to tell OS what kind of data server process is interested in to recieve. Remember, server machine
     * can run multiple server processes to process different data and service different clients. Note that, bind() is
     * used on server side, not on client side*/

    if (bind((*master_sock_tcp_fd), server_addr, sizeof(struct sockaddr)) == -1)
    {
        printf("socket bind failed\n");
        return;
    }

    /*Step 4 : Tell the Linux OS to maintain the queue of max length to Queue incoming
     * client connections.*/
    if (listen((*master_sock_tcp_fd), 5) < 0)
    {
        printf("listen failed\n");
        return;
    }

    /*Add master socket to Monitored set of FDs*/
    add_to_monitored_fd_set((*master_sock_tcp_fd));
}

void setup_udp_server_communication(char *SERVER_IP, int SERVER_PORT, int *master_sock_udp_fd, struct sockaddr_in *server_addr, int *addr_len, int *opt)
{

    printf("UDP Server instantiated %s:%d\n", SERVER_IP, SERVER_PORT);

    /*step 2: tcp master socket creation*/
    if (((*master_sock_udp_fd) = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        printf("socket creation failed\n");
        exit(1);
    }

    /*Step 3: specify server Information*/
    (*server_addr).sin_family = AF_INET;          /*This socket will process only ipv4 network packets*/
    (*server_addr).sin_port = htons(SERVER_PORT); /*Server will process any data arriving on port no 2000*/
    (*server_addr).sin_addr.s_addr = htonl(tcp_ip_covert_ip_p_to_n(SERVER_IP));

    addr_len = sizeof(struct sockaddr);

    if (setsockopt((*master_sock_udp_fd), SOL_SOCKET,
                   SO_REUSEADDR, (char *)opt, sizeof((*opt))) < 0)
    {
        printf("setsockopt Failed\n");
        exit(0);
    }

    if (setsockopt((*master_sock_udp_fd), SOL_SOCKET,
                   SO_REUSEPORT, (char *)opt, sizeof((*opt))) < 0)
    {
        printf("setsockopt Failed\n");
        exit(0);
    }

    /* Bind the server. Binding means, we are telling kernel(OS) that any data
     * you recieve with dest ip address = 192.168.56.101, and tcp port no = 2000, pls send that data to this process
     * bind() is a mechnism to tell OS what kind of data server process is interested in to recieve. Remember, server machine
     * can run multiple server processes to process different data and service different clients. Note that, bind() is
     * used on server side, not on client side*/

    if (bind((*master_sock_udp_fd), server_addr, sizeof(struct sockaddr)) == -1)
    {
        printf("socket bind failed\n");
        return;
    }

    /*Add master socket to Monitored set of FDs*/
    add_to_monitored_fd_set((*master_sock_udp_fd));
}

void send_tcp_message(char *SERVER_IP_ADDRESS, int DEST_PORT,
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
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in localaddr;
    localaddr.sin_family = AF_INET;
    localaddr.sin_addr.s_addr = inet_addr(LOCAL_IP_ADDRESS);
    localaddr.sin_port = htons(SRC_PORT); // Any local port will do
    bind(sockfd, (struct sockaddr *)&localaddr, sizeof(localaddr));

    int rc = connect(sockfd, (struct sockaddr *)&dest, sizeof(struct sockaddr));

    if (rc == 0)
    {
        printf("connected\n");
    }
    else
    {
        printf("connection failed, error no %d\n", errno);
        exit(0);
    }

    /*Step 4 : get the data to be sent to server*/
    /*Our client is now ready to send data to server. sendto() sends data to Server*/
    msg_t result;
    /*step 5 : send the data to server*/
    sent_recv_bytes = sendto(sockfd,
                             &client_data,
                             sizeof(msg_t),
                             0,
                             (struct sockaddr *)&dest,
                             sizeof(struct sockaddr));

    printf("No of bytes sent = %d\n", sent_recv_bytes);
}

int main(int argc, char **argv)
{

    if (argc != 5)
    {
        printf("usage : ./<binary_name> <ip-address> <tcp-port-no> <udp-port-no> <next-port-no>\n");
        exit(0);
    }

    /*Step 1 : Initialization*/
    /*Socket handle and other variables*/
    int master_sock_tcp_fd = 2, /*Master socket file descriptor, used to accept new client connection only, no data exchange*/
        sent_recv_bytes = 0,
        addr_len = 0,
        opt_tcp = 1;

    int comm_socket_fd = 0; /*client specific communication socket file descriptor, used for only data exchange/communication between client and server*/
    fd_set readfds;         /*Set of file descriptor on which select() polls. Select() unblocks whever data arrives on any fd present in this set*/
    /*variables to hold server information*/
    struct sockaddr_in server_addr, /*structure to store the server and client info*/
        client_addr;

    /* Just drain the array of monitored file descriptors (sockets)*/
    intitiaze_monitor_fd_set();

    char *SERVER_IP = argv[1];
    int TCP_SERVER_PORT = getInteger(argv[2]);
    int UDP_SERVER_PORT = getInteger(argv[3]);
    int NEXT_SERVER_PORT = getInteger(argv[4]);

    setup_tcp_server_communication(SERVER_IP, TCP_SERVER_PORT, &master_sock_tcp_fd, &server_addr, &addr_len, &opt_tcp);

    printf("TCP Server instantiated %s: tcp: %d, udp: %d\n", SERVER_IP, TCP_SERVER_PORT, UDP_SERVER_PORT);

    /* Server infinite loop for servicing the client*/

    /*Step 1 : Initialization*/
    /*Socket handle and other variables*/
    int master_sock_udp_fd = 3, /*Master socket file descriptor, used to accept new client connection only, no data exchange*/
        opt_udp = 2;
    setup_udp_server_communication(SERVER_IP, UDP_SERVER_PORT, &master_sock_udp_fd, &server_addr, &addr_len, &opt_udp);

    add_to_monitored_fd_set(0);

    hashtable_t h;
    initHashTable(&h);
    while (1)
    {
        /*Step 5 : initialze and dill readfds*/
        re_init_readfds(&readfds); /*Copy the entire monitored FDs to readfds*/

        printf("blocked on select System call...\n");

        /*Step 6 : Wait for client connection*/
        /*state Machine state 1 */
        select(get_max_fd() + 1, &readfds, NULL, NULL, NULL); /*Call the select system cal, server process blocks here. Linux OS keeps this process blocked untill the data arrives on any of the file Drscriptors in the 'readfds' set*/

        /*Some data on some fd present in monitored fd set has arrived, Now check on which File descriptor the data arrives, and process accordingly*/

        /*If Data arrives on master socket tcp FD*/
        if (FD_ISSET(0, &readfds)) /*If Data arrives on console socket*/
        {
            char buffer[10];
            const int BUFFER_SIZE = 10;
            memset(buffer, 0, BUFFER_SIZE);
            int ret = read(0, buffer, BUFFER_SIZE);
            int choice = buffer[0] - '0';

            msg_t msg;
            if (choice == WHAT_X)
            {
                readKey(&msg);
                if (getMachineForKey(msg.k) == TCP_SERVER_PORT)
                {
                    printf("value for %d is: %d\n", msg.k, h.table[msg.k]);
                }
                else
                {
                    msg.msg_type = GET_FORWARD;
                    msg.portNumber = TCP_SERVER_PORT;
                    send_udp_msg(SERVER_IP, NEXT_SERVER_PORT + 2000, SERVER_IP, UDP_SERVER_PORT, msg);
                }
            }
            else if (choice == PUT_X)
            {
                readKey(&msg);
                readValue(&msg);
                if (getMachineForKey(msg.k) == TCP_SERVER_PORT)
                {
                    h.table[msg.k] = msg.v;
                    printf("Value for k: %d, was updated with: %d\n", msg.k, msg.v);
                }
                else
                {
                    msg.msg_type = PUT_FORWARD;
                    msg.portNumber = TCP_SERVER_PORT;
                    send_udp_msg(SERVER_IP, NEXT_SERVER_PORT + 2000, SERVER_IP, UDP_SERVER_PORT, msg);
                }
            }
        }
        else if (FD_ISSET(master_sock_tcp_fd, &readfds))
        {
            /*Data arrives on Master socket only when new client connects with the server (that is, 'connect' call is invoked on client side)*/
            printf("TCP: New connection recieved recvd, accept the connection. Client and Server completes TCP-3 way handshake at this point\n");

            /* step 7 : accept() returns a new temporary file desriptor(fd). Server uses this 'comm_socket_fd' fd for the rest of the
             * life of connection with this client to send and recieve msg. Master socket is used only for accepting
             * new client's connection and not for data exchange with the client*/
            /* state Machine state 2*/
            comm_socket_fd = accept(master_sock_tcp_fd, (struct sockaddr *)&client_addr, &addr_len);
            if (comm_socket_fd < 0)
            {

                /* if accept failed to return a socket descriptor, display error and exit */
                printf("accept error : errno = %d\n", errno);
                exit(0);
            }

            add_to_monitored_fd_set(comm_socket_fd);
            printf("Connection accepted from client : %s:%u\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        }
        else if (FD_ISSET(master_sock_udp_fd, &readfds)) /*If Data arrives on master socket udp FD*/
        {
            memset(data_buffer, 0, sizeof(data_buffer));
            sent_recv_bytes = recvfrom(master_sock_udp_fd, (char *)data_buffer, sizeof(data_buffer), 0, (struct sockaddr *)&client_addr, &addr_len);

            printf("UDP: Server recvd %d bytes from client %s:%u\n", sent_recv_bytes,
                   inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            msg_t *client_data = (msg_t *)data_buffer;
            int desiredMachinePort = getMachineForKey(client_data->k);
            // Expeting either PUT_FORWARD OR GET_FORWARD

            if (desiredMachinePort == TCP_SERVER_PORT)
            {
                if (client_data->msg_type == GET_FORWARD)
                {
                    client_data->msg_type = GET_REPLY_X;
                    client_data->v = h.table[client_data->k];
                }
                else if (client_data->msg_type == PUT_FORWARD)
                {
                    client_data->msg_type = PUT_REPLY_X;
                    h.table[client_data->k] = client_data->v;
                }
                send_tcp_message(SERVER_IP, client_data->portNumber, SERVER_IP, TCP_SERVER_PORT, *client_data);
            }
            else if (client_data->msg_type == PUT_FORWARD || client_data->msg_type == GET_FORWARD)
            {
                send_udp_msg(SERVER_IP, NEXT_SERVER_PORT + 2000, SERVER_IP, UDP_SERVER_PORT, *client_data);
            }

        } /*step 10 : wait for new client request again*/
        else
        {

            int i = 0, comm_socket_fd = -1;
            for (; i < TCP_SERVER_MAX_CLIENT_SUPPORTED; i++)
            {

                if (FD_ISSET(monitored_fd_set[i], &readfds))
                { /*Find the clinet FD on which Data has arrived*/

                    comm_socket_fd = monitored_fd_set[i];

                    memset(data_buffer, 0, sizeof(data_buffer));
                    sent_recv_bytes = recvfrom(comm_socket_fd, (char *)data_buffer, sizeof(data_buffer), 0, (struct sockaddr *)&client_addr, &addr_len);

                    printf("Server recvd %d bytes from client %s:%u\n", sent_recv_bytes,
                           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                    if (sent_recv_bytes == 0)
                    {
                        /*If server recvs empty msg from client, server may close the connection and wait
                         * for fresh new connection from client - same or different*/
                        close(comm_socket_fd);
                        remove_from_monitored_fd_set(comm_socket_fd);
                        break; /*goto step 5*/
                    }

                    msg_t *client_data = (msg_t *)data_buffer;
                    msg_t result;

                    printMessage(client_data);

                    if (client_data->msg_type == GET_REPLY_X)
                    {
                        result.k = 200;
                        result.v = 200;

                        /* Server replying back to client now*/
                        sent_recv_bytes = sendto(comm_socket_fd, (char *)&result, sizeof(result_struct_t), 0,
                                                 (struct sockaddr *)&client_addr, sizeof(struct sockaddr));

                        printf("Server sent %d bytes in reply to client\n", sent_recv_bytes);
                    }
                    else if (client_data->msg_type == PUT_REPLY_X)
                    {
                        result.k = 200;
                        result.v = 200;
                        sent_recv_bytes = sendto(comm_socket_fd, (char *)&result, sizeof(result_struct_t), 0,
                                                 (struct sockaddr *)&client_addr, sizeof(struct sockaddr));

                        printf("Server sent %d bytes in reply to client\n", sent_recv_bytes);

                        printf("value for %d was setted to be %d\n", client_data->k, client_data->v);
                    }

                    close(comm_socket_fd);
                    remove_from_monitored_fd_set(comm_socket_fd);
                    printf("Server closes connection with client : %s:%u\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                    /*Goto state machine State 1*/
                    break; /*Get out of inner while loop, server is done with this client, time to check for new connection request by executing selct()*/
                }
            }
        }
    }
    return 0;
}
