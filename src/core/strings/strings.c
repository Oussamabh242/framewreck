#include "strings.h"
#include <stdio.h>
#include <string.h>

SV sv(const char *data) {
  String_view s = {
      .data = data,
      .len = strlen(data),
  };
  return s;
}

// Takes String_view to return null terminator c string
char *sv_fmt_str(String_view sv) {
  char *ns = malloc(sv.len + 1);
  if (!ns)
    return NULL;
  int size = sprintf(ns, STR_FMT, (int)sv.len, sv.data);
  ns[size] = '\0';
  return ns;
}

// Substring
// Hello World!
// 012345678901
// 3, 7
//
SV sv_substr(SV *sv, size_t start, size_t end) {
  SV ret = {.len = 0, .data = NULL};
  size_t act_s = start, act_e = end;
  if (sv->len == 0)
    return ret;
  if (end >= sv->len)
    act_e = sv->len;
  ret.len = (act_e - act_s) + 1;
  ret.data = sv->data + act_s;
  return ret;
}

size_t sv_split(SV s, char delim, SV *list) {
  size_t start = 0;
  size_t count = 0;

  for (size_t i = 0; i <= s.len; i++) {
    if (i == s.len || s.data[i] == delim) {
      list[count].data = s.data + start;
      list[count].len = i - start;
      count++;
      start = i + 1;
    }
  }

  return count;
}
