// Header file
#include "tree.h"

// Libraries
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
    void *value;
} TreeNode;


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
    TreeNode *new_node = alloc(sizeof(TreeNode) + value_size);
    if (new_node == NULL) {
        return NULL;
    }

    // Assign fields
    new_node->left_node = NULL;
    new_node->right_node = NULL;
    new_node->parent_node = partent;

    // initialize space for the value
    new_node->value = (void *)new_node + sizeof(TreeNode);

    // Copy value 'into' Node
    memcpy(new_node->value, value, value_size);

    return new_node;
}

typedef struct _Tree {
    const size_t elem_size;
    const Alloc alloc;
    const DeAlloc dealloc;
    const Comparator compare;
    TreeNode *root;
} Tree;


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

static uint64_t height(TreeNode *node) {
    if (node == NULL) {
        return 0;
    }
    return 1 + max(height(node->left_node), height(node->right_node));
}


/// Get where [node] is appended at [parent]
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
static Direction child_dir(TreeNode *node, TreeNode *parent) {
    if (parent == NULL) {
        return None;
    }
    if (node == parent->right_node) {
        return Right;
    }
    if (node == parent->left_node) {
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
///   New root of sub tree
static TreeNode *rotate(TreeNode *node, Direction dir) {
    // NULL checks
    if (node == NULL || node->right_node == NULL || node->left_node == NULL) {
        return NULL;
    }

    TreeNode *old_root = node, *new_root, *inner_grandchild;
    switch (dir) {
    case Left:
        new_root = node->right_node;
        inner_grandchild = new_root->left_node;
        new_root->left_node = old_root;
        old_root->right_node = inner_grandchild;
        break;
    case Right:
        new_root = node->left_node;
        inner_grandchild = new_root->right_node;
        new_root->right_node = old_root;
        old_root->left_node = inner_grandchild;
        break;
    case None:
        return NULL;
        break;
    }

    return new_root;
}


static int get_balance(TreeNode *node) {
    if (node == NULL) {
        return 0;
    }
    return height(node->right_node) - height(node->left_node);
}


static TreeNode *balance_node(TreeNode *node) {
    if (node == NULL) {
        return NULL;
    }
    const int balance_value = get_balance(node);
    if (balance_value <= -2) { // rotate right
        // check balance of child node
        int child_bal = get_balance(node->left_node);
        if (child_bal <= -1) { // Rotate left child inwards
            node->left_node = rotate(node->left_node, Right);
        }
        // Finally rotate the node itself
        return rotate(node, Right);
    } else if (balance_value >= 2) { // rotate left
        int child_bal = get_balance(node->right_node);
        if (child_bal >= 1) { // Rotate right child inwards
            node->right_node = rotate(node->right_node, Left);
        }
        // Finally rotate the node itself
        return rotate(node, Left);
    } else { 
        return node;
    }
}





void tree_insert(Tree *tree, void *value) {
    // Null cannot be inserted
    if (value == NULL) {
        return;
    }
    // This is the first insertion
    if (tree->root == NULL) {
        tree->root = node_init(value, tree->elem_size, NULL, tree->alloc);
        if (tree->root == NULL) { // This is actually not necessary
            return;
        }
        return;
    }
    // Find new parent node
    TreeNode *parent = tree->root;
    while (parent->value != NULL) {
        // Compare value and parent
        int compare_value = tree->compare(parent->value, value, tree->elem_size);

        if (compare_value > 0) { // Right
            // Can be inserted below parent
            if (parent->right_node == NULL) {
                parent->right_node = node_init(value, tree->elem_size, parent, tree->alloc);
                if (tree->root == NULL) { // This is actually not necessary
                    return;
                }
                return;
            } else {
                // Goto parent's right and keep going
                parent = parent->right_node;
            }
        } else { // Left
            // Can be inserted below parent
            if (parent->left_node == NULL) {
                parent->left_node = node_init(value, tree->elem_size, parent, tree->alloc);
                if (tree->root == NULL) { // This is actually not necessary
                    return;
                }
                return;
            } else {
                // Goto parent's left and keep going
                parent = parent->left_node;
            }
        }
    }
}
