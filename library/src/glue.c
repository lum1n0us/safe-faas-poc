#include "glue.h"
#include "wasm_c_api.h"
#include "wasm_export.h"
#include <stdio.h>
#include <stdlib.h>

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

typedef struct cstring {
  uint32_t pos;
  uint32_t len;
} cstring;

static bool load_wasm_file_content(const char *file_name,
                                   wasm_byte_vec_t *out) {
  bool ret = false;
  FILE *file = fopen(file_name, "rb");
  if (!file) {
    printf("> Error loading .wasm!\n");
    goto quit;
  }

  int offset = fseek(file, 0L, SEEK_END);
  if (offset == -1) {
    printf("> Error loading .wasm!\n");
    goto close_file;
  }

  long file_size = ftell(file);
  if (file_size == -1) {
    printf("> Error loading .wasm!\n");
    goto close_file;
  }

  offset = fseek(file, 0L, SEEK_SET);
  if (offset == -1) {
    printf("> Error loading .wasm!\n");
    goto close_file;
  }

  wasm_byte_vec_new_uninitialized(out, file_size);
  if (fread(out->data, file_size, 1, file) != 1) {
    printf("> Error loading content!\n");
    goto close_file;
  }

  ret = true;
close_file:
  fclose(file);
quit:
  return ret;
}

int echo_from_wasm(const char *name, char *out) {
  int ret = 0;

  printf("Initializing...\n");
  wasm_engine_t *engine = wasm_engine_new();
  if (!engine)
    goto quit;

  wasm_store_t *store = wasm_store_new(engine);
  if (!store)
    goto release_engine;

  printf("Loading binary...\n");
  wasm_byte_vec_t binary = {0};
  // TBD: replace with go_faas.wasm
  const char *file_name = "c_faas.wasm";
  if (!load_wasm_file_content(file_name, &binary))
    goto release_store;

  // Compile.
  printf("Compiling module...\n");
  wasm_module_t *module = wasm_module_new(store, &binary);
  wasm_byte_vec_delete(&binary);
  if (!module) {
    printf("> Error compiling module!\n");
    goto release_store;
  }

  // Instantiate.
  printf("Instantiating module...\n");
  wasm_extern_vec_t imports = WASM_EMPTY_VEC;
  wasm_instance_t *instance = wasm_instance_new(store, module, &imports, NULL);
  if (!instance) {
    printf("> Error instantiating module!\n");
    goto release_module;
  }

  // Extract export.
  printf("Extracting export...\n");
  wasm_extern_vec_t exports;
  wasm_instance_exports(instance, &exports);
  if (exports.size == 0) {
    printf("> Error accessing exports!\n");
    goto release_instance;
  }

  wasm_memory_t *wasm_memory = wasm_extern_as_memory(exports.data[0]);
  if (wasm_memory == NULL) {
    printf("> Error accessing export memory!\n");
    goto release_instance;
  }

  const wasm_func_t *malloc_func = wasm_extern_as_func(exports.data[3]);
  if (malloc_func == NULL) {
    printf("> Error accessing export malloc!\n");
    goto release_instance;
  }

  const wasm_func_t *free_func = wasm_extern_as_func(exports.data[4]);
  if (free_func == NULL) {
    printf("> Error accessing export free!\n");
    goto release_instance;
  }

  const wasm_func_t *hi_func = wasm_extern_as_func(exports.data[2]);
  if (hi_func == NULL) {
    printf("> Error accessing export hi!\n");
    goto release_instance;
  }

  // Call.
  printf("Calling export...\n");

  // buf = malloc(16) in .wasm
  unsigned buf_pos = 0;
  {
    wasm_val_t as[] = {WASM_I32_VAL(16)};
    wasm_val_t rs[] = {WASM_INIT_VAL};
    wasm_val_vec_t args = WASM_ARRAY_VEC(as);
    wasm_val_vec_t results = WASM_ARRAY_VEC(rs);
    wasm_trap_t *trap = wasm_func_call(malloc_func, &args, &results);
    if (trap) {
      printf("> Error calling malloc!\n");
      wasm_trap_delete(trap);
      goto release_exports;
    }

    buf_pos = results.data[0].of.i32;
    printf("> malloc(16) -> %u\n", buf_pos);
  }

  // hi(buf, 16) in .wasm
  {
    wasm_val_t as[] = {WASM_I32_VAL(buf_pos), WASM_I32_VAL(16)};
    wasm_val_t rs[] = {WASM_INIT_VAL};
    wasm_val_vec_t args = WASM_ARRAY_VEC(as);
    wasm_val_vec_t results = WASM_ARRAY_VEC(rs);
    wasm_trap_t *trap = wasm_func_call(hi_func, &args, &results);
    if (trap) {
      printf("> Error calling hi!\n");
      wasm_trap_delete(trap);
      goto release_exports;
    }

    unsigned hi_ret = results.data[0].of.i32;
    if (hi_ret != 0) {
      printf("> Error hi returns!\n");
      goto release_exports;
    }

    byte_t *data = wasm_memory_data(wasm_memory);
    char *hi_ret_cstr = (char*)(data + buf_pos);
    printf("> hi(buf, 16) -> %s\n", hi_ret_cstr);
    ret = sprintf(out, "Hi %s, %s", name, hi_ret_cstr);
  }

  // free(buf) in .wasm
  {
    wasm_val_t as[] = {WASM_I32_VAL(buf_pos)};
    wasm_val_vec_t args = WASM_ARRAY_VEC(as);
    wasm_val_vec_t results = WASM_EMPTY_VEC;
    wasm_trap_t *trap = wasm_func_call(free_func, &args, &results);
    if (trap) {
      printf("> Error calling hi!\n");
      wasm_trap_delete(trap);
      ret = 0;
      goto release_exports;
    }
  }

release_exports:
  wasm_extern_vec_delete(&exports);
release_instance:
  wasm_instance_delete(instance);
release_module:
  wasm_module_delete(module);
release_store:
  wasm_store_delete(store);
release_engine:
  wasm_engine_delete(engine);
quit:
  return ret;
}

#ifdef STANDALONE
int main() {
  char out[128] = {0};
  int ret = echo_from_wasm("Tester", out);

  if (!ret)
    return EXIT_FAILURE;

  printf("%s\n", out);
  return EXIT_SUCCESS;
}
#endif