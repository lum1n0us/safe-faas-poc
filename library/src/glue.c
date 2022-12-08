#include "glue.h"
#include "wasm_export.h"
#include <stdio.h>

int echo_from_glue(const char *name, char *out) {
  return sprintf(out, "Hi %s, this is glue", name);
}

int echo_from_wamr(const char *name, char *out) {
  unsigned wamr_version[3] = {0};

  wasm_runtime_get_version(&wamr_version[0], &wamr_version[1],
                           &wamr_version[2]);
  return sprintf(out, "Hi %s, this is WAMR %u.%u.%u", name, wamr_version[0],
                  wamr_version[1], wamr_version[2]);
}