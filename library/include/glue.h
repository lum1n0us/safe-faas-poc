#ifndef _GLUE_H_
#define _GLUE_H_

int echo_from_glue(const char *name, char *out);

int echo_from_wamr(const char *name, char *out);

int echo_from_wasm(const char *name, char *out);

#endif /* _GLUE_H_ */
