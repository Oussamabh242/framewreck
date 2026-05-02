#include "request.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <string.h>

Method makeMethod(const char *str) {
  if (strcmp(str, "GET") == 0)
    return GET;
  if (strcmp(str, "POST") == 0)
    return POST;
  if (strcmp(str, "PUT") == 0)
    return PUT;
  if (strcmp(str, "PATCH") == 0)
    return PATCH;
  if (strcmp(str, "DELETE") == 0)
    return DELETE;
  return UNKNOWN;
}

int find_index(const char *haystack, const char *needle) {
  if (!haystack || !needle)
    return -1;

  for (size_t i = 0; haystack[i] != '\0'; i++) {
    size_t j = 0;

    while (needle[j] != '\0' && haystack[i + j] == needle[j]) {
      j++;
    }

    if (needle[j] == '\0') {
      return (int)i;
    }
  }

  return -1;
}

Request req(String_view sv) {
  Request r = {0};
  r.headers = new_map();

  uint8_t count = 0;
  for (int i = 0; i < sv.len; i++) {
    if (sv.data[i] == '\n') {
      break;
    }
    count++;
  }

  char *first_line_data = malloc(count);
  if (first_line_data == NULL) {
    perror("Failed allocating firstline of requset");
  }

  SV first_line = {
      .len = count,
      .data = first_line_data,
  };
  strncpy(first_line.data, sv.data, count);

  SV subs[3] = {0};

  sv_split(first_line, ' ', subs);

  char *temp_s = sv_fmt_str(subs[0]);
  r.method = makeMethod(temp_s);
  char *path = malloc(subs[1].len + 1);
  if (!path) {
    perror("error allocating space for request.path");
  }
  strncpy(path, subs[1].data, subs[1].len);
  path[subs[1].len] = '\0';
  r.path = path;

  char *http_version = malloc(subs[2].len + 1);
  if (!http_version) {
    perror("error allocating space for request.http_version");
  }

  strncpy(http_version, subs[2].data, subs[2].len);
  http_version[subs[2].len] = '\0';
  r.http_version = http_version;

  free(temp_s);
  free(first_line.data);

  size_t start = find_index(sv.data, "\n") + 1;
  size_t end = find_index(sv.data, "\r\n\r\n");

  for (size_t i = start; i < end; i++) {
    if (sv.data[i] == '\n') {
      size_t line_len = i - start;
      char *line_data = malloc(line_len+1);

      if (line_data == NULL) {
        perror("Failed to allocat memory for line");
      }
      SV line = {.len = line_len, .data = line_data};

      strncpy(line.data, sv.data + start, line_len);
      line.data[line_len] = '\0';

      size_t end_k = 0;
      for (size_t j = 0; j < line.len; j++) {
        if (line.data[j] == ':') {
          end_k = j;
          break;
        }
      }
      // key
      char *key = malloc(end_k + 1);

      if (!key) {
        perror("error allocating space for req.header.key");
      }
      strncpy(key, line.data, end_k);
      key[end_k] = '\0';
      // val
      size_t end_v = line.len - (end_k + 2);
      char *val = malloc(end_v + 1);

      if (!val) {
        perror("error allocating space for req.header.val");
      }
      for (size_t j = end_k + 2; j < line.len; j++) {
        val[j - (end_k + 2)] = line.data[j];
      }
      val[end_v] = '\0';
      set(r.headers, key, val);

      start = i + 1;
    }
  }

  return r;
}

void destroy_req(Request *r) { destroy(r->headers); }
