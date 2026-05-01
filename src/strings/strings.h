#ifndef STRING_VIEW
#define STRING_VIEW

#include <strings.h>
#include <stdlib.h>

#define STR_FMT "%.*s"
#define SV String_view

typedef struct {
	char *data; 
	size_t len ;
}String_view ;

SV sv(const char *data);
char* sv_fmt_str(String_view sv);
SV sv_substr(String_view *sv, size_t start, size_t end);
size_t sv_split(SV sv, char delim,SV *list);

#endif // !STRING_VIEW
