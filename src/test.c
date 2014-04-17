#include "test.h"

static struct pair *map[HASH_TABLE_LENGTH];

int
get_hash_key(char *str)
{
	unsigned int seed = 131;
	unsigned int hash = 0;

	while (*str) {
		hash = hash * seed + (*str++);
	}

	return (hash % M);
}

void
insert(char *str, int value)
{
	struct pair *p;
	int key = get_hash_key(str);

	if (map[key] == NULL) {
		map[key] = (struct pair *)malloc(sizeof(struct pair));
		map[key]->value = value;
		map[key]->next = NULL;
		return;
	}

	p = map[key];
	map[key] = (struct pair *)malloc(sizeof(struct pair));
	map[key]->value = value;
	map[key]->next = p;
}

int
is_exist(char *str, int value)
{
	int key = get_hash_key(str);	
	struct *p = map[key];

	while (p != NULL) {
		if (p->value == value) {
			return 1;
		}		
	}

	return 0;
}

int 
serach(char *str)
{
	int key = get_hash_key(str);	
	struct *p = map[key];

	while (p != NULL) {
		if (strcpy(p))
	}

	return -1;
}

int
main()
{

	return 0;
}
