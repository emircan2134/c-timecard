#include <string.h>
#include "iso8601.h"

#define BASE_FORMAT_NO_TZ "%Y-%m-%dT%T"
#define PARSE_FORMAT "%Y-%m-%dT%T%z"
#define BASE_LEN 20
#define TZ_LEN 7

/*-----------  Private Prototypes  -----------*/
size_t strfiso8601tz(char *buf, size_t maxsize, struct tm *tptr);

/*-----------  Implementations  -----------*/
size_t strfiso8601(char *buf, size_t maxsize, struct tm *tptr) {
  if (maxsize < ISO8601_STR_LEN) {
    return 0;
  }
  size_t base_len_written = strftime(buf, BASE_LEN, BASE_FORMAT_NO_TZ, tptr);
  if (0 == base_len_written) {
    return 0;
  }
  size_t tz_len_written = strfiso8601tz(buf + BASE_LEN - 1, TZ_LEN, tptr);
  if (0 == tz_len_written) {
    return 0;
  }
  return strnlen(buf, maxsize);
}

// NOTE: currently does not support "Z" for timezone
char *strpiso8601(char *buf, struct tm *tptr) {
  char mbuf[ISO8601_STR_LEN];
  strncpy(mbuf, buf, strnlen(buf, ISO8601_STR_LEN));
  if (':' != mbuf[strnlen(buf, ISO8601_STR_LEN) - 3]) {
    return NULL;
  }
  for (size_t i = 3; i > 1; i--) {
    size_t target = strnlen(buf, ISO8601_STR_LEN) - i;
    mbuf[target] = mbuf[target + 1];
  }
  mbuf[strnlen(buf, ISO8601_STR_LEN) - 1] = '\0';
  return strptime(mbuf, PARSE_FORMAT, tptr);
}

size_t strfiso8601tz(char *buf, size_t maxsize, struct tm *tptr) {
  size_t written = strftime(buf, maxsize, "%z", tptr);
  if (0 == written || maxsize < TZ_LEN) {
    return 0;
  }
  // add the ":" into the str
  char tz_min[2];
  strncpy(tz_min, buf + 3, 2);
  buf[3] = ':';
  strncpy(buf + 4, tz_min, 2);
  buf[6] = '\0';
  return strnlen(buf, TZ_LEN);
}
