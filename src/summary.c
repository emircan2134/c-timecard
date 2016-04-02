#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "summary.h"

/*-----------  Private Prototypes  -----------*/

t_summary *summary_new(void);
void summary_append(t_summary *list, t_summary *summary);
void summarize_entry(t_summary** summaries, t_entry *entry);
t_summary *summary_for_day(t_summary *summary, struct tm day);
t_project_hours *entry_for_project(t_summary *summary, const char *project);

/*-----------  Implementations  -----------*/

t_summary *summaries_build(t_entry_log *log) {
  t_summary *summaries = NULL;
  while(NULL != log && NULL != log->entry) {
    summarize_entry(&summaries, log->entry);
    log = log->next;
  }
  return summaries;
}

t_entry_hours summary_duration(t_summary *summary) {
  t_entry_hours d = 0.0;
  for (unsigned long idx = 0; idx < summary->count; idx++) {
    d += summary->entries[idx].hours;
  }
  return d;
}

t_entry_hours summary_duration_for_project(t_summary *summary, char *project) {
  for (unsigned long idx = 0; idx < summary->count; idx++) {
    if (0 == strcmp(project, summary->entries[idx].project)) {
      return summary->entries[idx].hours;
    }
  }
  return 0.0;
}

unsigned long summaries_all_projects(t_summary *summaries, char ***projects_ptr) {
  char **projects = NULL;
  unsigned long count = 0;
  t_summary *summary = summaries;

  while (NULL != summary) {
    for(unsigned long eidx = 0; eidx < summary->count; eidx++) {
      t_project_hours ph = summary->entries[eidx];
      bool found = false;
      for(unsigned long pidx = 0; pidx < count; pidx++) {
        if(0 == strcmp(projects[pidx], ph.project)) {
          found = true;
          break;
        }
      }
      if (!found) {
        count++;
        projects = realloc(projects, count * sizeof(char*));
        projects[count - 1] = ph.project;
      }
    }
    summary = summary->next;
  }

  *projects_ptr = projects;
  return count;
}


void summary_free(t_summary* summary) {
  if (NULL != summary) {
    for (unsigned long idx = 0; idx < summary->count; idx++) {
      free(summary->entries[idx].project);
    }
    free(summary->entries);
    t_summary *succ = summary->next;
    free(summary);
    if(NULL != succ) {
      summary_free(succ);
    }
  }
}

t_summary *summary_new(void) {
  t_summary *s = malloc(sizeof(t_summary));
  memset(s, 0, sizeof(t_summary));
  return s;
}

void summary_append(t_summary *list, t_summary *summary) {
  assert(NULL != list);
  t_summary *tail = list;
  while(NULL != tail->next) {
    tail = tail->next;
  }
  tail->next = summary;
}

void summarize_entry(t_summary** summaries, t_entry *entry) {
  struct tm day;
  localtime_r(entry->in, &day);

  t_summary *summary = summary_for_day(*summaries, day);
  if (NULL == summary) {
    summary = summary_new();
    summary->day = day;
    if (NULL == *summaries) {
      *summaries = summary;
    } else {
      summary_append(*summaries, summary);
    }
  }

  t_project_hours *p_entry = entry_for_project(summary, entry->project);
  if (NULL == p_entry) {
    summary->count++;
    summary->entries = realloc(summary->entries, summary->count * sizeof(t_project_hours));
    p_entry = summary->entries + summary->count - 1;
    p_entry->project = strdup(entry->project);
    p_entry->hours = 0.0;
  }

  p_entry->hours += entry_duration(entry);
}

t_summary *summary_for_day(t_summary *summary, struct tm day) {
  while(summary != NULL) {
    struct tm s_day = summary->day;
    if(s_day.tm_year == day.tm_year &&
        s_day.tm_mon == day.tm_mon &&
        s_day.tm_mday == day.tm_mday) {
      break;
    }
    summary = summary->next;
  }
  return summary;
}

t_project_hours *entry_for_project(t_summary *summary, const char *project) {
  for(unsigned long i = 0; i < summary->count; i++) {
    if (0 == strcmp(project, summary->entries[i].project)) {
      return summary->entries + i;
    }
  };
  return NULL;
}
