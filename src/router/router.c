#include "router.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Router *new_router() {
  Router *r = malloc(sizeof(Router));
  r->routes = malloc(sizeof(Route) * 4);
  r->cap = 4;
  r->len = 0;
  return r;
}

Route *new_route(char* path, HandlerFunc *f, void* args){
  Route* route = malloc(sizeof(Route));
  route->path = path;
  route->f = f;
  route->args = args;
  return route;
}

void register_route(Router *router, char *path, HandlerFunc f, void *args) {
  if(router->len == router->cap) {
    router->cap *=2;
    router->routes = realloc(router->routes, router->cap);
  }
  router->routes[router->len]= new_route(path, f, args);
  router->len++;
}


Response* route_request_and_handle(Router* router,Request* req){

  printf("Path : |%s|",req->path);
  printf("%s\n",router->routes[0]->path);

  for(size_t i = 0 ; i < router->len ;i++){
    if(strcmp(req->path, router->routes[i]->path)==0){
      Route *route = router->routes[i];
      return route->f(req,route->args);
    }
  }

  Response* resp = res(NOT_FOUND, "PATH NOT FOUND");
  return resp;
}
