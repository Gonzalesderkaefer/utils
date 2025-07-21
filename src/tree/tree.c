// Header file
#include "tree.h"

// Libraries
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// Macros
#define max(x,y) (((x) > (y)) ? (x) : (y))
#define min(x,y) (((x) < (y)) ? (x) : (y))



/// Direction in a Node
///
/// This enum represents the side of a [TreeNode]
/// for the left or right child.
typedef enum {
    Left,
    Right,
    None,
} Direction;



/// Element in a [Tree]
///
/// This is the fundamental element in a [Tree]
/// and holds a Pointer to two child Nodes and
/// also holds the value.
typedef struct _TreeNode TreeNode;

typedef struct _TreeNode {
    TreeNode *left_node;
    TreeNode *right_node;
    TreeNode *parent_node;
    uint64_t height;
    const void *value;
} TreeNode;


typedef struct _Tree {
    const size_t elem_size;
    const Alloc alloc;
    const DeAlloc dealloc;
    const Comparator compare;
    TreeNode *root;
} Tree;



/// initialize a [TreeNode]
///
/// This function initializes a [TreeNode] with a value by allocating
/// Memory for the struct itself and the value;
///
/// Parameters:
///   - value pointer: to the memory that needs to be copied
///   - value_size: the size of the memoryblock pointed to by [value]
///   - alloc: an allocator function
///
/// Returns:
///   pointer to a [TreeNode] or NULL if [alloc] returns NULL
static TreeNode *node_init(const void *value, const size_t value_size, TreeNode *partent, const Alloc alloc) {
    // Allocate new_node
    TreeNode *new_node = alloc(sizeof(TreeNode));
    if (new_node == NULL) {
        return NULL;
    }

    // Assign fields
    new_node->left_node = NULL;
    new_node->right_node = NULL;
    new_node->height = 0;
    new_node->parent_node = partent;

    // initialize space for the value
    new_node->value = value;

    return new_node;
}

Tree *tree_init(Alloc alloc, DeAlloc dealloc, Comparator compare, size_t elem_size) {
    // Define local tree
    Tree local_tree = {
        .elem_size = elem_size,
        .alloc = alloc,
        .dealloc = dealloc,
        .compare = compare,
        .root = NULL,
    };
    // Allocate on the heap 
    Tree *heap_tree = alloc(sizeof(Tree));
    if (heap_tree == NULL) {
        return NULL;
    }
    // Copy local Tree to heap
    memcpy(heap_tree, &local_tree, sizeof(Tree));

    return heap_tree;
}


/// Update the height of a node
///
/// This macro updates the height of a node in the tree
#define update_height(node) node->height = max(node->left_node->height, node->right_node->height)



/// Get where [node] is appended at its parent
///
/// This function returns on which side [node] is appended
/// at [parent] 
///
/// Parameters:
///   - node: Node to check
///   - parent: parent node of [node]
///
/// Returns:
///   [Left] or [Right] if the node is actually connected. Else returns
///   [None].
static Direction child_dir(TreeNode *node) {
    if (node->parent_node == NULL) {
        return None;
    }
    if (node == node->parent_node->right_node) {
        return Right;
    }
    if (node == node->parent_node->left_node) {
        return Left;
    }
    return None;
}


/// Rotate [node] to the direction [dir]
///
/// This function does a treerotation to the child node of
/// [parent] in the direction specified by [dir]. [parent]
/// is required because the [node] will be replaced by one of
/// its children
///
/// Parameters:
///   - node: Node that needs to be rotated
///   - parent: Parent of [node]
///   - dir: Direction to rotate in
///
/// Returns:
///   New root of subtree
static TreeNode *rotate(TreeNode *node, Direction dir) {
    // NULL checks
    if (node == NULL) {
        return NULL;
    }

    TreeNode *old_root = node, *new_root, *inner_grandchild;
    switch (dir) {
    case Left:
        new_root = node->right_node;
        inner_grandchild = new_root->left_node;
        new_root->left_node = old_root;
        old_root->right_node = inner_grandchild;
        update_height(old_root);
        update_height(new_root);
        break;
    case Right:
        new_root = node->left_node;
        inner_grandchild = new_root->right_node;
        new_root->right_node = old_root;
        old_root->left_node = inner_grandchild;
        update_height(old_root);
        update_height(new_root);
        break;
    case None:
        return NULL;
        break;
    }

    return new_root;
}



