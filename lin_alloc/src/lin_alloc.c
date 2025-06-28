// Header file
#include "lin_alloc.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

/// Block is 16MiB in size 
#define blocksize 16777216



/******************* Block implementation *******************/
typedef struct _Block Block;


typedef struct _Block{ 
    void *memory;
    void *pos;
    void *endpos;
    Block *next;
} Block;


static Block *blk_init() {
    // Allocate Block struct
    Block *new_block = malloc(sizeof(Block));
    if (new_block == NULL) {
        return new_block;
    }

    // Allocate memory
    void *new_memory = malloc(blocksize);
    if (new_memory == NULL) {
        free(new_block);
        return NULL;
    }

    // Assign values
    new_block->memory = new_memory;
    new_block->next = NULL;
    new_block->pos = new_memory;
    new_block->endpos = new_memory + blocksize;


    return new_block;
}


static void block_free(Block *blk) {
    // NULL check
    if (blk == NULL) return;

    // free memory then block
    free(blk->memory);
    free(blk);
}

static void *blk_alloc(Block *blk, const size_t size, bool zero) {
    // NULL check
    if (blk == NULL) return NULL;

    // This must always be true
    assert(blk->endpos > blk->pos);

    // Check if there's enough space
    if (blk->endpos - blk->pos < size) return NULL;

    // Store current position
    void *to_return = blk->pos;

    // zero out if needed
    if (zero) memset(to_return, 0, size);

    // advance poition by size
    blk->pos += size;

    return to_return;
}






/******************* Linear Allocator implementation *******************/
typedef struct _LinAlloc {
    Block *first;
    Block *current;
} LinAlloc;



LinAlloc *linear_init() {
    // Allocate struct
    LinAlloc *new_linear = malloc(sizeof(LinAlloc));
    if (new_linear == NULL) {
        return NULL;
    }

    // Allocate first block
    Block *new_block = blk_init();
    if (new_block == NULL) {
        free(new_linear);
        return NULL;
    }

    // Assign nodes
    new_linear->first = new_block;
    new_linear->current = new_block;


    return new_linear;
}


void *lalloc(LinAlloc *linear_alloc, size_t size) {
    Block *cur_blk = linear_alloc->current; 
    assert(cur_blk != NULL); // This cannot be null
    void *ptr = NULL;
    while ((ptr = blk_alloc(cur_blk, size, false)) == NULL) {
        // Allocate new block
        Block *new_block = blk_init();
        if (new_block == NULL) {
            return NULL;
        }

        // Set next block to new_block
        cur_blk->next = new_block;

        // 'increment' cur_blk
        cur_blk = cur_blk->next;

        // Set next cur blk
        linear_alloc->current = cur_blk;
    }
    return ptr;
}

