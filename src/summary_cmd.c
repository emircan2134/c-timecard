#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmd_util.h"
#include "col_writer.h"
#include "entry_log.h"
#include "help_cmd.h"
#include "summary.h"
#include "summary_cmd.h"

/*--------- Private Prototypes ---------*/

typedef void (*t_formatter_fn)(t_summary *);

int summary_cmd_with_fh(FILE *fh, void *ctx);
int summary_cmd_with_log(t_entry_log *log, void *ctx);
void print_summary_human(t_summary *summaries);
void print_summary_csv(t_summary *summaries);

/*---------- Implementations ----------*/

int summary_cmd(int argc, char **argv) {
  t_formatter_fn formatter = print_summary_human;
  if (argc > 0 && 0 == strncmp("--csv", argv[0], 5)) {
    formatter = print_summary_csv;
  }

  int exit_code = 0;
  if (isatty(fileno(stdin))) {
    exit_code = with_open_log_file((void *)formatter, summary_cmd_with_fh);
  } else {
    exit_code = summary_cmd_with_fh(stdin, (void *)formatter);
  }
  return exit_code;
}

int summary_cmd_with_fh(FILE *fh, void *ctx) {
  return with_parsed_log(fh, ctx, summary_cmd_with_log);
}

int summary_cmd_with_log(t_entry_log *log, void *ctx) {
  t_summary *summaries = summaries_build(log);
  t_formatter_fn formatter = (t_formatter_fn)ctx;
  formatter(summaries);
  summary_free(summaries);
  return 0;
}

void print_summary_human(t_summary *summaries) {
  t_col_writer writer = {
    .io = stdout,
    .colcount = 2,
    .padding = 2,
  };
  t_summary *summary = summaries;
  size_t col_max = MAX_ENTRY_LINE_LEN / 2;
  char buf0[col_max], buf1[col_max];
  char *cols[2] = {buf0, buf1};

  while (NULL != summary) {
    assert(0 < strftime(buf0, col_max, "%A, %d %B %Y", &summary->day));
    printf("%s\n", buf0);

    for (unsigned long idx = 0; idx < summary->count; idx++) {
      t_project_hours e = summary->entries[idx];
      snprintf(buf0, col_max, "\t%s:", e.project);
      snprintf(buf1, col_max, "%.2f", e.hours);
      cw_add_line(&writer, cols);
    }
    snprintf(buf0, col_max, "\tDAY TOTAL:");
    snprintf(buf1, col_max, "%.2f", summary_duration(summary));
    cw_add_line(&writer, cols);
    cw_flush(&writer);

    summary = summary->next;
  }
}

void print_summary_csv(t_summary *summaries) {
  char **projects = NULL;
  char buf[MAX_ENTRY_LINE_LEN];
  t_summary *summary = summaries;

  unsigned long projects_count = summaries_all_projects(summaries, &projects);

  printf("Project");
  while (NULL != summary) {
    assert(0 < strftime(buf, MAX_ENTRY_LINE_LEN, "%Y-%m-%d", &summary->day));
    printf(",%s", buf);

    summary = summary->next;
  }
  printf("\n");

  for (unsigned long pidx = 0; pidx < projects_count; pidx++) {
    printf("%s", projects[pidx]);
    summary = summaries;
    while (NULL != summary) {
      printf(",%.2f", summary_duration_for_project(summary, projects[pidx]));
      summary = summary->next;
    }
    printf("\n");
  }

  free(projects);
}

