#include <signal.h>

#ifndef SERVER
#define SERVER

int const MAX_BACKLOG = 5;

int const MAX_THREADS = 5;

static volatile sig_atomic_t keep_running = 1;

#endif
