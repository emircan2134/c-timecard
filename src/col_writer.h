#pragma once

#include <stdio.h>

typedef char** t_cw_line;
typedef struct {
  FILE *io;
  int padding;
  int colcount;
  // private members
  t_cw_line *_lines;
  int _linecount;
  int *_colwidths;
} t_col_writer; // all fields must be 0 at init

/**
 * Buffer a line.
 * `line` must have `colcount` entries in it, and each entry must be a
 * null-terminated string.
 */
void cw_add_line(t_col_writer *writer, t_cw_line line);

/**
 * Flush buffered output to `io`.
 */
void cw_flush(t_col_writer *writer);
