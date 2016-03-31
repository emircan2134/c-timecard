#include "spec_helper.h"
#include "../summary.h"

/***  helper prototypes  ***/
long summary_len(t_summary *list);
t_entry_log *entry_log_fixture(void);

/***  Tests  ***/

void spec_summaries_build(void) {
  t_entry_log *log = entry_log_fixture();
  t_summary *summary = summaries_build(log);
  sp_assert_equal_i(2, summary_len(summary));
  sp_assert_equal_i(2, summary[0].count);
  sp_assert_equal_str("c-timecard", summary->entries[0].project);
  sp_assert_equal_d(0.75, summary->entries[0].hours);
  sp_assert_equal_str("go-timecard", summary->entries[1].project);
  sp_assert_equal_d(1.0, summary->entries[1].hours);
  sp_assert_equal_i(1, summary->next->count);
  sp_assert_equal_str("c-timecard", summary->next->entries[0].project);
  sp_assert_equal_d(0.5, summary->next->entries[0].hours);

  summary_free(summary);
  log_free(log);
}

void spec_summary_duration(void) {
  t_entry_log *log = entry_log_fixture();
  t_summary *summary = summaries_build(log);

  sp_assert_equal_d(1.75, summary_duration(summary));

  summary_free(summary);
  log_free(log);
}

void spec_summary_duration_for_project(void) {
  t_entry_log *log = entry_log_fixture();
  t_summary *summary = summaries_build(log);

  sp_assert_equal_d(1.0, summary_duration_for_project(summary, "go-timecard"));
  sp_assert_equal_d(0.0, summary_duration_for_project(summary, "not-a-project"));

  summary_free(summary);
  log_free(log);
}

void spec_summaries_all_projects(void) {
  t_entry_log *log = entry_log_fixture();
  t_summary *summary = summaries_build(log);
  char **projects = NULL;

  unsigned long projects_count = summaries_all_projects(summary, &projects);
  sp_assert_equal_i(2, projects_count);
  sp_assert_equal_str("c-timecard", projects[0]);
  sp_assert_equal_str("go-timecard", projects[1]);

  free(projects);
  summary_free(summary);
  log_free(log);
}

/***  helper impls  ***/

long summary_len(t_summary *list) {
  long l = 0;
  while (NULL != list) {
    l++;
    list = list->next;
  }
  return l;
}

FILE *tmp_log_file(char *contents) {
  FILE *fh = tmpfile();
  fputs(contents, fh);
  fflush(fh);
  fseek(fh, 0, SEEK_SET);
  return fh;
}

t_entry_log *entry_log_fixture(void) {
  FILE *fh = tmp_log_file(
      "c-timecard\t2016-03-04T14:05:00-05:00\t2016-03-04T14:35:00-05:00\n" \
      "go-timecard\t2016-03-04T15:00:00-05:00\t2016-03-04T16:00:00-05:00\n" \
      "c-timecard\t2016-03-04T16:10:00-05:00\t2016-03-04T16:25:00-05:00\n" \
      "c-timecard\t2016-03-06T14:05:00-05:00\t2016-03-06T14:35:00-05:00\n"
  );

  t_entry_log *log = log_parse(fh, NULL);
  fclose(fh);
  return log;
}
