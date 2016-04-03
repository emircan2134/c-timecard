#include <stdio.h>
#include "cmd_util.h"
#include "entry_log.h"
#include "help_cmd.h"
#include "in_cmd.h"

/*---------- Private Prototype ----------*/

int in_cmd_with_fh(FILE *fh, void *ctx);
int in_cmd_with_log(t_entry_log *log, void *ctx);

/*---------- Implementations ----------*/

int in_cmd(int argc, char **argv) {
  if (0 == argc) {
    char *cmd = "in";
    help_cmd(1, &cmd);
    return EXIT_USAGE;
  }

  int did_punch_in = with_open_log_file(argv[0], in_cmd_with_fh);
  return (0 != did_punch_in);
}

int in_cmd_with_fh(FILE *fh, void *ctx) {
  int can_punch_in = with_parsed_log(fh, NULL, in_cmd_with_log);

  if (-1 != can_punch_in) {
    char *project = (char *)ctx;
    time_t now = time(NULL);
    t_entry entry = {.project = project, .in = &now, .out = NULL};
    fseek(fh, 0, SEEK_END);
    char line[MAX_ENTRY_LINE_LEN];
    strfentry(line, MAX_ENTRY_LINE_LEN, &entry);
    fputs(line, fh);
    printf("Punched into %s\n", project);
    return 0;
  } else {
    return can_punch_in;
  }
}

int in_cmd_with_log(t_entry_log *log, void *ctx) {
  (void)ctx; //unused
  t_entry_log *tail = log;
  while (NULL != tail && NULL != tail->next) {
    tail = tail->next;
  }
  if (tail != NULL && NULL == tail->entry->out) {
    fprintf(stderr, "You are still punched in to %s. Please punch out.\n", tail->entry->project);
    return -1;
  }
  return 0;
}
