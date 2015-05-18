#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "queue.h"

#define MAXNESTING 10

int validate(char *);
void parse_cmd(char *, Queue *queues);
