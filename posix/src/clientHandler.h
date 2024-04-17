#ifndef CLIENT_HANDLER
#define CLIENT_HANDLER

struct clientHandlerArgs {
    int threadId;
    int sockFd;
};

void *clientHandler(void *vargp);

#endif
