#include "map.h"
#include <string.h>
#include <stdlib.h>

Map *new_map(){
	Map *m = (Map*)malloc(sizeof(Map));
	m->cap = 8 ;
	m->len = 0;
	m->entries = malloc(sizeof(Entry)*8);
	return m;
}

Entry *new_entry(char *key, char *value){
	Entry *e = malloc(sizeof(Entry));
	e->key = key;
	e->value = value;
	return e;
}

void set(Map *m, char *key, char *val){
	if(m->len == m->cap){
		m->entries = realloc(m->entries,sizeof(Entry)*m->cap*2 );
		m->cap*=2;
	}
	for(size_t i; i < m->len ; i++){
		if(strcmp(key, m->entries[i].key)){
			m->entries[i].value = val;
			return;
		}
	}
	m->entries[m->len].key = key;
	m->entries[m->len].value = val;
	m->len++;
}

char* get(Map *m, char *key){
	for(size_t i = 0 ; i < m->len; i++){
		if(strcmp(m->entries[i].key, key) == 0){
			return m->entries[i].value;
		}

	}
	return NULL;
}


void destroy(Map *m){
	free(m->entries);
	free(m);
}
