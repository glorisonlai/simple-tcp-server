#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // Used for universal read/write fd (recv(socket.h) would work otherwise)
#include <netinet/in.h>

int const MAX_BACKLOG = 5;

static volatile sig_atomic_t keep_running = 1;

void error(const char *msg) {
  perror(msg);
  exit(1);
}

static void sig_handler(int _)
{
  (void)_;
  keep_running = 0;
}

int main(int argc, char *argv[1]) {
  if (argc < 2) {
    fprintf(stderr, "Port No not provided. Program terminated\n");
    return 1;
  }

  int sockfd, newsockfd, portno;
  char buffer[255];
  int err;

  struct sockaddr_in serv_addr, cli_addr;
  socklen_t clilen;

  // Beej Programming Guide
  // Address families were thought to maybe support several protocols, which didn't end up happening.
  // Effectively same thing (sys/socket.h:20)
  // PF_INET in socket call
  // AF_INET in struct sockaddr_in
  // beej.us/guide/bgnet/html/#system-calls-or-bust
  sockfd = socket(PF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    error("Could not allocate socket");
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(portno);

  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    error("Binding failed.");
  }

  err = listen(sockfd, MAX_BACKLOG);

  if (err < 0) {
    error("Listening failed.");
  } else {
    printf("Listening on %d:\n", portno);
  }

  clilen = sizeof(cli_addr);

  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

  if (newsockfd < 0) {
    error("Error on Accept");
  } else {
    printf("Accepted Client!\n");
  }
  
  while (keep_running) {
    bzero(buffer, 255);
    err = read(newsockfd, buffer, 255);

    if (err < 0) {
      error("Error on Read");
    }
    printf("Client : %s\n", buffer);

    bzero(buffer, 255);
    fgets(buffer, 255, stdin);

    err = write(newsockfd, buffer, strlen(buffer));
    if (err < 0) {
      error("Error on Write");
    }

    int i = strcmp("Bye\n", buffer);
    if (i == 0) {
      break;
    }
  }

  close(newsockfd);
  close(sockfd);

  return 0;
}
