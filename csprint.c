#include "csprint.h"
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <stdlib.h>

struct csfmt_spec
{
  char *fmt;
  csfmt_handler handler;
};

static int ip_printer(FILE *stream, size_t size, void *data)
{
  u_int32_t ip = * (u_int32_t *) data;
  return fprintf(stream, "%d.%d.%d.%d", (ip >> 0x00) & 0xff,
                                        (ip >> 0x08) & 0xff,
                                        (ip >> 0x10) & 0xff,
                                        (ip >> 0x18) & 0xff);
}

/*
 * Tipos tratados pelo modulo
 */
struct csfmt_spec fmt_map[] = {
  {"ip", ip_printer},
};

int csprint_register_fmt(const char *fmt, csfmt_handler handler)
{
  return 0;
}

/*
 * Determina o tamanho da mascara comecada em \a fmt.
 * Baseado na especificado do 'man printf'
 */
static int fmt_calc_len(const char *fmt)
{
  const char *p = fmt + 1;

  // flags
  p += strspn(p, "#0- +'I");
  // field width
  p += strspn(p, "0123456789");
  // precision
  p += strspn(p, ".0123456789");
  // length modifier
  p += strspn(p, "hlqLjzZt");
  // conversion specifier
  p += strspn(p, "diouxXeEfFgGaAcsCSpnm%");

  return p - fmt;
}

static struct csfmt_spec *fmt_find(const char *fmt)
{
  int i;

  for (i = 0; i < sizeof(fmt_map) / sizeof(*fmt_map); i++)
    if (!strcmp(fmt, fmt_map[i].fmt))
      return &fmt_map[i];

  return NULL;
}

int csvfprintf(FILE *s, const char *fmt, va_list ap)
{
  int ret = 0;
  const char *p = fmt;
  size_t fmtlen;

  while (*p)
    switch (*p) {
      case '%': {
        fmtlen = fmt_calc_len(p);
        ret += vfprintf(s, strndupa(p, fmtlen), ap);
        p += fmtlen;
        break;
      }
      case '$':
        if (p[1] == '{') {
          p += 2; // saltando o ${
          fmtlen = strcspn(p, "}");
          struct csfmt_spec *spec = fmt_find(strndupa(p, fmtlen));
          if (spec)
            ret += spec->handler(s, 0, va_arg(ap, void *));
          p += fmtlen + 1;
          break;
        }
        // fallthrough
      default:
        ret += fputc(*p++, s);
        break;
    }

  return ret;
}

int csfprintf(FILE *s, const char *fmt, ...)
{
  int ret;
  va_list ap;

  va_start(ap, fmt);
  ret = csvfprintf(s, fmt, ap);
  va_end(ap);

  return ret;
}

int csprintf(const char *fmt, ...)
{
  int ret;
  va_list ap;

  va_start(ap, fmt);
  ret = csvfprintf(stdout, fmt, ap);
  va_end(ap);

  return ret;
}

int cssprintf(char *str, const char *fmt, ...)
{
  int ret;
  va_list ap;
  char *buffer;
  size_t size;
  FILE *stream = open_memstream(&buffer, &size);

  va_start(ap, fmt);
  ret = csvfprintf(stream, fmt, ap);
  va_end(ap);

  fclose(stream);
  strcpy(str, buffer);
  free(buffer);

  return ret;
}

