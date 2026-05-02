#ifndef HOME_H
#define HOME_H

#include "../../core/router/router.h"
#include "../../core/views/view.h"
#include <stdio.h>

Response *home_route_controller(Request *req, void *args) {
  Map *m = new_map();
  set(m, "something", "thingsome");
  set(m, "userId", get(req->headers, "userId"));
  View v = {.path = "../resources/main.template", .vars = m};
  char *formatted = replace_vars(&v);
  Response *resp = res(OK, formatted);
  return resp;
}

#endif // !HOME_H
