#ifndef GETCH_H
#define GETCH_H

#include "getch.c"

void initTermios(int echo);

void resetTermios();

char getch_(int echo);

char getch(void);
#endif
