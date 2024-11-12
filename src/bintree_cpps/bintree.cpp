#include "../bintree_headers/bintree.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "../../lib_file_proc/file.h"
#include "../../lib_buffer_proc/buffer.h"

Tree_Errors insert(Node* node, NodeElem_t elem)
{
    int node_counter = 0;

    if(init_free_node(node, elem, *(NodeElem_t*)node->data, &node_counter) != NODE_IS_OKAY)
    {
        tree_branch_dtor(node);
        return NODE_INSERT_ERR;
    }

    return NODE_IS_OKAY;
}


void elem_ctor(void** elem, const size_t size)
{
    *elem = calloc(size, sizeof(char));

    assert(*elem);
}

void elem_dtor(void* elem)
{
    free(elem);
    elem = nullptr;
}


Tree_Errors node_init(Node** node, const NodeElem_t elem, const size_t elem_size)
{
    assert(!*node);

    Node* new_node = (Node*)calloc(sizeof(Node), 1);
    new_node->left  = nullptr;
    new_node->right = nullptr;

    *node = new_node;
    (*node)->data = elem;
    (*node)->data_size = elem_size;

    if((*node)->left != nullptr || (*node)->right != nullptr)
    {
        return NODE_INIT_ERR;
    }

    return NODE_IS_OKAY;
}


Tree_Errors init_free_node(Node* node, const NodeElem_t elem, const NodeElem_t comp_elem, int* node_counter)
{
    assert(node);
    assert(node->data);

    if(comp_elem >= elem)               // switch?
    {
        if(node->left == nullptr)
        {
            if(node_init(&node->left, elem, strlen(elem)) != NODE_IS_OKAY) // FIXME STRLEN ELEM WHAT
            {
                tree_branch_dtor(node);
                return NODE_INIT_ERR;
            }
            (*node_counter)++;
            memcpy(&node->left->parent, (void*)node, sizeof(Node*));
        }
        else
        {
            if(init_free_node(node->left, elem, *(NodeElem_t*)node->left->data, node_counter))
            {
                tree_branch_dtor(node);
                return NODE_INIT_ERR;
            }
        }
    }
    else if(comp_elem < elem)
    {
        if(node->right == nullptr)
        {
           if(node_init(&node->right, elem, strlen(elem)) != NODE_IS_OKAY) //FIXME STRLEN ELEM WHAT
            {
                tree_branch_dtor(node);
                return NODE_INIT_ERR;
            }
            (*node_counter)++;
            memcpy(&node->right->parent, (void*)node, sizeof(Node*));
        }
        else
        {
           if(init_free_node(node->right, elem, *(NodeElem_t*)node->right->data, node_counter))
            {
                tree_branch_dtor(node);
                return NODE_INIT_ERR;
            }
        }
    }
    else
    {
        fprintf(stderr, "undefined action in init\n");
        return NODE_INIT_ERR;
    }

    return NODE_IS_OKAY;
}


Tree_Errors tree_is_err(const Tree_Errors result, const char* name, const size_t line)
{
    assert(name);

    if(result != NODE_IS_OKAY)
    {
        fprintf(stderr, "Error with code:%d here: %s:%lu\n\n", result, name, line);
        return MACRO_ERR;
    }

    return MACRO_GOOD;
}


void tree_print(Node* node)
{
    if(!node) return;
    assert(node->data);

    if(node->left)
        tree_print(node->left);

    fprintf(stderr, "(%c", *node->data);

    if(node->right)
        tree_print(node->right);

    fprintf(stderr, ")");
}


void tree_branch_dtor(Node* node)
{
    if(node->left != nullptr)
    {
        tree_branch_dtor(node->left);
    }
    if(node->right != nullptr)
    {
        tree_branch_dtor(node->right);
    }

    free(node);

    return;
}