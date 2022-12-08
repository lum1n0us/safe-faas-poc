#include "glue.h"
#include <stdio.h>

int echo_from_glue(const char *name, char *out) {
  return sprintf(out, "Hi %s, this is glue", name);
}