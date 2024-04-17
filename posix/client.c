#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

void error(const char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {
  int sockfd, portno;
  int err;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[255];
  if (argc < 3) {
    fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
    exit(1);
  }

  portno = atoi(argv[2]);
  sockfd = socket(PF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    error ("Error opening socket");
  }

  server = gethostbyname(argv[1]);
  if (server == NULL) {
    fprintf(stderr, "Error: no such host");
    exit(1);
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno);

  err = connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
  if (err < 0) {
    error("Could not connect to server");
  }

  while (1) {
    bzero(buffer, 255);
    fgets(buffer, 255, stdin);
    err = write(sockfd, buffer, strlen(buffer));
    if (err < 0) {
      error("Error on Write");
    }
    bzero(buffer, 255);
    err = read(sockfd, buffer, 255);
    if (err < 0) {
      error("Error on Read");
    }

    int i = strcmp("Bye\n", buffer);
    if (i == 0) {
      break;
    }
  }

  close(sockfd);
  return 0;
}

