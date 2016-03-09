#pragma once

#include <stdio.h>
#include "entry.h"

typedef struct entry_log_struct {
  struct entry_log_struct *prev, *next;
  t_entry *entry;
} t_entry_log;

/**
 * Parse a file into an entry log.
 * On error, return value is `NULL`, and `err` will be a string describing
 * The reason for failure. This function does not close `fh`.
 */
t_entry_log *log_parse(FILE *fh, char **err);

/**
 * Append an entry to a log, and return the new node.
 * This function will walk `log` forwards until it finds a node with no `next`
 * node.
 */
t_entry_log *log_append(t_entry_log *log, t_entry *entry);

/**
 * Free the memory used by a log. This will free all tail nodes
 * following the passed one, but will not walk backwards at all.
 */
void log_free(t_entry_log *log);
