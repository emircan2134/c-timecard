#pragma once

#include <time.h>

// this length includes the null byte
#define ISO8601_STR_LEN 26

/* format `tm` into an ISO8601 formatted date/time string, stored in `buf`. */
size_t strfiso8601(char *buf, size_t maxsize, struct tm *tptr);

/* parse `buf` as an ISO8601 formatted date/time string, stored in `tptr`. */
char *strpiso8601(char *buf, struct tm *tptr);
