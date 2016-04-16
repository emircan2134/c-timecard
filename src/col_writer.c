#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "col_writer.h"

/*-----------  Private Prototypes  -----------*/
t_cw_line dup_line(t_col_writer *writer, t_cw_line line);
void free_lines(t_col_writer *writer);
void flush_line(t_col_writer *writer, t_cw_line line);

/*-----------  Implementations  -----------*/
void cw_add_line(t_col_writer *writer, t_cw_line line) {
  // add line data to internal line buffer
  writer->_linecount++;
  writer->_lines = realloc(writer->_lines, writer->_linecount * sizeof(t_cw_line));
  assert(NULL != writer->_lines);
  writer->_lines[writer->_linecount - 1] = dup_line(writer, line);

  // initialize _colwidths if it's empty
  if (NULL == writer->_colwidths) {
    writer->_colwidths = malloc(writer->colcount * sizeof(int));
    memset(writer->_colwidths, 0, writer->colcount * sizeof(int));
  }

  // update _colwidths entries
  for (int i = 0; i < writer->colcount; i++) {
    if (i < (writer->colcount - 1) &&
        (int)strlen(line[i]) + writer->padding > writer->_colwidths[i]) {
      writer->_colwidths[i] = (int)strlen(line[i]) + writer->padding;
    }
  }
}

void cw_flush(t_col_writer *writer) {
  for (int i = 0; i < writer->_linecount; i++) {
    flush_line(writer, writer->_lines[i]);
  }

  free_lines(writer);
  free(writer->_colwidths);
  writer->_colwidths = NULL;
}

void flush_line(t_col_writer *writer, t_cw_line line) {
  for (int col = 0; col < writer->colcount; col++) {
    assert(EOF != fputs(line[col], writer->io));
    for (int padcount = writer->_colwidths[col] - (int)strlen(line[col]);
         padcount > 0; padcount--) {
      assert(EOF != fputc(' ', writer->io));
    }
  }
  assert(EOF != fputc('\n', writer->io));
}

t_cw_line dup_line(t_col_writer *writer, t_cw_line line) {
  t_cw_line dup = malloc(writer->colcount * sizeof(char *));
  assert(NULL != dup);
  for (int i = 0; i < writer->colcount; i++) {
    dup[i] = strdup(line[i]);
  }
  return dup;
}

void free_lines(t_col_writer *writer) {
  for (int lidx = 0; lidx < writer->_linecount; lidx++) {
    for (int cidx = 0; cidx < writer->colcount; cidx++) {
      free(writer->_lines[lidx][cidx]);
    }
    free(writer->_lines[lidx]);
  }
  free(writer->_lines);
  writer->_lines = NULL;
  writer->_linecount = 0;
}
