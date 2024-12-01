#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "mac.h"
#include "ipAddress.h"

#define SOCKET_NAME "/tmp/DemoSocket2"
#define BUFFER_SIZE NUMBER_OF_ENTRIES * sizeof(mac_t)

mac_t *mac_table_entries;

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int i;
    int ret;
    int data_socket;
    char buffer[BUFFER_SIZE];
    mac_table_entries = malloc(sizeof(mac_t) * NUMBER_OF_ENTRIES);
    memset(mac_table_entries, 0, sizeof mac_table_entries);

    /* Create data socket. */

    data_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    printf("data socket: %d\n", data_socket);

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
        int choice;
        printf("do you want to get IPAddress? (Y/N)\n");
        scanf("%d", &choice);
        if (choice == 1)
        {
            mac_t macToSearch;
            enter_mac(&macToSearch);
            ipAddress_t *ipAddress = getIpAddress(macToSearch, NUMBER_OF_ENTRIES, "test.txt");
            if (ipAddress == NULL)
            {
                printf("ip address not found\n");
            }
            else
            {
                printf("mac: %s\n", ipAddress->macAddress);
                printf("ip address: %s\n", ipAddress->ipAddress);
            }
            continue;
        }

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

        sync_mac_t *msg = (sync_mac_t *)(&buffer);
        if (msg->op == 1)
            addNewMac(&mac_table_entries, NUMBER_OF_ENTRIES, msg->mac);
        else if (msg->op == 2)
            updateMsg(&mac_table_entries, NUMBER_OF_ENTRIES, msg->mac);
        else if (msg->op == 3)
            removeMsg(&mac_table_entries, NUMBER_OF_ENTRIES, msg->mac);
        else if (msg->op == 4)
        {
            printTable(&mac_table_entries, NUMBER_OF_ENTRIES);
        }
        flushInputBuffer();
    } while (1);

    /* Close socket. */

    close(data_socket);

    exit(EXIT_SUCCESS);
}
