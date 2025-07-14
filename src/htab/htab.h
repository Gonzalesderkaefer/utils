#ifndef HTAB_H
#define HTAB_H

#include <sys/types.h>
#include <stdbool.h>


// Macros
#define INIT_SIZE 16

// TODO: Add documentation

typedef struct _Htab  Htab;
typedef void *(*HtabAlloc)(size_t);
typedef void (*HtabDeAlloc)(void *);

Htab *htab_init(void *(*alloc)(size_t), void (*dealloc)(void *));

void htab_insert(Htab *htab, void *value, void *key, size_t keylen, bool allocated);

void *htab_lookup(Htab *htab, char *key, size_t keylen);

void htab_free(Htab *htab);

HtabAlloc htab_alloc(Htab *htab);

HtabDeAlloc htab_dealloc(Htab *htab);

#endif // HTAB_H
