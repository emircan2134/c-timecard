#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmd_util.h"
#include "entry_log.h"
#include "help_cmd.h"
#include "in_cmd.h"
#include "out_cmd.h"

/*---------- Prototypes ----------*/

bool show_cmd_help(int argc, char **argv);
int summary_cmd(int argc, char **argv);

/*---------- Implementations ----------*/

int main(int argc, char **argv) {
  int status = EXIT_SUCCESS;
  if (argc < 2) {
    help_cmd(0, NULL);
    status = EXIT_USAGE;
  } else if (true == show_cmd_help(argc - 2, argv + 2)) {
    help_cmd(1, &argv[1]);
  } else if (0 == strncmp("in", argv[1], 3)) {
    status = in_cmd(argc - 2, argv + 2);
  } else if (0 == strncmp("out", argv[1], 4)) {
    status = out_cmd(argc - 2);
  } else if (0 == strncmp("summary", argv[1], 8)) {
    status = summary_cmd(argc - 2, argv + 2);
  } else {
    help_cmd(argc - 2, argv + 2);
  }
  return status;
}

/**
 * if `argv[0]` is '-h' or '--help', return `true`, otherwise return `false`.
 */
bool show_cmd_help(int argc, char **argv) {
  return (argc > 0 && (0 == strcmp(argv[0], "-h") || 0 == strcmp(argv[0], "--help")));
}


int summary_cmd(int argc, char **argv) {
  (void)argc; // not used yet
  (void)argv; // not used yet
  puts("TODO: implement summary command.");
  return EXIT_SUCCESS;
}
