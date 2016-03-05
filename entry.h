#pragma once

#include <time.h>

typedef struct {
  char *project;
  time_t *in, *out;
} t_entry;
typedef double t_entry_hours;

/* format entry */
size_t strfentry(char *buf, size_t maxsize, t_entry *entry);

/* parse entry */
char *strpentry(char *buf, t_entry *entry);

t_entry_hours entry_duration(t_entry *entry);

void entry_free(t_entry *entry);
