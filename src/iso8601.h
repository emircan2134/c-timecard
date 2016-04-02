#pragma once

#include <time.h>

// this length includes the null byte
#define ISO8601_STR_LEN 26

/**
 * Format `tm` into an ISO8601 formatted date/time string, stored in `buf`.
 * Function behavior is similar to `strftime`: return value will be the number
 * of bytes written, or `NULL` if an error occurred.
 */
size_t strfiso8601(char *buf, size_t maxsize, struct tm *tptr);

/**
 * Parse `buf` as an ISO8601 formatted date/time string, stored in `tptr`.
 * The return value is similar to `strptime`: a pointer to the next unused
 * byte in `buf` on success, or `NULL` if an error occurred.
 */
char *strpiso8601(char *buf, struct tm *tptr);
