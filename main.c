#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RECORD_KEY_MAX 128

struct record{
    char key[RECORD_KEY_MAX];
    void *value;
    struct record *next;
};

struct hashmap{
    int size;
    int count;
    struct record** array;
};

struct record *new_record(char *key, void *value, struct record *next) {
    struct record *r = malloc(sizeof(struct record));
    strcpy(r->key, key);
    r->value = value;
    r->next = next;
    return r;
}

struct hashmap *new_hashmap(int size) {
    struct hashmap *h = malloc(sizeof(struct hashmap));
    h->size = size;
    h->count = 0;
    h->array = malloc(sizeof(struct record *) * size);

    for (int i = 0; i < h->size; ++i) {
        h->array[i] = NULL;
    }

    return h;
}

int generate_hashvalue(char *key) {
    int tmp = 0;
    int len = strlen(key);

    for (unsigned int i = 0; i < len; ++i) {
        tmp = (tmp << i) + (char)key[i];
    }

    return tmp;
}

struct record *get_tail_record(struct record *record) {
    struct record *r = record;

    while (r) {
        r = r->next;
    }

    return r;
}

void append_record(struct record *body, struct record *tail) {
    body = get_tail_record(body);
    body->next = tail;
}

void hashmap_just_insert(struct hashmap *hashmap, struct record *record) {
    int hashvalue = generate_hashvalue(record->key) % hashmap->size;
    struct record *r = hashmap->array[hashvalue];
    ++hashmap->count;

    if (r == NULL) {
        hashmap->array[hashvalue] = record;
    } else {
        append_record(hashmap->array[hashvalue], record);
    }
}

void hashmap_rehash(struct hashmap *hashmap) {
    int size = hashmap->size * 2;
    struct record **array = malloc(sizeof(struct record *) * size);

    for (int i = 0; i < size; ++i) {
        array[i] = NULL;
    }

    int hashvalue;

    for (int i = 0; i < hashmap->size; ++i) {
        for (struct record *r = hashmap->array[i]; r; r = r->next) {
            hashvalue = generate_hashvalue(r->key);
            array[hashvalue % size] = r;
        }
    }

    hashmap->size = size;
    hashmap->array = array;
}

void hashmap_insert(struct hashmap *hashmap, struct record *record) {
    if (hashmap->count * 2 >= hashmap->size) {
        hashmap_rehash(hashmap);
    }

    hashmap_just_insert(hashmap, record);
}

void print_hashmap(struct hashmap *hashmap) {
    printf("<%%hashmap count: %d, size: %d\n", hashmap->count, hashmap->size);
    for (int i = 0; i < hashmap->size; ++i) {
        for (struct record *r = hashmap->array[i]; r; r = r->next) {
            printf("  <%%record key: %s, value: %s\n", r->key, r->value);
        }
    }
    printf(">\n");
}

int main(void) {
    struct hashmap *hashmap = new_hashmap(4);

    hashmap_insert(hashmap, new_record("x00", NULL, NULL));
    hashmap_insert(hashmap, new_record("x01", NULL, NULL));
    hashmap_insert(hashmap, new_record("x02", NULL, NULL));
    hashmap_insert(hashmap, new_record("x03", NULL, NULL));
    hashmap_insert(hashmap, new_record("x04", NULL, NULL));

    print_hashmap(hashmap);

    return 0;
}
