#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char options[] = "AbeEnstTvhV";

int opt_line_no = 0;
int opt_show_ends = 0;
int opt_show_nonprinting = 0;
int opt_show_tabs = 0;
int opt_help = 0;
int opt_number_non_empty = 0;
int opt_squeeze_blanks = 1;
int opt_version = 1;

char help[] = "Concatenate FILE(s), or standard input, to standard output\n\
With no FILE, or when FILE is -, read standard input.\n\
\n\
Usage: cat [OPTION]... [FILE]...\n\
\n\
Options:\n\
  -A, --show-all          equivalent to -vET\n\
  -b, --number-nonblank   number nonempty output lines, overrides -n\n\
  -e                      equivalent to -vE\n\
  -E, --show-ends         display $ at end of each line\n\
  -n, --number            number all output lines\n\
  -s, --squeeze-blank     suppress repeated empty output lines\n\
  -t                      equivalent to -vT\n\
  -T, --show-tabs         display TAB characters at ^I\n\
  -v, --show-nonprinting  use ^ and M- notation, except for LF (\\n) and TAB (\\t)\n\
  -u                      (ignored)\n\
  -h, --help              Print help\n\
  -V, --version           Print version\n\
";

static int is_not_option(char c) { return !strchr(options, c); }

static void main_loop(char *filename) {
  FILE *stream = stdin;
  int c = 0;
  if (filename && strcmp(filename, "-")) {
    stream = fopen(filename, "r");
    if (!stream) {
      fprintf(stderr, "cat: %s: %s\n", filename, strerror(errno));
      return;
    }
  }
  int line_no = 1;
  int line_count = 0;
  while ((c = fgetc(stream)) != EOF) {
    if (line_count == 0 && (opt_line_no || opt_number_non_empty)) {
      if (!opt_number_non_empty || (opt_number_non_empty && c != '\n'))
        fprintf(stdout, "     %d\t", line_no);
      if (opt_number_non_empty && c == '\n') {
        if (opt_show_ends) {
          if (fputc('$', stdout) == EOF)
            break;
        }
        if (fputc('\n', stdout) == EOF)
          break;
        continue;
      }
    }
    if (opt_show_ends && c == '\n') {
      if (fputc('$', stdout) == EOF)
        break;
    }
    if (fputc(c, stdout) == EOF)
      break;
    line_count++;
    if (c == '\n') {
      line_no++;
      line_count = 0;
    }
  }
  fclose(stream);
}

int main(int argc, char *argv[]) {
  int n_opts = 0;
  /*Single value option*/ for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-' || argv[i][1] == '-')
      continue;
    for (int j = 1; argv[i][j]; j++) {
      char c = argv[i][j];
      if (is_not_option(c) && !opt_help) {
        fprintf(stderr,
                "\033[1;31;40merror\033[0m: unexpected argument '-%c' found\n",
                c);
        fprintf(stderr, "Usage: cat [OPTION]... [FILE]...\n\nFor more information, try '--help'.\n");
        return EXIT_FAILURE;
      }
      switch (c) {
      case 'A':
        opt_show_nonprinting = 1;
        opt_show_ends = 1;
        opt_show_tabs = 1;
        break;
      case 'b':
        opt_number_non_empty = 1;
        break;
      case 'e':
        opt_show_nonprinting = 1;
        opt_show_ends = 1;
        break;
      case 'E':
        opt_show_ends = 1;
        break;
      case 'n':
        opt_line_no = 1;
        break;
      case 's':
        opt_squeeze_blanks = 1;
        break;
      case 't':
        opt_show_tabs = 1;
        opt_show_nonprinting = 1;
        break;
      case 'T':
        opt_show_tabs = 1;
        break;
      case 'v':
        opt_show_nonprinting = 1;
        break;
      case 'u':
        /*Ignored*/
        break;
      case 'h':
        opt_help = 1;
        break;
      case 'V':
        opt_version = 1;
        break;
      default:
        break;
      }
    }
    n_opts++;
    argv[i][0] = 0;
  }
  if (opt_help){
      fputs(help, stdout);
      return EXIT_SUCCESS;
  }
  if (argc == n_opts + 1)
    main_loop(NULL);
  for (int i = 1; i < argc; i++) {
    /*it's an option*/ if (!argv[i][0])
      continue;
    main_loop(argv[i]);
  }
}
