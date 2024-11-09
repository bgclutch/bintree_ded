#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "../bintree_headers/bintree.h"


Tree_Errors insert(Node* node, const NodeElem_t elem)
{
    NODE_ERR(init_free_node(node, elem, *(NodeElem_t*)node->data), NODE_FIND_ERR);

    // tree_print(node);
    // fprintf(stderr, "\n\n");

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


Tree_Errors node_init(Node** node, const NodeElem_t elem)
{
    assert(!*node);

    Node* new_node = (Node*)calloc(sizeof(Node), 1);

    new_node->left  = nullptr;
    new_node->right = nullptr;

    *node = new_node;

    elem_ctor(&(*node)->data, sizeof(NodeElem_t));

    if((*node)->left != nullptr || (*node)->right != nullptr)
    {
        return NODE_INIT_ERR;
    }

    memcpy((*node)->data, &elem, sizeof(elem));

    return NODE_IS_OKAY;
}


Tree_Errors init_free_node(Node* node, const NodeElem_t elem, const NodeElem_t comp_elem)
{
    assert(node);
    assert(node->data);

    if(comp_elem > elem)               // switch?
    {
        if(node->left == nullptr)
        {
            if(node_init(&node->left, elem) != NODE_IS_OKAY)
            {
                tree_branch_dtor(node);
                return NODE_INIT_ERR;
            }
        }
        else
        {
            if(init_free_node(node->left, elem, *(NodeElem_t*)node->left->data))
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
            NODE_ERR(node_init(&node->right, elem), NODE_INIT_ERR);
        }
        else
        {
            NODE_ERR(init_free_node(node->right, elem, *(NodeElem_t*)node->right->data), NODE_FIND_ERR);
        }
    }
    else
    {
        fprintf(stderr, "penis in init\n");
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

    fprintf(stderr, "(");

    if(node->left)
        tree_print(node->left);

    fprintf(stderr, "%d", *(NodeElem_t*)node->data);

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

    elem_dtor(node->data);
    free(node);

    return;
}