#include <stdio.h>
#include "spec_helper.h"
#include "entry_log.h"

/***  helper prototypes  ***/
long log_len(t_entry_log *log);
FILE *tmp_log_file(char *contents);

/***  Tests  ***/

void spec_test_parse_success1(void) {
  FILE *fh = tmp_log_file(
               "c-timecard\t2016-03-04T14:05:30-05:00\t2016-03-04T14:35:30-05:00\n" \
               "go-timecard\t2016-03-05T14:05:30-05:00\t2016-03-05T14:35:30-05:00\n" \
               "c-timecard\t2016-03-06T14:05:30-05:00\t2016-03-06T14:35:30-05:00\n"
             );

  t_entry_log *log = log_parse(fh, NULL);
  sp_assert(3 == log_len(log));
  sp_assert_equal_str("c-timecard", log->entry->project);
  sp_assert_equal_str("go-timecard", log->next->entry->project);

  fclose(fh);
  log_free(log);
}

void spec_test_parse_success2(void) {
  FILE *fh = tmp_log_file(
               "c-timecard\t2016-03-04T14:05:30-05:00\t2016-03-04T14:35:30-05:00\n" \
               "go-timecard\t2016-03-05T14:05:30-05:00\t2016-03-05T14:35:30-05:00\n" \
               "c-timecard\t2016-03-06T14:05:30-05:00"
             );

  t_entry_log *log = log_parse(fh, NULL);
  sp_assert(3 == log_len(log));
  sp_assert_equal_str("c-timecard", log->entry->project);
  sp_assert_equal_str("go-timecard", log->next->entry->project);

  fclose(fh);
  log_free(log);
}

void spec_test_parse_fail1(void) {
  FILE *fh = tmp_log_file(
               "c-timecard\t2016-03-04T14:05:30-05:00\t2016-03-04T14:35:30-05:00\n" \
               "go-timecard"
             );

  // nothing bad should happen when failing, even if NULL was passed as err.
  t_entry_log *log = log_parse(fh, NULL);
  sp_assert(NULL == log);

  fclose(fh);
}

void spec_test_parse_fail2(void) {
  FILE *fh = tmp_log_file(
               "c-timecard\t2016-03-04T14:05:30-05:00\t2016-03-04T14:35:30-05:00\n" \
               "go-timecard"
             );

  char *err;
  t_entry_log *log = log_parse(fh, &err);
  sp_assert(NULL == log);
  sp_assert(NULL != err);
  sp_assert(err == strstr(err, "Error parsing log line"));

  fclose(fh);
}

void spec_test_append(void) {
  struct tm t = fixture_time();
  time_t in = mktime(&t),
         out = in + (60 * 30);
  t_entry e1 = {.project = "p1", .in = &in, .out = &out},
          e2 = {.project = "p2", .in = &in, .out = &out},
          e3 = {.project = "p3", .in = &in, .out = NULL};

  t_entry_log *log = log_append(NULL, &e1);
  sp_assert(1 == log_len(log));
  sp_assert_equal_str("p1", log->entry->project);

  t_entry_log *log2 = log_append(log, &e2);
  sp_assert(2 == log_len(log));
  sp_assert_equal_str("p2", log->next->entry->project);
  sp_assert_equal_str("p2", log2->entry->project);
  sp_assert(log->next->prev == log);
  sp_assert(log->next == log2);

  log_append(log, &e3);
  sp_assert(3 == log_len(log));
  sp_assert_equal_str("p3", log->next->next->entry->project);
  sp_assert(log->next->next->prev == log->next);

  log_free(log);
}

/***  helper impls  ***/

long log_len(t_entry_log *log) {
  long l = 0;
  while (NULL != log) {
    l++;
    log = log->next;
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