/// Get the height of the Node
///
/// This macro evaluates the height of a node
/// and return 0 if node == NULL
#define node_height(node) ((node) == (NULL) ? (0) : (uint64_t)(node->height))



/// Evaluate the balance of a node
///
/// This macro evaluates the balance of a Node
/// if NULL is passed the balance is 0 since an empty
/// tree is balanced
#define get_balance(node) ((node) == (NULL) ? (0) : (int64_t)(node_height(node->left_node)) - (node_height(node->right_node)))

/// Balance the given node if necessary
///
/// This function balances the node if the absolute height difference
/// between its left and right child node is greater than 2
///
/// Parameters:
///   - node: root node of subtree that needs to be balanced
///
/// Returns:
///   The new Root of subtree
static TreeNode *balance_node(TreeNode *node) {
    if (node == NULL) {
        return NULL;
    }
    int balance_value = get_balance(node);
    if (balance_value <= -2) { // Is right heavy -> rotate left
        int inner_bal = get_balance(node->right_node);
        if (inner_bal >= 1) {
            node->right_node = rotate(node->right_node, Right);
        }
        return rotate(node, Left);

    } else if (balance_value >= 2) { // Is left heavy -> rotate right
        int inner_bal = get_balance(node->left_node);
        if (inner_bal <= -1) {
            node->left_node = rotate(node->left_node, Left);
        }
        return rotate(node, Right);
    }
    return node;
}





void tree_insert(Tree *tree, void *value) {
    // Null cannot be inserted
    if (value == NULL) {
        return;
    }

    TreeNode *new_node = node_init(value, tree->elem_size, NULL, tree->alloc);
    if (new_node == NULL) { // This is actually not necessary
        return;
    }


    // This is the first insertion
    if (tree->root == NULL) {
        tree->root = new_node;
        tree->root->height = 1;
        return;
    }
    // Find new parent node
    TreeNode *parent = tree->root;
    while (parent->value != NULL) {
        // Compare value and parent
        int compare_value = tree->compare(parent->value, value, tree->elem_size);

        // Update height of parent
        parent->height += 1;

        if (compare_value < 0) { // Right
            // Can be inserted below parent
            if (parent->right_node == NULL) {
                parent->right_node = new_node;
                new_node->parent_node = parent;
                break;
            } else {
                // Goto parent's right and keep going
                parent = parent->right_node;
            }
        } else { // Left
            // Can be inserted below parent
            if (parent->left_node == NULL) {
                parent->left_node = new_node;
                new_node->parent_node = parent;
                break;
            } else {
                // Goto parent's left and keep going
                parent = parent->left_node;
            }
        }
    }

    TreeNode *inserted_par = new_node->parent_node;
    assert(inserted_par != NULL);
    while (true) {
        // We are currently at the root node
        if (inserted_par->parent_node == NULL) {
            tree->root = balance_node(tree->root);
            break;
        }
        const Direction dir = child_dir(inserted_par);
        switch (dir) {
        case Left:
            inserted_par->parent_node->left_node = balance_node(inserted_par);
            break;
        case Right:
            inserted_par->parent_node->left_node = balance_node(inserted_par);
            break;
        case None:
            break;
        }
        inserted_par = inserted_par->parent_node;
    }
}



const void *tree_lookup(const Tree *tree, const void *value, const size_t value_len) {
    // Sanity check
    if (tree == NULL || value == NULL || value_len == 0) {
        return NULL;
    }

    TreeNode *parent = tree->root;
    while (parent != NULL) {
        // Compare value and parent
        int compare_value = tree->compare(parent->value, value, tree->elem_size);

        if (compare_value == 0) {
            return parent->value;
        }
        if (compare_value < 0) { // Right
            // Goto parent's right and keep going
            parent = parent->right_node;
        } else { // Left
            // Goto parent's left and keep going
            parent = parent->left_node;
        }
    }
    return NULL;
}




