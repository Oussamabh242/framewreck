#ifndef MAP_H
#define MAP_H

#include<stdio.h>

typedef struct {
	char* key;
	char* value;
}Entry ;

typedef struct {
	Entry *entries;
	size_t len ;
	size_t cap ;
} Map;

Map *new_map();
Entry *new_entry(char* key, char* value );
char *get(Map *m, char *key);
void set(Map *m, char *key, char *val);
void destroy(Map *m);

#endif // !MAP_H
