#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmd_util.h"
#include "entry_log.h"
#include "help_cmd.h"
#include "in_cmd.h"

/*---------- Prototypes ----------*/

int out_cmd(int argc, char **argv);
int summary_cmd(int argc, char **argv);

/*---------- Implementations ----------*/

int main(int argc, char **argv) {
  int status = 0;
  if (argc < 2) {
    help_cmd(0, NULL);
  } else if (0 == strncmp("in", argv[1], 3)) {
    status = in_cmd(argc - 2, argv + 2);
  } else if (0 == strncmp("out", argv[1], 4)) {
    status = out_cmd(argc - 2, argv + 2);
  } else if (0 == strncmp("summary", argv[1], 8)) {
    status = summary_cmd(argc - 2, argv + 2);
  } else {
    help_cmd(argc - 2, argv + 2);
  }
  return status;
}


int out_cmd(int argc, char **argv) {
  if (show_cmd_help("out", argc, argv)) {
    return 0;
  }
  puts("TODO: implement out command.");
  return 0;
}

int summary_cmd(int argc, char **argv) {
  if (show_cmd_help("summary", argc, argv)) {
    return 0;
  }
  puts("TODO: implement summary command.");
  return 0;
}
