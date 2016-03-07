#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entry_log.h"

/*-----------  Private Prototypes  -----------*/
t_entry *blank_entry(void);
void reset_entry(t_entry *entry);

/*-----------  Implementations  -----------*/
t_entry_log *log_parse(FILE *fh, char **err) {
  char line[MAX_ENTRY_LINE_LEN];
  t_entry *entry = blank_entry();
  t_entry_log *log_head = NULL, *log_tail = NULL;
  while(NULL != fgets(line, MAX_ENTRY_LINE_LEN, fh)) {
    if (NULL == strpentry(line, entry)) {
      if (NULL != err) {
        //TODO: this err is leaked right now -- it's going to exit shortly,
        //so not the worst, but not great.
        *err = malloc((100 + MAX_ENTRY_LINE_LEN) * sizeof(char));
        snprintf(*err, 100 + MAX_ENTRY_LINE_LEN, "Error parsing log line: \"%s\"", line);
      }
      goto fail;
    } else {
      log_tail = log_append(log_tail, entry);
      if (NULL == log_head) {
        log_head = log_tail;
      }
    }
  }

  if (0 != ferror(fh)) {
    if (NULL != err) {
      *err = "A file error occurred.";
    }
    clearerr(fh);
    goto fail;
  }

  return log_head;

fail:
  entry_free(entry);
  if (NULL != log_head) {
    log_free(log_head);
  }
  return NULL;
}

t_entry_log *log_append(t_entry_log *log, t_entry *entry) {
  t_entry_log *succ = malloc(sizeof(t_entry_log));
  t_entry_log *tail = log;
  if (NULL != log) {
    while(NULL != tail->next) {
      tail = tail->next;
    }
    tail->next = succ;
  }
  succ->entry = entry_dup(entry);
  succ->prev = tail;
  succ->next = NULL;
  return succ;
}

void log_free(t_entry_log *log) {
  assert(NULL != log);
  t_entry_log *succ = log->next;
  entry_free(log->entry);
  free(log);
  if (NULL != succ) {
    succ->prev = NULL;
    log_free(succ);
  }
}

t_entry *blank_entry(void) {
  t_entry *entry = malloc(sizeof(t_entry));
  memset(entry, 0, sizeof(t_entry));
  return entry;
}

void reset_entry(t_entry *entry) {
  if(NULL != entry->project) {
    free(entry->project);
    entry->project = NULL;
  }
  if(NULL != entry->in) {
    free(entry->in);
    entry->in = NULL;
  }
  if(NULL != entry->out) {
    free(entry->out);
    entry->out = NULL;
  }
}
