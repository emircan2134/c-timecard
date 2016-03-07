#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "entry_log.h"

/*---------- Prototypes ----------*/

void help_cmd(int argc, char **argv);
int in_cmd(int argc, char **argv);
int out_cmd(int argc, char **argv);
int summary_cmd(int argc, char **argv);
bool show_cmd_help(char *cmd, int argc, char **argv);

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

void help_cmd(int argc, char **argv) {
  if (argc > 0 && 0 == strcmp(argv[0], "in")) {
    puts("usage: punch in <project name>");
  } else if (argc > 0 && 0 == strcmp(argv[0], "out")) {
    puts("usage: punch out");
  } else if (argc > 0 && 0 == strcmp(argv[0], "summary")) {
    puts("usage: punch summary [--csv]");
  } else {
    puts("Punch is a simple time tracker.\n\n" \
         "Usage:\n\n" \
         "\tpunch <command> [arguments]\n\n" \
         "The commands are:\n\n" \
         "\tin\tpunch in to a project\n" \
         "\tout\tpunch out of a project\n" \
         "\tsummary\tsummarize your time");
  }
}

int in_cmd(int argc, char **argv) {
  if (show_cmd_help("in", argc, argv)) {
    return 0;
  }
  puts("TODO: implement in command.");
  return 0;
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

bool show_cmd_help(char *cmd, int argc, char **argv) {
  if (argc > 0 && (0 == strcmp(argv[0], "-h") || 0 == strcmp(argv[0], "--help"))) {
    help_cmd(1, &cmd);
    return true;
  }
  return false;
}
