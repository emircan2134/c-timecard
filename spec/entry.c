#include "spec_helper.h"
#include "../entry.h"

void spec_strfentry_full_entry(void) {
  struct tm t = fixture_time();
  time_t in = mktime(&t),
         out = in + (60 * 30);
  t_entry entry = {
    .project = "c-timecard",
    .in = &in,
    .out = &out
  };
  char strentry[MAX_ENTRY_LINE_LEN],
       *expected = "c-timecard\t2016-03-04T14:05:30-05:00\t2016-03-04T14:35:30-05:00\n";

  SET_TZ("est");
  size_t written = strfentry(strentry, MAX_ENTRY_LINE_LEN, &entry);
  sp_assert(written > 0);
  sp_assert_equal_str(expected, strentry);
  RESET_TZ
}

void spec_strfentry_half_entry(void) {
  struct tm t = fixture_time();
  time_t in = mktime(&t);
  t_entry entry = {
    .project = "c-timecard",
    .in = &in,
    .out = NULL
  };
  char strentry[MAX_ENTRY_LINE_LEN],
       *expected = "c-timecard\t2016-03-04T14:05:30-05:00";

  SET_TZ("est");
  size_t written = strfentry(strentry, MAX_ENTRY_LINE_LEN, &entry);
  sp_assert(written > 0);
  sp_assert_equal_str(expected, strentry);
  RESET_TZ
}

void spec_strfentry_invalid_entry(void) {
  struct tm t = fixture_time();
  time_t in = mktime(&t);
  t_entry entry = {
    .project = NULL,
    .in = &in,
    .out = NULL
  };
  char strentry[MAX_ENTRY_LINE_LEN];

  size_t written = strfentry(strentry, MAX_ENTRY_LINE_LEN, &entry);
  sp_assert(0 == written);

  entry.project = "c-timecard";
  entry.in = NULL;
  written = strfentry(strentry, MAX_ENTRY_LINE_LEN, &entry);
  sp_assert(0 == written);
}

void spec_strpentry_full_entry(void) {
  SET_TZ("est");
  char *line = "c-timecard\t2016-03-04T14:05:30-05:00\t2016-03-04T14:35:30-05:00\n",
       *line_heap = heap_str(line);
  t_entry *entry = malloc(sizeof(t_entry));
  char *next = strpentry(line_heap, entry);
  sp_assert(NULL != next);
  sp_assert_equal_str("c-timecard", entry->project);
  sp_assert(NULL != entry->in);
  sp_assert(NULL != entry->out);

  // test round-tripping the data
  char strentry[MAX_ENTRY_LINE_LEN];
  size_t written = strfentry(strentry, MAX_ENTRY_LINE_LEN, entry);
  sp_assert(0 != written);
  sp_assert_equal_str(line, strentry);

  free(line_heap);
  entry_free(entry);
  RESET_TZ
}

void spec_strpentry_half_entry(void) {
  char *line = "c-timecard\t2016-03-04T14:05:30-05:00",
       *line_heap = heap_str(line);
  t_entry *entry = malloc(sizeof(t_entry));

  char *next = strpentry(line_heap, entry);
  sp_assert(NULL != next);
  sp_assert_equal_str("c-timecard", entry->project);
  sp_assert(NULL != entry->in);
  sp_assert(NULL == entry->out);

  free(line_heap);
  entry_free(entry);
}

void spec_strpentry_invalid_entry(void) {
  char *line_heap = heap_str("c-timecard");
  t_entry *entry = malloc(sizeof(t_entry));

  char *next = strpentry(line_heap, entry);
  sp_assert(NULL == next);
  free(line_heap); entry_free(entry); entry = malloc(sizeof(t_entry));

  line_heap = heap_str("c-timecard\t2016-03-04T14:05:30invalid-time");
  next = strpentry(line_heap, entry);
  sp_assert(NULL == next);
  free(line_heap); entry_free(entry); entry = malloc(sizeof(t_entry));

  line_heap = heap_str("c-timecard\t2016-03-04T14:05:30-05:00\t2016-03-04T14:35:30-05:00\textra-field");
  next = strpentry(line_heap, entry);
  sp_assert(NULL == next);
  free(line_heap); entry_free(entry);
}

void spec_entry_duration_invalid(void) {
  t_entry entry = {
    .project = NULL,
    .in = NULL,
    .out = NULL
  };
  sp_assert(-1 == entry_duration(&entry));
}

void spec_entry_duration_half(void) {
  struct tm t = fixture_time();
  time_t in = mktime(&t);
  t_entry entry = {
    .project = "c-timecard",
    .in = &in,
    .out = NULL
  };
  sp_assert(0 == entry_duration(&entry));
}

void spec_entry_duration_full(void) {
  struct tm t = fixture_time();
  time_t in = mktime(&t),
         out = in + (60 * 30);
  t_entry entry = {
    .project = "c-timecard",
    .in = &in,
    .out = &out
  };
  sp_assert(0.5 == entry_duration(&entry));
}

void spec_entry_dup(void) {
  struct tm t = fixture_time();
  time_t in = mktime(&t);
  t_entry *e1 = malloc(sizeof(t_entry));
  e1->project = heap_str("c-timecard");
  e1->in = memcpy(malloc(sizeof(time_t)), &in, sizeof(time_t));
  e1->out = NULL;

  t_entry *e2 = entry_dup(e1);
  sp_assert(e1->project != e2->project); // pointers should be different
  entry_free(e1);
  sp_assert_equal_str("c-timecard", e2->project);
  sp_assert(in == *(e2->in));
  entry_free(e2);
}
