#include "view.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *begin = "{{";
const char *end = "}}";

char *read_view(char *path) {
  FILE *f = fopen(path, "rb");
  if (!f){

    perror("Seomthing went wrong reading the file");
      return NULL;
  }
  
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

  char *temp_ptr = view_str;

  int i = 0;
  while (i < strlen(view_str) - 1) {
    int cond = view_str[i] == '{' && view_str[i + 1] == '{';
    if (!cond) {
      i++;
      continue;
    }
    int open = i;
    Expression e = {0};
    // advance cursor to the start of words
    i += 2;
    e.start = i;
    while (view_str[i] != '}')
      i++;
    e.end = i;

    char *var_name = malloc((e.end - e.start) + 2);
    memcpy(var_name, view_str + e.start, e.end - e.start);
    var_name[e.end - e.start] = '\0';

    e.var = var_name;
    e.start -= 2;
    e.end += 2;
    // int right_size = strlen(view_str) - e.end;
    //
    // char *left = malloc(e.start + 1);
    // char *right = malloc(right_size + 1);
    //
    // strncpy(left, view_str, e.start);
    // left[e.start] = '\0';
    // view_str = view_str + e.end;
    // strncpy(right, view_str, right_size);
    // right[right_size] = '\0';

    char *e_var = get(v->vars, e.var);
    if (e_var == NULL) {
      e_var = "undefined";
    }

    int llen = open, vlen = strlen(e_var), rlen = strlen(view_str) - e.end;
    char *result = malloc(llen + vlen + rlen + 1);
    memcpy(result, view_str, llen);
    memcpy(result + llen, e_var, vlen);
    memcpy(result + llen + vlen, view_str + e.end, rlen);
    result[llen + vlen + rlen] = '\0';


    free(view_str);
    view_str = result;
    i = llen + vlen;

    // char *temp = malloc(strlen(left) + strlen(right) + strlen(e_var) + 1);
    // sprintf(temp, "%s%s%s", left, e_var, right);
    // view_str = temp;
    //
    // free(left);
    // free(right);

    // exps[len] = e;
    // len++;
    // i++;
  }

  return view_str;
}
