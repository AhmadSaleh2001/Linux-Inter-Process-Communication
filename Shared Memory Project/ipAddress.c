#include "ipAddress.h"
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <memory.h>

void saveItOnFile(ipAddress_t **ipAddresses, int sz, char *filePath)
{
  int fd = open(filePath, O_RDWR);

  if (fd < 0)
  {
    printf("\n\"%s \" could not open\n",
           filePath);
    exit(1);
  }

  /* Extract the size of the file */
  struct stat statbuf;
  int err = fstat(fd, &statbuf);
  if (err < 0)
  {
    printf("\n\"%s \" could not open\n",
           filePath);
    exit(2);
  }

  char *ptr = mmap(NULL,                   /*Let Kernel decide the starting address of mapping in Virtual Memory */
                   statbuf.st_size,        /* Memory size to Map */
                   PROT_READ | PROT_WRITE, /* Map the memory in Read-Write mode, meaning the Virtual Memory can be read
                                                and written by the process. Note that, this permission must be compatible
                                                with permissions with which the file is open using open () */
                   MAP_SHARED,             /* To see the changes in the file, should be always MAP_SHARED.   */
                   fd,                     /* FD represents the identifier of the external data source, in this case it is a file */
                   0);                     /* Offset into the text file from which the mapping has to begin, pass zero to map from the
                                                   beginning of the file */

  if (ptr == MAP_FAILED)
  {
    printf("Mapping Failed, errorno = %d\n", errno);
    return 1;
  }

  close(fd); /* We will not going to read/write into file using file-handling routines, hence close the fd. mmap can still
                        use this file even if its fd is closed */

  memcpy(ptr, *ipAddresses, sz * sizeof(ipAddress_t));

  msync(ptr, sz * sizeof(ipAddress_t), MS_SYNC);

  err = munmap(ptr, statbuf.st_size); /* Destroy the mapping once done */

  if (err != 0)
  {
    printf("UnMapping Failed\n");
  }
}
ipAddress_t *getIpAddress(mac_t mac, int sz, char *filePath)
{
  /* OPen the file in Read Only Mode */
  int fd = open(filePath, O_RDONLY /*| O_WRONLY | O_RDWR */);

  if (fd < 0)
  {
    printf("\n\"%s \" could not open\n",
           filePath);
    exit(1);
  }

  /* Extract the size of the file */
  struct stat statbuf;
  int err = fstat(fd, &statbuf);
  if (err < 0)
  {
    printf("\n\"%s \" could not open\n",
           filePath);
    exit(2);
  }

  char *ptr = mmap(NULL,            /*Let Kernel decide the starting address of mapping in Virtual Memory */
                   statbuf.st_size, /* Memory size to Map */
                   PROT_READ,       /* Map the memory in Read only mode, meaning the Virtual Memory can be read
                                                      and cannot be modified by the process. Note that, this permission must be compatible
                                                      with permissions with which the file is open using open () */
                   MAP_PRIVATE,     /* No Other process can memory map this file as long as this program is using it */
                   fd,              /* FD represents the identifier of the external data source, in this case it is a file */
                   0);              /* Offset into the text file from which the mapping has to begin, pass zero to map from the
                                            beginning of the file */

  if (ptr == MAP_FAILED)
  {
    printf("Mapping Failed, errorno = %d\n", errno);
    return NULL;
  }
  close(fd);
  /* We will not going to read/write into file using file-handling routines, hence close the fd. mmap can still
                        use this file even if its fd is closed */

  printf("searching about: %s\n", mac.macAddress);
  ipAddress_t *ipAddresses;
  memcpy(ipAddresses, ptr, sz * sizeof(ipAddress_t));
  ipAddress_t *toReturn = NULL;
  for (int i = 0; i < sz; i++)
  {
    if (ipAddresses[i].macAddress[0] == 0)
      continue;
    if (strcmp(ipAddresses[i].macAddress, mac.macAddress) == 0)
    {
      toReturn = &ipAddresses[i];
      break;
    }
  }

  err = munmap(ptr, statbuf.st_size); /* Destroy the mapping once done */

  if (err != 0)
  {
    printf("UnMapping Failed\n");
    return NULL;
  }
  return toReturn;
}

void addNewIpEntry(ipAddress_t **arr, int sz, ipAddress_t newData)
{
  for (int i = 0; i < sz; i++)
  {

    if ((*arr)[i].macAddress[0] == 0)
    {
      (*arr)[i] = newData;
      break;
    }
  }
}

void enter_ipAddress(ipAddress_t *newToAdd)
{
  printf("Enter up address: \n");
  scanf("%32s", newToAdd->ipAddress);
  flushInputBuffer();
}