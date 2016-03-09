#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "cmd_util.h"
#include "help_cmd.h"

bool show_cmd_help(char *cmd, int argc, char **argv) {
  if (argc > 0 && (0 == strcmp(argv[0], "-h") || 0 == strcmp(argv[0], "--help"))) {
    help_cmd(1, &cmd);
    return true;
  }
  return false;
}

void get_data_dir(char *buf) {
  snprintf(buf, FILENAME_MAX, "%s/.punch", getenv("HOME"));
}

void get_log_file_path(char *buf) {
  char dir[FILENAME_MAX];
  get_data_dir(dir);
  snprintf(buf, FILENAME_MAX, "%s/entries.log", dir);
}

bool ensure_data_dir_exists(void) {
  char path[FILENAME_MAX];
  get_data_dir(path);
  struct stat finfo;
  int success = stat(path, &finfo);
  if (-1 == success) {
    if (ENOENT == errno) {
      success = mkdir(path, S_IFDIR | S_IRWXU | S_IRGRP);
      if (-1 == success) {
        goto fail;
      }
    } else {
      goto fail;
    }
  }
  return true;

fail:
  fprintf(stderr, "Error %d (%s) trying to create %s\n", errno, edesc(errno), path);
  return false;
}

FILE *open_log_file(void) {
  if (!ensure_data_dir_exists()) {
    return NULL;
  }
  char path[FILENAME_MAX];
  get_log_file_path(path);
  struct stat finfo;
  int success = stat(path, &finfo);
  if (-1 == success) {
    if (ENOENT == errno) {
      success = open(path, O_WRONLY | O_CREAT);
      if (-1 == success) {
        goto fail;
      } else {
        close(success);
      }
    } else {
      goto fail;
    }
  }
  FILE *fh = fopen(path, "r+");
  if (NULL == fh) {
    goto fail;
  }
  return fh;

fail:
    fprintf(stderr, "Error %d (%s) opening %s\n", errno, edesc(errno), path);
    return NULL;
}

int with_open_log_file(void *ctx, int (*worker)(FILE *, void *)) {
  FILE *fh = NULL;

  fh = open_log_file();
  if (NULL == fh) {
    return -1;
  }

  int rv = worker(fh, ctx);

  fclose(fh);
  return rv;
}

int with_parsed_log(FILE *fh, void *ctx, int (*worker)(t_entry_log*, void *)) {
  t_entry_log *log = NULL;
  char *err = NULL;
  log = log_parse(fh, &err);
  if (NULL == log && NULL != err) {
    fprintf(stderr, "%s\n", err);
    return -1;
  }

  int rv = worker(log, ctx);

  if (NULL != log) {
    log_free(log);
  }
  return rv;
}

char *edesc(int err) {
  switch(err) {
    case ENOENT:
      return "file does not exist";
    case ENOTDIR:
      return "is not a directory";
    case EACCES:
      return "access denied";
    case EBADF:
      return "bad file descriptor";
    case EEXIST:
      return "the file exists";
    case ENAMETOOLONG:
      return "filename too long";
    case EROFS:
      return "write access requested on a read-only filesystem";
    case EINVAL:
      return "flag is invalid";
    default:
      return "unknown error";
  }
}
