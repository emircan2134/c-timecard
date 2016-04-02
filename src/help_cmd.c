#include <stdio.h>
#include <string.h>
#include "help_cmd.h"

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
