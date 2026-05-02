#ifndef REQUEST_H
#define REQUEST_H

#include "../strings/strings.h"
#include "../map/map.h"

typedef enum { POST, GET, PATCH, PUT, DELETE, UNKNOWN } Method;


typedef struct {
  Method method;
  const char *path;
  char *http_version;
  Map *headers;
} Request;

Request req(String_view sv);
void destroy_req(Request *r);

#endif // !REQUEST_H
