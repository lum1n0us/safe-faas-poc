#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static char *selfie = "this is .wasm";

__attribute__((export_name("hi"))) uint32_t hi(uint32_t buf, uint32_t len) {
  if (len < strlen(selfie))
    return 1;

  memcpy((void *)buf, (void *)selfie, strlen(selfie));
  return 0;
}
