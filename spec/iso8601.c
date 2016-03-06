#include "spec_helper.h"
#include "../iso8601.h"

void spec_format_string_utc(void) {
  struct tm t = fixture_time();
  char t_formatted[ISO8601_STR_LEN],
       *expected = "2016-03-04T14:05:30+00:00";

  SET_TZ("utc");

  size_t written = strfiso8601(t_formatted, ISO8601_STR_LEN, &t);
  sp_assert(written > 0);
  sp_assert_equal_str(expected, t_formatted);

  RESET_TZ
}

void spec_format_string_est(void) {
  struct tm t = fixture_time();
  char t_formatted[ISO8601_STR_LEN],
       *expected = "2016-03-04T14:05:30-05:00";

  SET_TZ("est");

  size_t written = strfiso8601(t_formatted, ISO8601_STR_LEN, &t);
  sp_assert(written > 0);
  sp_assert_equal_str(expected, t_formatted);

  RESET_TZ
}

void spec_parse_string_success(void) {
  char *time = "2016-03-04T14:05:30-05:00";
  struct tm t = {};

  SET_TZ("utc");

  char *next = strpiso8601(time, &t);
  sp_assert(NULL != next);
  sp_assert_equal_i(116, t.tm_year);
  sp_assert_equal_i(2, t.tm_mon);
  sp_assert_equal_i(19, t.tm_hour); // tz offset got handled
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
