#include "map/map.h"
#include "request/request.h"
#include "request/response.h"
#include "strings/strings.h"
#include "views/view.h"
#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

const size_t MAX_BUFFER = 16 * 1024;
void handle_request(int socket, char *buffer);
Response *handle(Request *r);

int main() {

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
  address.sin_port = htons(8000);

  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  int bind_res = bind(server_fd, (struct sockaddr *)&address, sizeof(address));

  assert(bind_res == 0);

  listen(server_fd, 3);

  printf("Server listenning on port 8000\n");

  char *buffer = malloc(sizeof(char) * MAX_BUFFER);

  while (1) {
    int addrlen = sizeof(address);
    int new_socket =
        accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

    if (new_socket < 0) {
      perror("Failed to accept connection");
      continue;
    }

    handle_request(new_socket, buffer);

    close(new_socket);
  }
  close(server_fd);

  return 0;
}

void handle_request(int socket, char *buffer) {

  memset(buffer, 0, MAX_BUFFER);
  read(socket, buffer, MAX_BUFFER);

  SV string_v = sv(buffer);

  Request r = req(string_v);

  Response *resp = handle(&r);
  char *out = stringify_response(resp);

  destroy_res(resp);
  destroy_req(&r);

  write(socket, out, strlen(out));
}

Response *handle(Request *r) {
  if (strcmp(r->path, "/") != 0) {
    Response *resp = res(NOT_FOUND, "Something went wrong");
    return resp;
  }

  Map *m = new_map();
  set(m, "something", "thingsome");
  View v = {.path = "../resources/main.template", .vars = m};
  char *formatted = replace_vars(&v);
  Response *resp = res(OK, formatted);
  return resp;
}
