#ifndef BINTREE_H_
#define BINTREE_H_

#include <stdio.h>


#define NODE_ERR(result, error) if(tree_is_err(result, __FILE__, __LINE__) == MACRO_ERR) return error


typedef int NodeElem_t;

struct Node
{
    void* data;
    Node* left;
    Node* right;
};


struct Tree
{
    Node* root; // ctor elem && dtor elem


    // tree size
};


enum Tree_Errors
{
    NODE_IS_OKAY    = 0x00,
    NODE_CTOR_ERR   = 0x01,
    NODE_FIND_ERR   = 0x02,
    NODE_INIT_ERR   = 0x03,
    NODE_INSERT_ERR = 0x04,


    MACRO_ERR  = 0x20,
    MACRO_GOOD = 0x30,

};


Tree_Errors insert(Node* node, const NodeElem_t elem);

Tree_Errors init_free_node(Node* node, const NodeElem_t elem, const NodeElem_t comp_elem);

Tree_Errors node_init(Node** node, const NodeElem_t elem);

void tree_branch_dtor(Node* node);

Tree_Errors tree_is_err(const Tree_Errors result, const char* name, const size_t line);

void tree_print(Node* node);


void elem_ctor(void** elem, const size_t size);

void elem_dtor(void* elem);


#endif // BINTREE_H_