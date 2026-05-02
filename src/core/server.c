#include "server.h"
#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

void handle_request(Router *r, int socket, char *buffer, uint16_t buffer_len) ;

Server *new_server(const char *addr, const uint16_t port,
                   const uint16_t max_reqeust_buffer) {
  Server *srv = malloc(sizeof(Server));
  srv->addr = addr;
  srv->port = port;
  srv->max_reqeust_buffer = max_reqeust_buffer;
  srv->router = new_router();
  return srv;
}

void declare_route(Server *server, char *path, HandlerFunc f, void *args) {
  register_route(server->router, path, f, args);
}

void run(Server *srv) {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  inet_pton(AF_INET, srv->addr, &address.sin_addr);
  address.sin_port = htons(srv->port);

  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  int bind_res = bind(server_fd, (struct sockaddr *)&address, sizeof(address));

  assert(bind_res == 0);

  listen(server_fd, 3);

  printf("Server listenning on port :%d \n", srv->port);

  char *buffer = malloc(sizeof(char) * srv->max_reqeust_buffer);

  while (1) {
    int addrlen = sizeof(address);
    int new_socket =
        accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

    if (new_socket < 0) {
      perror("Failed to accept connection");
      continue;
    }

    handle_request(srv->router, new_socket, buffer, srv->max_reqeust_buffer);

    close(new_socket);
  }
  close(server_fd);

}

void handle_request(Router *r, int socket, char *buffer, uint16_t buffer_len) {

  memset(buffer, 0, buffer_len);
  read(socket, buffer, buffer_len);

  SV string_v = sv(buffer);

  Request request = req(string_v);

  // routing logic

  Response *resp = route_request_and_handle(r, &request);

  char *out = stringify_response(resp);

  destroy_res(resp);
  destroy_req(&request);

  write(socket, out, strlen(out));
}


