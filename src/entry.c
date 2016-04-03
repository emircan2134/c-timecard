#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "entry.h"
#include "iso8601.h"

/*-----------  Private Prototypes  -----------*/
bool entry_valid(t_entry *entry);
size_t strfentry_time(char *buf, time_t *t);

/*-----------  Implementations  -----------*/
size_t strfentry(char *buf, size_t maxsize, t_entry *entry) {
  if (!entry_valid(entry)) {
    return 0;
  }
  if (NULL == strncpy(buf, entry->project, maxsize)) {
    return 0;
  }
  size_t tot_written = strnlen(entry->project, maxsize);
  if (NULL == strncpy(buf + tot_written, "\t", 1)) {
    return 0;
  }
  tot_written += 1;
  char tstr[ISO8601_STR_LEN];
  if (0 == strfentry_time(tstr, entry->in)) {
    return 0;
  }
  if (NULL == strncpy(buf + tot_written, tstr, strnlen(tstr, ISO8601_STR_LEN))) {
    return 0;
  }
  tot_written += strnlen(tstr, ISO8601_STR_LEN);
  if (NULL != entry->out) {
    if (NULL == strncpy(buf + tot_written, "\t", 1)) {
      return 0;
    }
    tot_written += 1;
    if (0 == strfentry_time(tstr, entry->out)) {
      return 0;
    }
    if (NULL == strncpy(buf + tot_written, tstr, strnlen(tstr, ISO8601_STR_LEN))) {
      return 0;
    }
    tot_written += strnlen(tstr, ISO8601_STR_LEN);
    if (NULL == strncpy(buf + tot_written, "\n", 1)) {
      return 0;
    }
    tot_written += 1;
  }
  if (NULL == strncpy(buf + tot_written, "\0", 1)) {
    return 0;
  }
  tot_written += 1;
  return tot_written;
}

char *strpentry(char *buf, t_entry *entry) {
  char *piece, *strtok_memo;
  struct tm tptr;
  time_t t;

  piece = strtok_r(buf, "\t", &strtok_memo);
  if (NULL == piece) {
    return NULL;
  }
  entry->project = strndup(piece, strnlen(piece, MAX_ENTRY_LINE_LEN - 1));
  piece = strtok_r(NULL, "\t\n", &strtok_memo);
  if (NULL == piece || NULL == strpiso8601(piece, &tptr)) {
    return NULL;
  }
  t = mktime(&tptr);
  entry->in = memcpy(malloc(sizeof(time_t)), &t, sizeof(time_t));
  piece = strtok_r(NULL, "\n", &strtok_memo);
  if (NULL != piece) {
    if (NULL == strpiso8601(piece, &tptr)) {
      return NULL;
    }
    t = mktime(&tptr);
    entry->out = memcpy(malloc(sizeof(time_t)), &t, sizeof(time_t));
  } else {
    entry->out = NULL;
  }
  // memo is NULL for partial entries: strtok_r runs past the null byte.
  // NULL is an error return code, so we back up the pointer one, which
  // isn't strictly in adherence with similar stdlib fns, but works
  // in our case.
  if (NULL == strtok_memo) {
    strtok_memo--;
  }
  return strtok_memo;
}

t_entry_hours entry_duration(t_entry *entry) {
  if (!entry_valid(entry)) {
    return -1;
  } else if (NULL == entry->out) {
    return 0;
  } else {
    return difftime(*entry->out, *entry->in) / (60.0 * 60.0);
  }
}

void entry_free(t_entry *entry) {
  if (NULL != entry) {
    if (NULL != entry->project) {
      free(entry->project);
    }
    if (NULL != entry->in) {
      free(entry->in);
    }
    if (NULL != entry->out) {
      free(entry->out);
    }
    free(entry);
  }
}

t_entry *entry_dup(t_entry *entry) {
  t_entry *dup = malloc(sizeof(t_entry));
  dup->project = strndup(entry->project, strnlen(entry->project, MAX_ENTRY_LINE_LEN));
  dup->in = malloc(sizeof(time_t));
  memcpy(dup->in, entry->in, sizeof(time_t));
  if (NULL != entry->out) {
    dup->out = malloc(sizeof(time_t));
    memcpy(dup->out, entry->out, sizeof(time_t));
  } else {
    dup->out = NULL;
  }
  return dup;
}

size_t strfentry_time(char *buf, time_t *t) {
  struct tm tt;
  localtime_r(t, &tt);
  size_t foo = strfiso8601(buf, ISO8601_STR_LEN, &tt);
  return foo;
}

bool entry_valid(t_entry *entry) {
  return (NULL != entry->project &&
          NULL != entry->in &&
          strnlen(entry->project, MAX_ENTRY_LINE_LEN) < MAX_ENTRY_LINE_LEN);
}
