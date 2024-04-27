#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct arguments {
  int b, n, s, E, T, v;
} arguments;

arguments argument_parser(int argc, char** argv) {
  arguments config = {0};
  struct option long_options[] = {{"number", no_argument, NULL, 'n'},
                                  {"number-nonblank", no_argument, NULL, 'b'},
                                  {"squeeze-blank", no_argument, NULL, 's'},
                                  {0, 0, 0, 0}};
  int option;
  option = getopt_long(argc, argv, "bneEtTvs", long_options, 0);
  switch (option) {
    case 'b':
      config.b = 1;
      break;
    case 'n':
      config.n = 1;
      break;
    case 's':
      config.s = 1;
      break;
    case 'e':
      config.E = 1;
      config.v = 1;
      break;
    case 'E':
      config.E = 1;
      break;
    case 't':
      config.T = 1;
      config.v = 1;
      break;
    case 'T':
      config.T = 1;
      break;
    case '?':
      perror("ERROR");
      exit(1);
      break;
    default:
      break;
  }
  return config;
}

void read_file() {}

char print_v_line(char ch) {
  if (ch == '\n' || ch == '\t') {
    return ch;
  }
  if (ch <= 31) {
    putchar('^');
    ch = ch + 64;
  } else if (ch == 127) {
    putchar('^');
    ch = '?';
  }
  return ch;
}

void print_line(arguments* config, char* line, int n) {
  for (int i = 0; i < n; i++) {
    int flag = 0;
    if (config->T && line[i] == '\t') {
      printf("^I");
      flag = 1;
    }
    if (config->E && line[i] == '\n') {
      putchar('$');
    }
    if (config->v) {
      line[i] = print_v_line(line[i]);
    }
    if (flag == 0) {
      putchar(line[i]);
    }
  }
}

void output(arguments* config, char** argv) {
  FILE* f = fopen(argv[optind], "r");
  char* line = NULL;
  size_t memline = 0;
  int ch = 0;
  int line_count = 1;
  int empty_line = 0;
  ch = getline(&line, &memline, f);
  while (ch != -1) {
    if (line[0] == '\n') {
      empty_line++;
    } else {
      empty_line = 0;
    }
    if (config->s && empty_line > 1) {
    } else {
      if (config->b || config->n) {
        if (config->b && line[0] != '\n') {
          printf("%6d\t", line_count);
          line_count++;
        } else if (config->n) {
          printf("%6d\t", line_count);
          line_count++;
        }
      }
      print_line(config, line, ch);
    }
    ch = getline(&line, &memline, f);
  }
  free(line);
  fclose(f);
}

int main(int argc, char** argv) {
  arguments config = argument_parser(argc, argv);
  output(&config, argv);
  return 0;
}