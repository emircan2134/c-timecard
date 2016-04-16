#include <stdio.h>
#include "spec_helper.h"
#include "col_writer.h"

void spec_test_col_writer(void) {
  FILE *fh = tmpfile();
  char *lines[3][2] = {
    {"a", "baz"},
    {"foo", "bz"},
    {"b", "bb"},
  };

  t_col_writer w = {
    .io = fh,
    .padding = 2,
    .colcount = 2,
  };
  for (int i = 0; i < 3; i++) {
    cw_add_line(&w, lines[i]);
  }
  cw_flush(&w);

  fflush(fh);
  fseek(fh, 0, SEEK_SET);

  char *expected =
    "a    baz\n"
    "foo  bz\n"
    "b    bb\n";

  char *res = malloc((strlen(expected) + 1) * sizeof(char));
  res[strlen(expected)] = '\0';
  fread(res, sizeof(char), strlen(expected), fh);

  sp_assert_equal_str(expected, res);

  fclose(fh);
  free(res);
}
