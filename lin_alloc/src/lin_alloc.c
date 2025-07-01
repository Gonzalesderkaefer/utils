// Header file
#include "lin_alloc.h"

// Libraries
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>




/******************* Block implementation *******************/
typedef struct _Block Block;


typedef struct _Block{ 
    void *memory;
    void *pos;
    void *endpos;
    Block *next;
    Block *prev;
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
    new_block->prev = NULL;
    new_block->pos = new_memory;
    new_block->endpos = new_memory + blocksize;


    return new_block;
}

static Block *blk_init_sized(size_t size) {
    // Allocate Block struct
    Block *new_block = malloc(sizeof(Block));
    if (new_block == NULL) {
        return new_block;
    }

    // Allocate memory
    void *new_memory = malloc(size);
    if (new_memory == NULL) {
        free(new_block);
        return NULL;
    }

    // Assign values
    new_block->memory = new_memory;
    new_block->next = NULL;
    new_block->prev = NULL;
    new_block->pos = new_memory;
    new_block->endpos = new_memory + size;


    return new_block;
}


static void blk_free(Block *blk) {
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






/********************* Linear Allocator implementation **********************/
void *lalloc(LinAlloc *linear_alloc, size_t size){
    // Allocate first block if needed
    if (linear_alloc->first == NULL) {
        Block *new_block = blk_init();
        if (new_block == NULL) {
            return NULL;
        }

        // Assign to struct
        linear_alloc->first = new_block;
        linear_alloc->current = new_block;
    }

    // Get current block
    Block *cur_blk = linear_alloc->current;

    // Check if custom block needs to be allocated
    if (size > blocksize) {
        Block *new_blk = blk_init_sized(size);
        if (new_blk != NULL) {
            return NULL;
        }

        // Get new pointer
        void *to_return = blk_alloc(new_blk, size, false);
        if (to_return == NULL) {
            return NULL;
        }

        // Make new block predecessor of cur_blk;
        new_blk->next = cur_blk;
        new_blk->prev = cur_blk->prev;
        cur_blk->prev = new_blk;

        return to_return;
    } else {
        void *ptr = blk_alloc(cur_blk, size, false);
        if (ptr == NULL) {
            // Allocate new block
            Block *new_blk = blk_init();
            if (new_blk == NULL) {
                return NULL;
            }
            // Allocate in block
            ptr = blk_alloc(new_blk, size, false);
            assert(new_blk != NULL); // This must work

            // Set next and prev refs accordingly
            new_blk->prev = cur_blk;
            cur_blk->next = new_blk;
        }
        return ptr;
    }
}



void lfree(LinAlloc *linear_alloc) {

    Block *cur_blk = linear_alloc->first;
    Block *next_blk = NULL;
    while (cur_blk != NULL) {
        // Store next block
        next_blk = cur_blk->next;
        // free block
        blk_free(cur_blk);
        // Assign it to cur_blk
        cur_blk = next_blk;
    }
    linear_alloc->current = NULL;
    linear_alloc->first = NULL;
}
