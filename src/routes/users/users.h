#ifndef USERS_H
#define USERS_H

#include "../../core/router/router.h"
#include "../../core/views/view.h"
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

int callback(void *data, int argc, char **argv, char **col_name) {
  char **result = (char **)data;
  if (argv[1]) {
    *result = (char *)malloc(strlen(argv[1]) + 1);
    strcpy(*result, argv[1]);
  }
  return 0;
}

Response *users_route_controller(Request *req, void *args) {

  sqlite3 *db = (sqlite3 *)args;
  char *err_msg = 0;
  char sql[256];

  char *userId = get(req->headers, "userId");

  sprintf(sql, "SELECT * FROM users WHERE id = %s;", userId);
  char *name = NULL;
  int rc = sqlite3_exec(db, sql, callback, (void *)&name, &err_msg);
  if (rc != SQLITE_OK) {
    printf("Select error: %s\n", err_msg);
    sqlite3_free(err_msg);
  }

  Map *m = new_map();
  set(m, "userId", get(req->headers, "userId"));
  set(m, "userName", name);
  View v = {.path = "../resources/user.template", .vars = m};
  char *formatted = replace_vars(&v);
  Response *resp = res(OK, formatted);
  return resp;
}

#endif // !USERS_H
