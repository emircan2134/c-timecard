#include <stdio.h>
#include "cmd_util.h"
#include "entry_log.h"
#include "help_cmd.h"
#include "iso8601.h"
#include "out_cmd.h"

/*---------- Private Prototype ----------*/

int out_cmd_with_fh(FILE *fh, void *ctx);
int out_cmd_with_log(t_entry_log *log, void *ctx);

/*---------- Implementations ----------*/

int out_cmd(int argc) {
  if (argc > 0) {
    char *cmd = "out";
    help_cmd(1, &cmd);
    return EXIT_USAGE;
  }

  int did_punch_out = with_open_log_file(NULL, out_cmd_with_fh);
  return (0 != did_punch_out);
}

int out_cmd_with_fh(FILE *fh, void *ctx) {
  (void)ctx; // unused
  t_entry *cur_entry = NULL;
  int can_punch_out = with_parsed_log(fh, &cur_entry, out_cmd_with_log);

  if (-1 != can_punch_out && NULL != cur_entry) {
    write_out(fh);
    entry_free(cur_entry);
    return 0;
  } else {
    return can_punch_out;
  }
}

void write_out(FILE *fh) {
  time_t t = time(NULL);
  struct tm tt;
  localtime_r(&t, &tt);
  fseek(fh, -1, SEEK_END);
  if ('\n' != fgetc(fh)) {
    fseek(fh, 0, SEEK_END);
  }
  fputc('\t', fh);
  char time[ISO8601_STR_LEN];
  strfiso8601(time, ISO8601_STR_LEN, &tt);
  fputs(time, fh);
  fputc('\n', fh);
}

int out_cmd_with_log(t_entry_log *log, void *ctx) {
  t_entry_log *tail = log;
  while (NULL != tail && NULL != tail->next) {
    tail = tail->next;
  }
  if (NULL == tail || NULL != tail->entry->out) {
    fprintf(stderr, "You are not punched in.\n");
    return -1;
  }
  t_entry **t_ctx = (t_entry **)ctx;
  *t_ctx = entry_dup(tail->entry);
  return 0;
}
