#include "response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *get_status_message(StatusCode code) {
  switch (code) {
  case OK:
    return "OK";
  case CREATED:
    return "Created";
  case ACCEPTED:
    return "Accepted";
  case NO_CONTENT:
    return "No Content";
  case BAD_REQUEST:
    return "Bad Request";
  case UNAUTHORIZED:
    return "Unauthorized";
  case FORBIDDEN:
    return "Forbidden";
  case NOT_FOUND:
    return "Not Found";
  case INTERNAL_SERVER_ERROR:
    return "Internal Server Error";
  default:
    return "Unknown Status Code";
  }
}

Response *res(StatusCode code, const char *body) {
  Response *r = malloc(sizeof(Response));
  r->http_version = "HTTP/1.1";
  r->status_code = code;
  r->status_message = get_status_message(code);
  r->headers = new_map();
  r->body = (char *)body;

  char content_length[32];
  sprintf(content_length, "%d", (int)strlen(body));
  set(r->headers, "Content-Length", content_length);
  set(r->headers, "Content-Type", "text/html");
  return r;
}

char *stringify_response(Response *r) {
  size_t buffer_size = 1024;
  char *buffer = malloc(buffer_size);

  if (!buffer)
    return NULL;

  size_t len = 0;

  len += snprintf(buffer + len, buffer_size - len, "%s %d %s\r\n",
                  r->http_version, r->status_code, r->status_message);

  for (size_t i = 0; i < r->headers->len; i++) {

    Entry e = r->headers->entries[i];

    len +=
        snprintf(buffer + len, buffer_size - len, "%s: %s\r\n", e.key, e.value);

    if (buffer_size - len < 256) {
      buffer_size *= 2;
      buffer = realloc(buffer, buffer_size);
    }
  }
  len += snprintf(buffer + len, buffer_size - len, "\r\n");
  if (r->body) {
    len += snprintf(buffer + len, buffer_size - len, "%s", r->body);
  }

  return buffer;
}

void destroy_res(Response *r) {
  free(r->headers);
  // free(r->body);
}
