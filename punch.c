#include <stdio.h>
#include <string.h>

/*---------- Prototypes ----------*/

void help_cmd(int argc, char **argv);
void in_cmd(int argc, char **argv);
void out_cmd(int argc, char **argv);
void summary_cmd(int argc, char **argv);

/*---------- Implementations ----------*/

int main(int argc, char **argv) {
  if (argc < 2) {
    help_cmd(0, NULL);
  } else if (0 == strncmp("in", argv[1], 3)) {
    in_cmd(argc - 2, argv + 2);
  } else if (0 == strncmp("out", argv[1], 4)) {
    out_cmd(argc - 2, argv + 2);
  } else if (0 == strncmp("summary", argv[1], 8)) {
    summary_cmd(argc - 2, argv + 2);
  } else {
    help_cmd(argc - 2, argv + 2);
  }
  return 0;
}

void help_cmd(int argc, char **argv) {
  (void) argc; (void) argv; // not used yet
  puts("Punch is a simple time tracker.\n\n" \
       "Usage:\n\n" \
       "\tpunch <command> [arguments]\n\n" \
       "The commands are:\n\n" \
       "\tin\tpunch in to a project\n" \
       "\tout\tpunch out of a project\n" \
       "\tsummary\tsummarize your time");
}

void in_cmd(int argc, char **argv) {
  (void) argc; (void) argv; // not used yet
  puts("TODO: implement in command.");
}

void out_cmd(int argc, char **argv) {
  (void) argc; (void) argv; // not used yet
  puts("TODO: implement out command.");
}

void summary_cmd(int argc, char **argv) {
  (void) argc; (void) argv; // not used yet
  puts("TODO: implement summary command.");
}
