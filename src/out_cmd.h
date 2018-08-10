#pragma once

#include <stdio.h>

// entry point for `punch out`
int out_cmd(int argc);

// Append the current time to the file `fh` as an out entry.
// This function does check whether the file ends with a newline (and overwrites
// if it does), but does not check tha the file is in any other way valid for
// appending an out time to.
void write_out(FILE* fh);
