#pragma once

#include "entry_log.h"

typedef struct {
  char *project;
  t_entry_hours hours;
} t_project_hours;

typedef struct summary_struct {
  struct summary_struct *next;
  struct tm day;
  t_project_hours *entries;
  unsigned long count;
} t_summary;

/**
 * Build a summary list from an entry log.
 * The return value is allocated, and the caller must free it with `summary_free`.
 */
t_summary *summaries_build(t_entry_log *log);

/**
 * Calculate the total hours for a single day.
 */
t_entry_hours summary_duration(t_summary *summary);

/**
 * Find the entry for a given project and return the duration, or return 0 if
 * the summary has no entry for the given project
 */
t_entry_hours summary_duration_for_project(t_summary *summary, char *project);

/**
 * Collect all project names from a list of summaries.
 * Returns the number of entries written into `projects`.
 * `projects` is suitable to be `free`ed by the caller.
 * For efficiency, individual project names share memory with the passed in
 * `summaries`: you must free your `projects` list before freeing the
 * `summaries`.
 */
unsigned long summaries_all_projects(t_summary *summaries, char ***projects_ptr);

/**
 * Free all the memory used by a summary.
 */
void summary_free(t_summary* summary);
