#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "msg.h"

#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE NUMBER_OF_ENTRIES * sizeof(sync_msg_t)

msg_body_t *route_table_entries;

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int i;
    int ret;
    int data_socket;
    char buffer[BUFFER_SIZE];
    route_table_entries = malloc(sizeof(msg_body_t) * NUMBER_OF_ENTRIES);
    memset(route_table_entries, 0, sizeof route_table_entries);

    /* Create data socket. */

    data_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (data_socket == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /*
     * For portability clear the whole structure, since some
     * implementations have additional (nonstandard) fields in
     * the structure.
     * */

    memset(&addr, 0, sizeof(struct sockaddr_un));

    /* Connect socket to socket address */

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    ret = connect(data_socket, (const struct sockaddr *)&addr,
                  sizeof(struct sockaddr_un));

    if (ret == -1)
    {
        fprintf(stderr, "The server is down.\n");
        exit(EXIT_FAILURE);
    }

    // /* Send arguments. */
    // do
    // {
    //     printf("Enter number to send to server :\n");
    //     scanf("%d", &i);
    //     ret = write(data_socket, &i, sizeof(int));
    //     if (ret == -1)
    //     {
    //         perror("write");
    //         break;
    //     }
    //     printf("No of bytes sent = %d, data sent = %d\n", ret, i);
    // } while (i);

    /* Request result. */

    // memset(buffer, 0, BUFFER_SIZE);
    // strncpy(buffer, "RES", strlen("RES"));
    // buffer[strlen(buffer)] = '\0';
    // printf("buffer = %s\n", buffer);

    // ret = write(data_socket, buffer, strlen(buffer));
    // if (ret == -1)
    // {
    //     perror("write");
    //     exit(EXIT_FAILURE);
    // }

    /* Receive result. */
    do
    {
        memset(buffer, 0, BUFFER_SIZE);
        printf("blocking reading\n");
        ret = read(data_socket, buffer, BUFFER_SIZE);

        if (ret == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }

        char firstByte = buffer[0];
        if (firstByte == 0)
            break;

        printf("first byte: %d\n", firstByte);

        sync_msg_t *msg = (sync_msg_t *)(&buffer);
        if (msg->op == 1)
            addNewMsg(&route_table_entries, NUMBER_OF_ENTRIES, msg->msg);
        else if (msg->op == 2)
            updateMsg(&route_table_entries, NUMBER_OF_ENTRIES, msg->msg);
        else if (msg->op == 3)
            removeMsg(&route_table_entries, NUMBER_OF_ENTRIES, msg->msg);
        else if (msg->op == 4)
        {
            printTable(&route_table_entries, NUMBER_OF_ENTRIES);
        }
    } while (1);

    /* Close socket. */

    close(data_socket);

    exit(EXIT_SUCCESS);
}
