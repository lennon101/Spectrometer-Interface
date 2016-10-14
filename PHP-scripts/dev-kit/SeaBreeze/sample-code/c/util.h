#ifndef SEABREEZE_TEST_UTIL_H
#define SEABREEZE_TEST_UTIL_H

const char* get_error_string(int error);
int  check_error(int index, int *error, const char *func);
int  get_external_trigger_mode(const char *type);
void graph(const char *type, const char *label, double *spectrum, int pixels);
void logger(const char *fmt, ...);
void logger_header(const char *fmt, ...);

#endif
