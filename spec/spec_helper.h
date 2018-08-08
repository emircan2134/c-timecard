#include <assert.h>
#include <speck.h>
#include <time.h>
#include "entry.h"
#include "iso8601.h"

#define SET_TZ(tz) char *_old_tz = getenv("TZ"); setenv("TZ", tz, 1); tzset();
#define RESET_TZ if (NULL == _old_tz) { unsetenv("TZ"); } else { setenv("TZ", _old_tz, 1); } tzset();

struct tm fixture_time(void) {
  time_t t = time(NULL);
  struct tm tt;
  localtime_r(&t, &tt);
  tt.tm_sec = 30;
  tt.tm_min = 5;
  tt.tm_hour = 14;
  tt.tm_mday = 4;
  tt.tm_wday = 4;
  tt.tm_mon = 2;
  tt.tm_yday = 62;
  tt.tm_year = 116; // years since 1900. I know, right?
  return tt;
}

char *heap_str(const char *str) {
  int l = strlen(str);
  char *dst = malloc((l + 1) * sizeof(char));
  strcpy(dst, str);
  return dst;
}

t_entry *alloc_entry(void) {
  t_entry *entry = malloc(sizeof(t_entry));
  memset(entry, 0, sizeof(t_entry));
  return entry;
}

#define sp_assert_equal_d(x, y) sp_assert_equal_d_lineno(x, y, __LINE__)

void sp_assert_equal_d_lineno(double x, double y, int lineno)
{
    state.assertions = realloc(state.assertions, (state.index + 1) * sizeof(char *));
    alloc_sprintf(&(state.assertions[state.index]), ":%d -> %s::sp_assert_equal_d(%f, %f)", lineno, state.function, x, y);

    state.codes = realloc(state.codes, (state.index + 1) * sizeof(int));

    if (x == y) {
        state.codes[state.index] = 0;
    } else {
        state.codes[state.index] = 1;
    }

    state.index++;
}

#define sp_assert_equal_tm(x, y) sp_assert_equal_tm_lineno(x, y, __LINE__)

void sp_assert_equal_tm_lineno(struct tm x, struct tm y, int lineno) {
    state.assertions = realloc(state.assertions, (state.index + 1) * sizeof(char *));
    char x_str[ISO8601_STR_LEN], y_str[ISO8601_STR_LEN];
    strfiso8601(x_str, ISO8601_STR_LEN, &x);
    strfiso8601(y_str, ISO8601_STR_LEN, &y);
    alloc_sprintf(&(state.assertions[state.index]), ":%d -> %s::sp_assert_equal_time_t(%s, %s)", lineno, state.function, x_str, y_str);

    state.codes = realloc(state.codes, (state.index + 1) * sizeof(int));

    if (x.tm_year == y.tm_year &&
        x.tm_mon == y.tm_mon &&
        x.tm_yday == y.tm_yday &&
        x.tm_wday == y.tm_wday &&
        x.tm_mday == y.tm_mday &&
        x.tm_hour == y.tm_hour &&
        x.tm_min == y.tm_min &&
        x.tm_sec == y.tm_sec
        ) {
        state.codes[state.index] = 0;
    } else {
        state.codes[state.index] = 1;
    }

    state.index++;
}
