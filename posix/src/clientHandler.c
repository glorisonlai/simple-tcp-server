#include "clientHandler.h"
#include <stdio.h>
#include <unistd.h>
#include <strings.h>

void *clientHandler(void *vargp)
{
  struct clientHandlerArgs *args = (struct clientHandlerArgs *)vargp;

  int sockfd = args->sockFd;
  int threadId = args->threadId;
  int err;
  char buffer[255];

  while (1)
  {
    bzero(buffer, 255);

    err = read(sockfd, buffer, 255);

    if (err < 0) {
      fprintf(stderr, "Error on Read");
    }

    printf("Client %d: %s\n", threadId, buffer);
    err = write(sockfd, "done", 4);
  }

  close(sockfd);

  return NULL;
}
