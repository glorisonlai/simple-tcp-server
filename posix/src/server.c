#include "clientHandler.h"
#include "server.h"
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h> // Used for universal read/write fd (recv(socket.h) would work otherwise)
#include <netinet/in.h>
#include <pthread.h>

void error(const char *msg)
{
  perror(msg);
  exit(1);
}

static void sig_handler(int _)
{
    (void)_;
    keep_running = 0;
}

void runServer(int sockfd, int portno)
{
  int err;
  err = listen(sockfd, MAX_BACKLOG);

  if (err < 0)
  {
    error("Listening failed.");
  }
  else
  {
    printf("Listening on %d:\n", portno);
  }

  int newsockfd, threadCounter = 0;
  struct clientHandlerArgs args;

  while (1) {
    if (keep_running == 0) {
      break;
    }

    if (threadCounter >= MAX_THREADS) {
      fprintf(stderr, "Max threads reached. Dropping connection\n");
      printf("Max threads reached. Dropping connection\n");
      continue;
    }

    struct sockaddr_in servAddr;
    socklen_t clilen = sizeof(servAddr);

    newsockfd = accept(sockfd, (struct sockaddr *) &servAddr, &clilen);

    if (newsockfd < 0) {
      error("Error on Accept");
    } else {
      printf("Accepted Client!\n");
    }

    // Spawn new thread
    pthread_t threadId;

    args.threadId = threadCounter;
    args.sockFd = newsockfd;

    err = pthread_create(&threadId, NULL, clientHandler, (void *) &args);
    if (err != 0)
    {
      fprintf(stderr, "Could not create thread from connection - dropping connection \n");
      close(newsockfd);
      continue;
    }

    threadCounter++;
  }
}

int main(int argc, char *argv[1])
{
  if (argc < 2)
  {
    fprintf(stderr, "Port No not provided. Program terminated\n");
    return 1;
  }

  signal(SIGINT, sig_handler);

  int sockfd, portno;

  struct sockaddr_in serv_addr;

  // Beej Programming Guide
  // Address families were thought to maybe support several protocols, which didn't end up happening.
  // Effectively same thing (sys/socket.h:20)
  // PF_INET in socket call
  // AF_INET in struct sockaddr_in
  // beej.us/guide/bgnet/html/#system-calls-or-bust
  sockfd = socket(PF_INET, SOCK_STREAM, 0);

  if (sockfd < 0)
  {
    error("Could not allocate socket");
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(portno);

  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
  {
    error("Binding failed.");
  }

  runServer(sockfd, portno);

  close(sockfd);

  return 0;
}
