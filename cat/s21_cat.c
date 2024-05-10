#include "s21_cat.h"

int main(int argc, char** argv) {
  arguments config = argument_parser(argc, argv);
  file_parser(&config, argc, argv);
  return 0;
}

arguments argument_parser(int argc, char** argv) {
  arguments config = {0};
  struct option long_options[] = {{"number", no_argument, NULL, 'n'},
                                  {"number-nonblank", no_argument, NULL, 'b'},
                                  {"squeeze-blank", no_argument, NULL, 's'},
                                  {0, 0, 0, 0}};
  int option = 0;
  while (option != -1) {
    option = getopt_long(argc, argv, "bneEtTvs", long_options, 0);
    switch (option) {
      case 'b':
        config.b = 1;
        break;
      case 'v':
        config.v = 1;
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
  }
  return config;
}

void file_parser(arguments* config, int argc, char** argv) {
  int line_count = 1;
  int empty_line = 0;
  for (int i = optind; i < argc; i++) {
    FILE* f = fopen(argv[i], "r");
    if (f == NULL) {
      perror("Error opening file");
      exit(1);
    }
    char* line = NULL;
    size_t max = 0;
    int ch = 0;
    ch = getline(&line, &max, f);
    while (ch != -1) {
      if (line[0] == '\n') {
        empty_line++;
      } else {
        empty_line = 0;
      }
      if (config->s && empty_line > 1) {
      } else {
        if (config->b && config->n) {
          config->n = 0;
        }
        if (config->b && line[0] != '\n' && !config->new_line) {
          printf("%6d\t", line_count);
          line_count++;
        } else if (config->n) {
          printf("%6d\t", line_count);
          line_count++;
        }
        print_line(config, line, ch);
      }
      ch = getline(&line, &max, f);
    }
    free(line);
    fclose(f);
  }
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
    if (line[i] != '\n') {
      config->new_line = 1;
    } else {
      config->new_line = 0;
    }
  }
}

char print_v_line(char ch) {
  if (ch == '\n' || ch == '\t') {
    return ch;
  }
  if (ch < 32) {
    putchar('^');
    ch = ch + 64;
  } else if (ch == 127) {
    putchar('^');
    ch = '?';
  }
  return ch;
}