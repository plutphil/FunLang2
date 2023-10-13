#include "fallbackstdlib.hpp"

#define LISTSIZE 15
typedef struct KeyValPair {
	char* key;
	char* val;
}KeyValPair;
typedef struct StaticArrayList {
	int size = 0;
	KeyValPair items[LISTSIZE];
	char* next;
}StaticArrayList;

void statlist_add(StaticArrayList* list, char* key, char* val) {
	list->items[list->size].key = key;
	list->items[list->size++].val = val;
}
void statlist_get(StaticArrayList* list, int i, char** key, char** val) {
	*key = list->items[i].key;
	*val = list->items[i].val;
}
#define MAPSIZE 13
typedef struct StaticHashMap {
	StaticArrayList items[MAPSIZE];
}StaticHashMap;

int strhash2(char* str) {
	//https://stackoverflow.com/questions/7666509/hash-function-for-string
	int hash = 5381;
	char c;
	int maxloop = 100;
	while (c = *str++ && maxloop > 0) {
		maxloop--;
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	return hash;
}
KeyValPair* statmap_contains(StaticHashMap* statmap, char* key) {
	unsigned int keyhash = strhash2(key);
	StaticArrayList* arrl = &(statmap->items[keyhash % MAPSIZE]);
	int i = 0;
	while (i < arrl->size) {
		if (__strcmp(arrl->items[i].key, key) == 0) {
			return &arrl->items[i];
		}
		i++;
	}
	return 0;
}
char* statmap_get(StaticHashMap* statmap, char* key) {
	StaticArrayList* arrl;
	KeyValPair* kv = statmap_contains(statmap, key);
	if (kv !=0) {
		return kv->val;
	}
	return 0;
}


char* statmap_set(StaticHashMap* statmap, char* key, char* val) {

	unsigned int keyhash = strhash2(key);
	StaticArrayList* arrl = &(statmap->items[keyhash % MAPSIZE]);
	int i = 0;
	KeyValPair* kv = 0;
	while (i < arrl->size) {
		if (__strcmp(arrl->items[i].key, key) == 0) {
			kv = &arrl->items[i];
		}
		i++;
	}
	if (i == 0) {
		statlist_add(arrl, key, val);
	}
	else {
		arrl->items[i].key = key;
		arrl->items[i].val = val;
	}
	return val;
}

