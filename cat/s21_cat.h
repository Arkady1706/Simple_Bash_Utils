#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct arguments {
  int b, n, s, E, T, v;
} arguments;

arguments argument_parser(int argc, char** argv);
void output(arguments* config, char** argv);
void print_line(arguments* config, char* line, int n);
char print_v_line(char ch);

#endif  // S21_CAT_H