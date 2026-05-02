#ifndef ROUTER_H
#define ROUTER_H

#include "../request/response.h"
#include "../request/request.h"

typedef Response* HandlerFunc(Request* req, void* args);

typedef struct {
	char* path;
	HandlerFunc *f;
	void* args; 
} Route;

typedef struct {
	size_t len;
	size_t cap;
	Route** routes;
} Router;


Router *new_router();
Route *new_route(char* path, HandlerFunc *f, void* args);
void register_route(Router* router, char* path, HandlerFunc f, void* args  );
Response* route_request_and_handle(Router* router,Request* req);

#endif // !ROUTER_H
