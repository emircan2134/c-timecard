#include <speck.h>
#include <time.h>

#define SET_TZ(tz) char *_old_tz = getenv("TZ"); setenv("TZ", tz, 1);
#define RESET_TZ if (NULL == _old_tz) { unsetenv("TZ"); } else { setenv("TZ", _old_tz, 1); }

struct tm fixture_time(void) {
  struct tm t = {
    .tm_sec = 30,
    .tm_min = 5,
    .tm_hour = 14,
    .tm_mday = 4,
    .tm_wday = 4,
    .tm_mon = 2,
    .tm_yday = 62,
    .tm_year = 116, // years since 1900. I know, right?
    .tm_isdst = 0
  };
  return t;
}

char *heap_str(const char *str) {
  int l = strlen(str);
  char *dst = malloc((l + 1) * sizeof(char));
  strcpy(dst, str);
  return dst;
}

