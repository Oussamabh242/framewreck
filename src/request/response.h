#ifndef RESPONSE_H
#define RESPONSE_H

#include "../map/map.h"

typedef enum {
  OK = 200,
  CREATED = 201,
  ACCEPTED = 202,
  NO_CONTENT = 204,
  BAD_REQUEST = 400,
  UNAUTHORIZED = 401,
  FORBIDDEN = 403,
  NOT_FOUND = 404,
  INTERNAL_SERVER_ERROR = 500
} StatusCode;

typedef struct {
  char* http_version;
  StatusCode status_code;
  const char *status_message;
  Map *headers;
  char *body;
} Response;

Response *res(StatusCode code, const char *body);
void destroy_res(Response *r);
char* stringify_response(Response *r);

#endif // !RESPONSE_H
