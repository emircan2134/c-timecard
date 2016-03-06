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
  char strentry[255],
       *expected = "c-timecard\t2016-03-04T14:05:30-05:00\t2016-03-04T14:35:30-05:00\n";

  SET_TZ("est");
  size_t written = strfentry(strentry, 255, &entry);
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
  char strentry[255],
       *expected = "c-timecard\t2016-03-04T14:05:30-05:00";

  SET_TZ("est");
  size_t written = strfentry(strentry, 255, &entry);
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
  char strentry[255];

  size_t written = strfentry(strentry, 255, &entry);
  sp_assert(0 == written);;

  entry.project = "c-timecard";
  entry.in = NULL;
  written = strfentry(strentry, 255, &entry);
  sp_assert(0 == written);
}

void spec_strpentry_full_entry(void) {
  /*char *line = "c-timecard\t2016-03-04T14:05:30-05:00\t2016-03-04T14:35:30-05:00";*/
  /*t_entry entry;*/
  //TODO
}

void spec_strpentry_half_entry(void) {
  /*char *line = "c-timecard\t2016-03-04T14:05:30-05:00";*/
  /*t_entry entry;*/
  //TODO
}

void spec_strpentry_invalid_entry(void) {
  /*char *line = "not-a-real-entry-line";*/
  /*t_entry entry;*/
  //TODO
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
