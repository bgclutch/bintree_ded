#include "../bintree_headers/akinator.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../bintree_headers/bintree.h"
#include "../bintree_headers/dump.h"
#include "../../lib_file_proc/file.h"
#include "../../lib_buffer_proc/buffer.h"


Akinator_Err akinator_is_err(const Akinator_Err result, const char* name, const size_t line)
{
    assert(name);

    if(result != AKINATOR_STILL_ALIVE)
    {
        fprintf(stderr, "Error with code:%d here: %s:%lu\n\n", result, name, line);
        return MACRO_AK_ERR;
    }

    return MACRO_AK_GOOD;
}


Tree_Errors move_old_and_add_new_answer(Node* node)
{
    assert(node);

    NODE_ERR(node_init(&node->left, node->data, node->data_size), NODE_INIT_ERR);
    node->left->parent = node;                           // move old answer

    char* new_answer_data = nullptr;
    fprintf(stderr, "\nplease, input your expected object:\n");
    ssize_t new_answer_size = getline(&new_answer_data, NULL, stdin);

    if(new_answer_size == GETLINEERR)
        return NEWNODE_CRE_ERR;

    NODE_ERR(node_init(&node->right, new_answer_data, (size_t)new_answer_size), NODE_INIT_ERR);
    node->right->parent = node;

    return NODE_IS_OKAY;
}


Tree_Errors change_recieved_leaf(Node* node)
{
    assert(node);

    if(node->left || node->right)
        return NODE_IS_BRANCH;

    move_old_and_add_new_answer(node);

    char* new_branch_data = nullptr;
    fprintf(stderr, "\nplease, input difference between wrong(old) and right(new) answers:\n");
    ssize_t new_branch_size = getline(&new_branch_data, NULL, stdin);

    if(new_branch_size == -1)
        return NEWNODE_CRE_ERR;

    NODE_ERR(edit_node(&node, new_branch_data, (size_t)new_branch_size), DENY_TO_EDIT);

    return NODE_IS_OKAY;
}


Tree_Errors edit_node(Node** node, const NodeElem_t new_data, const size_t new_data_size)
{
    assert(*node);
    assert((*node)->left);
    assert((*node)->right);

    if(*node != (*node)->left)
        return DENY_TO_EDIT;

    if(!(*node)->left || !(*node)->right)
        return DENY_TO_EDIT;

    memcpy((*node)->data, new_data, new_data_size);

    (*node)->data_size = new_data_size;

    return NODE_IS_OKAY;
}