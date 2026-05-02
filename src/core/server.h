#ifndef SERVER_H
#define SERVER_H

#include "router/router.h"
#include <stdint.h>

typedef struct {
  const char *addr;
  uint16_t port;
  uint16_t max_reqeust_buffer;
  Router *router;
} Server;

Server *new_server(const char *addr,  uint16_t port,
                    uint16_t max_reqeust_buffer);

void declare_route(Server* server, char* path, HandlerFunc f, void* args  );

void run(Server *srv);

#endif // !SERVER_H
