#pragma once

#include <time.h>

#define MAX_ENTRY_LINE_LEN 255

typedef struct {
  char *project;
  time_t *in, *out;
} t_entry;
typedef double t_entry_hours;

/**
 * Format an entry as it appears in the log file.
 * A punched-in entry will be formatted as `"PROJECT\\tTIME_IN"`.
 * A punched-out entry will be formatted as `"PROJECT\\tTIME_IN\\tTIME_OUT\\n"`.
 *
 * Similar to `strftime`, the return value will be the number of bytes written,
 * or `NULL` if an error occurred.
 */
size_t strfentry(char *buf, size_t maxsize, t_entry *entry);

/**
 * Parse a log line (formatted according to `strfentry, above) into an entry.
 * The return value, similar to `strptime`, points to the next position in
 * `buf` not used for parsing, or `NULL` if an error occurred.
 */
char *strpentry(char *buf, t_entry *entry);

/**
 * Determine how many hours an entry was punched in for.
 * Currently punched-in entries are considered to have a duration of 0.
 * Invalid entries may return a negative value.
 */
t_entry_hours entry_duration(t_entry *entry);

/**
 * Free the memory used by an entry & its fields.
 */
void entry_free(t_entry *entry);

/**
 * Deep-duplicate an entry & its values into a new location.
 * Calling code is responsible for calling `entry_free`.
 */
t_entry *entry_dup(t_entry *entry);
