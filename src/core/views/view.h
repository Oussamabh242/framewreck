#ifndef VIEW
#define VIEW

#include "../map/map.h"
#include <stdlib.h>

 typedef struct{
	 char *path;
	 Map *vars;
 } View;


char *read_view(char* path);
char *replace_vars(View *v);

#endif // !VIEW
