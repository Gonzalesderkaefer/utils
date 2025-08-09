// Header file
#include "tree.h"

// Libraries
#include <assert.h>
#include <complex.h>
#include <locale.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


// Macros
#define min(x,y) (((x) < (y)) ? (x) : (y))
#define max(x,y) (((x) > (y)) ? (x) : (y))

/********************************* TreeNode ***********************************/

typedef struct _TreeNode TreeNode;


struct _TreeNode {
    uint64_t height;
    TreeNode *parent;
    TreeNode *left;
    TreeNode *right;
    void *value;
};

typedef enum {
    Left,
    Right,
    Root,
    Fail,
} Direction;

typedef enum {
    LeftRot,
    RightRot,
} RotationDir;

static TreeNode *node_init(const void *value, const TreeNode *parent, size_t elem_size, AllocFunc alloc) {
    // Allocate Node
    TreeNode *new_node = alloc(sizeof(TreeNode) + elem_size);
    if (new_node == NULL) {
        return NULL;
    }

    // Get storage pointer
    void *value_ptr = (char *)new_node + sizeof(TreeNode);

    memcpy(value_ptr, value, elem_size);


    // Assign values
    new_node->parent = (TreeNode *)parent;
    new_node->value = value_ptr;
    new_node->height = 1;
    new_node->left = NULL;
    new_node->right = NULL; 

    return new_node;
}

static Direction child_dir(const TreeNode *node) {
    assert(node != NULL);
    if (node->parent == NULL) {
        return Root;
    }
    if (node->parent->left == node) {
        return Left;
    }
    if (node->parent->right == node) {
        return Right;
    }
    // Something's not right
    return Fail;
}



static uint64_t node_height(const TreeNode *node) {
    if (node != NULL) {
        return node->height;
    } else {
        return 0;
    }
}
// NOTE:
// return_val < 0 ==> Right heavy
// return_val > 0 ==> Left heavy
static int64_t node_get_balance(const TreeNode *node) {
    if (node != NULL) {
        return node_height(node->left) - node_height(node->right);
    } else {
        return 0;
    }
}

static void node_update_height(TreeNode *node) {
    if (node == NULL) {
        return;
    }
    node->height = 1 + max(node_height(node->left), node_height(node->right));
}



static TreeNode *node_rotate(TreeNode *node, const RotationDir dir) {
    // NULL checks
    if (node == NULL) {
        return NULL;
    }

    TreeNode *old_root = node, *old_parent = node->parent;
    TreeNode *new_root, *inner_grandchild;
    switch (dir) {
    case Left:
        new_root = node->right;
        inner_grandchild = new_root->left;
        new_root->left = old_root;
        old_root->right = inner_grandchild;
        old_root->parent = new_root;
        new_root->parent = old_parent;
        node_update_height(old_root);
        node_update_height(new_root);
        break;
    case Right:
        new_root = node->left;
        inner_grandchild = new_root->right;
        new_root->right = old_root;
        old_root->left = inner_grandchild;
        node_update_height(old_root);
        node_update_height(new_root);
        break;
    }
    return new_root;
}





static TreeNode *node_balance(TreeNode *node) {
    if (node == NULL) {
        return NULL;
    }
    int balance_value = node_get_balance(node);
    if (balance_value <= -2) { // Is right heavy -> rotate left
        int inner_bal = node_get_balance(node->right);
        if (inner_bal >= 1) {
            node->right = node_rotate(node->right, RightRot);
        }
        return node_rotate(node, LeftRot);

    } else if (balance_value >= 2) { // Is left heavy -> rotate right
        int inner_bal = node_get_balance(node->left);
        if (inner_bal <= -1) {
            node->left = node_rotate(node->left, LeftRot);
        }
        return node_rotate(node, RightRot);
    }
    return node;
}


static void node_free(FreeFunc dealloc, TreeNode *node) {
    dealloc(node);
}




/********************************* Tree ***************************************/

struct _Tree {
    size_t elem_size;
    TreeNode *root;
    AllocFunc alloc;
    FreeFunc dealloc;
    Comparator comp;
};


Tree *tree_init(const size_t elem_size, const AllocFunc alloc, const FreeFunc dealloc, const Comparator comp) {
    // Check if alloc and free could be NULL
    AllocFunc local_alloc = alloc;
    FreeFunc local_free = dealloc;
    if (alloc == NULL || dealloc == NULL) {
        local_alloc = malloc;
        local_free = free;
    }


    // Allcoate local tree
    Tree local_tree = {
        .elem_size = elem_size,
        .alloc = local_alloc,
        .dealloc = local_free,
        .root = NULL,
        .comp = comp == NULL ? memcmp : comp,
    };


    // Allocate space on heap
    Tree *new_tree = local_alloc(sizeof(Tree));
    if (new_tree == NULL) {
        return NULL;
    }
    memcpy(new_tree, &local_tree, sizeof(Tree));

    return new_tree;
}

Tree *tree_from_handle(SpecialTree handle) {
    // Sanity check
    if (handle == NULL) {
        return NULL;
    }
    void *tree_struct = (char *)handle - sizeof(Tree *);
    return *(Tree **)tree_struct;
}


