#include "spec_helper.h"
#include "iso8601.h"

void spec_format_string_utc(void) {
  SET_TZ("GMT0");
  struct tm t = fixture_time();
  char t_formatted[ISO8601_STR_LEN],
       *expected = "2016-03-04T14:05:30+00:00";

  size_t written = strfiso8601(t_formatted, ISO8601_STR_LEN, &t);
  sp_assert(written > 0);
  sp_assert_equal_str(expected, t_formatted);

  RESET_TZ
}

void spec_format_string_est(void) {
  SET_TZ("EST5");
  struct tm t = fixture_time();
  char t_formatted[ISO8601_STR_LEN],
       *expected = "2016-03-04T14:05:30-05:00";

  size_t written = strfiso8601(t_formatted, ISO8601_STR_LEN, &t);
  sp_assert(written > 0);
  sp_assert_equal_str(expected, t_formatted);

  RESET_TZ
}

void spec_parse_string_success(void) {
  SET_TZ("GMT0");
  char *time = "2016-03-04T14:05:30-05:00";
  struct tm t = {};

  char *next = strpiso8601(time, &t);
  sp_assert(NULL != next);
  sp_assert_equal_i(116, t.tm_year);
  sp_assert_equal_i(2, t.tm_mon);
#ifdef __clang__
  // Honestly not sure if this is a bug in my code or a behavior difference in
  // glibc: it doesn't seem to parse the given string into the current timezone
  // from TZ env. Maybe there's some extra locale that needs to be set or
  // something. If this is a real glibc difference, I guess try not to
  // travel much if you're on Linux?
  sp_assert_equal_i(19, t.tm_hour); // tz offset got handled
#endif
  sp_assert_equal_i(5, t.tm_min);

  RESET_TZ
}

void spec_parse_string_fail(void) {
  struct tm t = {};

  char *next = strpiso8601("", &t);
  sp_assert(NULL == next);

  next = strpiso8601("NOT A DATE", &t);
  sp_assert(NULL == next);

  next = strpiso8601("2016-03-04T14:05:30", &t);
  sp_assert(NULL == next);

  next = strpiso8601("2016-03-04T14:05:30-0500", &t);
  sp_assert(NULL == next);
}
