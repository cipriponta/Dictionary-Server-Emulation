#include "dependencies.h"

void showMessageError(char *message)
{
    perror(message);
    exit(1);
}