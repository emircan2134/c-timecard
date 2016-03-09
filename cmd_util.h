#pragma once

#include <stdbool.h>
#include "entry_log.h"

/**
 * if `argv[0]` is '-h' or '--help', print the specific help for `cmd` and
 * return `true`, otherwise return `false`.
 */
bool show_cmd_help(char *cmd, int argc, char **argv);

/**
 * Resolve punch's storage directory (~/.punch), and print the full path into
 * `buf`.
 */
void get_data_dir(char *buf);

/**
 * Resolve punch's default log file name (~/.punch/entries.log), and print the
 * full path into `buf`.
 */
void get_log_file_path(char *buf);

/**
 * If ~/.punch does not exist, attempt to create it.
 * Returns true if the directory already exists or
 * was created succesfully. Returns false if an error occurred.
 * Errors will be printed to stderr.
 */
bool ensure_data_dir_exists(void);

/**
 * Open ~/.punch/entries.log for reading & writing.  If the file does not
 * exist, it will be created.  If an error occurs, the eror will be printed to
 * stderr & `NULL` will be returned.
 */
FILE *open_log_file(void);

/**
 * Wrap the boilerplate of opening/closing the log file: call `worker` with a
 * `fh` pointing at the open log file (if no errors occur opening it), as well
 * as the given `ctx`. `fh` will be closed after running `worker`.
 * If an error occurs, the return value is -1. Otherwise, it is the return
 * value of `worker`.
 */
int with_open_log_file(void *ctx, int (*worker)(FILE *, void *));

/**
 * Wrap the boilerplate of parsing & freeing an entry log: similar
 * semantice as with_open_log_file, above.
 */
int with_parsed_log(FILE *fh, void *ctx, int (*worker)(t_entry_log*, void *));

/**
 * Return a human-readable description of a standard error code.
 */
char *edesc(int err);