SpecialTree tree_init_special(const size_t elem_size, AllocFunc alloc, FreeFunc dealloc, Comparator comp) {

    Tree *tree_ptr = tree_init(elem_size, alloc, dealloc, comp);
    if (tree_ptr == NULL) {
        return NULL;
    }



    void *handle_and_buffer = tree_ptr->alloc(sizeof(Tree *) + elem_size);
    if (handle_and_buffer == NULL) {
        FreeFunc dealloc = tree_ptr->dealloc;
        dealloc(tree_ptr);
        return NULL;
    }


    // Copy tree pointer to handle_and_buffer
    memcpy(handle_and_buffer, &tree_ptr, sizeof(Tree *));

    // Get buffer pointer
    void *buf_ptr = (char *)handle_and_buffer + sizeof(Tree *);


    return buf_ptr;
}


void tree_free_special(SpecialTree handle) {
    if (handle == NULL) {
        return;
    }
    Tree *tree = tree_from_handle(handle);
    void *handle_and_buffer = (char *)handle - sizeof(Tree *);
    FreeFunc dealloc = tree->dealloc;
    tree_free(tree);
    dealloc(handle_and_buffer);
}




Tree *tree_init_def(const size_t elem_size, const Comparator comp) {
    // Allcoate local tree
    Tree local_tree = {
        .elem_size = elem_size,
        .alloc = malloc,
        .dealloc = free,
        .root = NULL,
        .comp = comp,
    };

    Tree *new_tree = malloc(sizeof(Tree));
    if (new_tree == NULL) {
        return NULL;
    }
    memcpy(new_tree, &local_tree, sizeof(Tree));

    return new_tree;
}

void tree_insert(Tree *tree, const void *value, const size_t val_size) {
    // Sanity check
    if (value == NULL || tree->comp == NULL || 
            tree->alloc == NULL || tree->dealloc == NULL || val_size != tree->elem_size) {
        return;
    }

    // This is the first insertion
    if (tree->root == NULL) {
        tree->root = node_init(value, NULL, tree->elem_size, tree->alloc);
        if (tree->root == NULL) {
            return;
        }
        return;
    }

    // Create new node that will be added to the tree
    TreeNode *new_node = node_init(value, NULL, tree->elem_size, tree->alloc);
    if (new_node == NULL) {
        return;
    }

    // Find the parent of the new node
    TreeNode *parent = tree->root;
    while (parent != NULL) {
        int compval = tree->comp(value, parent->value, tree->elem_size); 

        if (compval == 0) { // NO duplicates!!
            // We have to free this node first
            node_free(tree->dealloc, new_node);
            return;
        } else if (compval < 0) { // Go left
            if (parent->left == NULL) {
                // Set for parent node
                parent->left = new_node;
                new_node->parent = parent;
                break;
            }
            // Keep going
            parent = parent->left;
        } else if (compval > 0) { // Go right
            if (parent->right == NULL) {
                // Set for parent node
                parent->right = new_node;
                new_node->parent = parent;
                break;
            }
            // Keep going
            parent = parent->right;
        }
    }
    // Parent cannot be null
    assert(parent != NULL);

    // We have to rotate the grand parent node
    TreeNode *g_parent = parent->parent;

    // update heights
    while (parent != NULL) {
        node_update_height(parent);
        parent = parent->parent;
    }

    if (g_parent == NULL) {
        return;
    }
    const Direction dir = child_dir(g_parent);
    assert(dir != Fail && "Getting direction failed\n");

    switch (dir) {
    case Left:
        g_parent->parent->left = node_balance(g_parent);
        break;
    case Right:
        g_parent->parent->right = node_balance(g_parent);
        break;
    case Root:
        tree->root = node_balance(g_parent);
        break;
    case Fail:
        break;
    }
}







void *tree_insert_with_buf(Tree *tree, const void *value, const size_t val_size) {
    // Sanity check
    if (value == NULL || tree->comp == NULL || 
            tree->alloc == NULL || tree->dealloc == NULL || val_size != tree->elem_size) {
        return NULL;
    }

    // This is the first insertion
    if (tree->root == NULL) {
        tree->root = node_init(NULL, NULL, tree->elem_size, tree->alloc);
        if (tree->root == NULL) {
            return NULL;
        }
        return (void *)tree->root->value;
    }

    // Create new node that will be added to the tree
    TreeNode *new_node = node_init(NULL, NULL, tree->elem_size, tree->alloc);
    if (new_node == NULL) {
        return NULL;
    }

    // Find the parent of the new node
    TreeNode *parent = tree->root;
    while (parent != NULL) {
        int compval = tree->comp(value, parent->value, tree->elem_size); 

        if (compval == 0) { // NO duplicates!!
            // We have to free this node first
            node_free(tree->dealloc, new_node);
            return NULL;
        } else if (compval < 0) { // Go left
            if (parent->left == NULL) {
                // Set for parent node
                parent->left = new_node;
                new_node->parent = parent;
                break;
            }
            // Keep going
            parent = parent->left;
        } else if (compval > 0) { // Go right
            if (parent->right == NULL) {
                // Set for parent node
                parent->right = new_node;
                new_node->parent = parent;
                break;
            }
            // Keep going
            parent = parent->right;
        }
    }
    // Parent cannot be null
    assert(parent != NULL);

    // We have to rotate the grand parent node
    TreeNode *g_parent = parent->parent;

    // update heights
    while (parent != NULL) {
        node_update_height(parent);
        parent = parent->parent;
    }

    if (g_parent == NULL) {
        return NULL;
    }
    const Direction dir = child_dir(g_parent);
    assert(dir != Fail && "Getting direction failed\n");

    switch (dir) {
    case Left:
        g_parent->parent->left = node_balance(g_parent);
        break;
    case Right:
        g_parent->parent->right = node_balance(g_parent);
        break;
    case Root:
        tree->root = node_balance(g_parent);
        break;
    case Fail:
        break;
    }

    // Return buffer to write to
    return (void *)new_node->value;
}








