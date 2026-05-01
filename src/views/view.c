#include "view.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *begin = "{{";
const char *end = "}}";

char *read_view(char *path) {
  FILE *f = fopen(path, "rb");
  if (!f)
    return NULL;
  if (fseek(f, 0, SEEK_END) != 0) {
    perror("Errro in fseek");
    fclose(f);
    return NULL;
  }
  long size_l = ftell(f);
  if (size_l < 0) {
    perror("Errro in ftell");
    fclose(f);
    return NULL;
  }
  size_t size = (size_t)size_l;
  rewind(f);
  char *buffer = malloc(size + 1);
  if (!buffer) {
    perror("Errro in allocating");
    fclose(f);
    return NULL;
  }
  size_t read = fread(buffer, 1, size, f);
  buffer[read] = '\0';
  fclose(f);
  return buffer;
}

typedef struct {
  int start;
  int end;
  char *var;
} Expression;

char *replace_vars(View *v) {

  Expression *exps = malloc(sizeof(Expression) * 10);
  int len = 0;

  char *view_str = read_view(v->path);

  int i = 0;
  int str_len = strlen(view_str);
  while (i < str_len - 1) {
    int cond = view_str[i] == '{' && view_str[i + 1] == '{';
    if (cond) {
      Expression e = {0};
      // advance cursor to the start of words
      i += 2;
      e.start = i;
      while (view_str[i] != '}')
        i++;
      e.end = i;

      char *var = malloc((e.end - e.start) + 2);
      for (int k = e.start; k < e.end; k++) {
        var[k - e.start] = view_str[k];
      }
      var[(e.end - e.start)] = '\0';
      e.var = var;
      e.start -= 2;
      e.end += 2;

      char *left = malloc(e.start);
      int right_size = strlen(view_str) - e.end;
      char *right = malloc(right_size);

      strncpy(left, view_str, e.start);
      view_str = view_str + e.end;
      strncpy(right, view_str, right_size);

      char *e_var = get(v->vars, e.var);
      if (e_var == NULL) {
        e_var = "undefined";
      }

      char *temp = malloc(strlen(left) + strlen(right) + strlen(e_var));
      sprintf(temp, "%s%s%s", left, e_var, right);
      view_str = temp;

      exps[len] = e;
      len++;
    }
    i++;
  }
  // char *p = view_str;
  // int acc_offset = 0;
  // for (int i = 0; i < len; i++) {
  //   Expression e = exps[i];
  //   char *left = malloc(e.start);
  //   int right_size = strlen(p) - e.end;
  //   char *right = malloc(right_size);
  //   strncpy(left, p, e.start);
  //   p = p + e.end;
  //   strncpy(right, p, right_size);
  //
  //   char *var = get(v->vars, e.var);
  //   if (var == NULL) {
  //     var = "undefined";
  //   }
  //
  //   char *temp = malloc(strlen(left) + strlen(right) + strlen(var));
  //   sprintf(temp, "%s%s%s", left, var, right);
  //   p = temp;
  // }

  // printf("result ::\n%s\n", p);

  return view_str;
}
