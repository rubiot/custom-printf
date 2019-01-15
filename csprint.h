#pragma once

#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif

#include <stdio.h>

typedef int (*csfmt_handler) (FILE *stream, size_t size, void *data);

int csvfprintf(FILE *s, const char *fmt, va_list ap);
int csfprintf(FILE *s, const char *fmt, ...);
int cssprintf(char *str, const char *fmt, ...);
int csprintf(const char *fmt, ...);
int csprint_register_fmt(const char *fmt, csfmt_handler handler);
