#include "router.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef struct {
  int match;
  Entry *paths;
  int paths_len;
} RouteMatchResult;

Router *new_router() {
  Router *r = malloc(sizeof(Router));
  r->routes = malloc(sizeof(Route) * 4);
  r->cap = 4;
  r->len = 0;
  return r;
}

Route *new_route(char *path, HandlerFunc *f, void *args) {
  Route *route = malloc(sizeof(Route));
  route->path = path;
  route->f = f;
  route->args = args;
  return route;
}

void register_route(Router *router, char *path, HandlerFunc f, void *args) {
  if (router->len == router->cap) {
    router->cap *= 2;
    router->routes = realloc(router->routes, router->cap);
  }
  router->routes[router->len] = new_route(path, f, args);
  router->len++;
}

void match_route(char *reg_path, char *inc_path, RouteMatchResult *res) {

  int i = 0;
  int j = 0;
  int c_path_idx = 0;
  while (i < strlen(reg_path)) {

    if (reg_path[i] != inc_path[j] && reg_path[i] != '{') {
      res->paths_len = 0;
      res->match = 0;
      return;
    }
    if (reg_path[i] != '{') {
      i++;
      j++;
      continue;
    }

    int reg_start = i + 1;
    int inc_start = j;

    int reg_end = reg_start;
    int inc_end = inc_start;

    while (reg_path[reg_end] != '}')
      reg_end++;
    while (inc_path[inc_end] != '/' && inc_path[inc_end] != '\0')
      inc_end++;

    char *reg_var = malloc(reg_end - reg_start + 1);
    char *inc_var = malloc(inc_end - inc_start + 1);
    memcpy(reg_var, reg_path + reg_start, reg_end - reg_start);
    memcpy(inc_var, inc_path + inc_start, inc_end - inc_start);
    reg_var[reg_end - reg_start + 1] = '\0';
    inc_var[inc_end - inc_start ] = '\0';

    res->paths[res->paths_len].key = reg_var;
    res->paths[res->paths_len++].value = inc_var;

    i = reg_end + 1;
    j = inc_end;
  }
  if (j < strlen(inc_path)) {
    res->paths_len = 0;
    res->match = 0;
    return;
  }
  res->match = 1;
}

Response *route_request_and_handle(Router *router, Request *req) {

  Entry *ents = malloc(sizeof(Entry) * 8);

  RouteMatchResult m_res = {.match = 0, .paths = ents, .paths_len = 0};

  for (size_t i = 0; i < router->len; i++) {
    match_route(router->routes[i]->path, req->path, &m_res);
    if (m_res.match == 1) {
      Route *route = router->routes[i];

      for (int k = 0; k < m_res.paths_len; k++) {
        set(req->headers, m_res.paths[k].key, m_res.paths[k].value);
      }

      return route->f(req, route->args);
    }
  }
  Response *resp = res(NOT_FOUND, "PATH NOT FOUND");
  return resp;
}