static const TreeNode *tree_lookup_node(const Tree *tree, const void *value) {
    TreeNode *cur_node = tree->root;
    int compare_value;
    while (cur_node != NULL) {
        compare_value = tree->comp(value, cur_node->value, tree->elem_size);
        if (compare_value == 0) {
            return cur_node;
        } else if (compare_value < 0) { // Go left
            cur_node = cur_node->left;
        } else { // Go right
            cur_node = cur_node->right;
        }
    }
    return NULL;
}

const void *tree_lookup(const Tree *tree, const void *value) {
    // Sanity check
    if (tree == NULL || value == NULL) {
        return NULL;
    }

    // Find node
    const TreeNode *found_node = tree_lookup_node(tree, value);

    if (found_node == NULL) {
        return NULL;
    }

    // return value
    return found_node->value;

}


static void free_nodes(TreeNode *root, FreeFunc dealloc) {
    if (root == NULL) {
        return;
    } else if (root->left == NULL && root->right == NULL) {
        node_free(dealloc, root);
    } else {
        free_nodes(root->left, dealloc);
        free_nodes(root->right, dealloc);
        dealloc(root);
    }
}


void tree_delete(Tree *tree, const void *value) {
    // Sanity check
    if (tree == NULL || value == NULL) {
        return;
    }

    // find node to delete
    TreeNode *to_delete = (TreeNode *) tree_lookup_node(tree, value);
    TreeNode *to_del_parent = to_delete->parent;
    if (to_delete == NULL) {
        return;
    }
    while (to_delete != NULL) {
        if (to_delete->left != NULL && to_delete->right != NULL) {
            // Find next greater
            TreeNode *next = to_delete->left;
            while (next->right != NULL) {
                next = next->right;
            }
            // Replace value
            to_delete-> value = next->value;

            // Now delete the next
            to_delete = next;
        } else if (to_delete->left != NULL) {
            const Direction dir = child_dir(to_delete);
            switch (dir) {
            case Left:
                to_delete->parent->left = to_delete->left;
                to_delete->left->parent = to_delete->parent;
                break;
            case Right:
                to_delete->parent->right = to_delete->left;
                to_delete->left->parent = to_delete->parent;
                break;
            case Root:
                tree->root = to_delete->left;
                to_delete->left->parent = NULL;
                break;
            case Fail:
                break;
            }
            node_free(tree->dealloc, to_delete);
            break;
        } else if (to_delete->right != NULL) {
            const Direction dir = child_dir(to_delete);
            switch (dir) {
            case Left:
                to_delete->parent->left = to_delete->right;
                to_delete->right->parent = to_delete->parent;
                break;
            case Right:
                to_delete->parent->right = to_delete->right;
                to_delete->right->parent = to_delete->parent;
                break;
            case Root:
                tree->root = to_delete->right;
                to_delete->right->parent = NULL;
                break;
            case Fail:
                break;
            }
            node_free(tree->dealloc, to_delete);
            break;
        } else { // Both are NULL
            const Direction dir = child_dir(to_delete);
            switch (dir) {
            case Left:
                to_delete->parent->left = NULL;
                break;
            case Right:
                to_delete->parent->right = NULL;
                break;
            case Root:
                tree->root = NULL;
                break;
            case Fail:
                break;
            }
            node_free(tree->dealloc, to_delete);
            break;
        }

        if (to_del_parent == NULL) {
            return;
        }
        const Direction dir = child_dir(to_del_parent);

        switch (dir) {
        case Left:
            to_del_parent->parent->left = node_balance(to_del_parent);
            break;
        case Right:
            to_del_parent->parent->right = node_balance(to_del_parent);
            break;
        case Root:
            tree->root = node_balance(to_del_parent);
            break;
        case Fail:
            break;
        }
    }
}


void tree_free(Tree *tree) {
    // Free all nodes
    free_nodes(tree->root, tree->dealloc);

    FreeFunc dealloc = tree->dealloc;
    dealloc(tree);
}



