#ifndef __DEPENDENCIES_H__
#define __DEPENDENCIES_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>

#define USED_PORT       5000
#define BACKLOG         10
#define MAXBUFFERSIZE   255

void showMessageError(char *message);

#endif