#include "core/server.h"
#include "routes/home/home.h"
#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <sqlite3.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {

  sqlite3 *db;
  char *err_msg = 0;

  // Open database
  int rc = sqlite3_open("../test.db", &db);
  if (rc != SQLITE_OK) {
    printf("Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }
  
  Server *srv = new_server("127.0.0.1", 8000, 16 * 1024);
  declare_route(srv, "/", home_route_controller, NULL);
  declare_route(srv, "/{userId}/name", home_route_controller, NULL);
  run(srv);
  return 0;
}
