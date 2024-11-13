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

    fprintf(stderr, "moldata:%p\n", node->left->data);

    char* new_answer_data = nullptr;
    fprintf(stderr, "\nplease, input your expected object:\n");
    size_t new_answer_size = 0;

    if(getline(&new_answer_data, &new_answer_size, stdin) == GETLINEERR)
        return NEWNODE_CRE_ERR;

    new_answer_size = strlen(new_answer_data) - 1;

    NODE_ERR(node_init(&node->right, new_answer_data, new_answer_size), NODE_INIT_ERR);
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
    fprintf(stderr, "\nplease, input difference between wrong(old) and right(new) answers(w/o '?' sign):\n");
    size_t new_branch_size = 0;

    if(getline(&new_branch_data, &new_branch_size, stdin) == -1)
        return NEWNODE_CRE_ERR;

    new_branch_size = strlen(new_branch_data) - 1;

    NODE_ERR(edit_node(&node, new_branch_data, new_branch_size), DENY_TO_EDIT);
    return NODE_IS_OKAY;
}


Tree_Errors edit_node(Node** node, const NodeElem_t new_data, const size_t new_data_size)
{
    assert(*node);
    assert((*node)->left);
    assert((*node)->right);

    if((*node)->data != (*node)->left->data)
        return DENY_TO_EDIT;

    if(!(*node)->left || !(*node)->right)
        return DENY_TO_EDIT;

    fprintf(stderr, "edit node:%p\n", (*node)->data);
    (*node)->data = new_data;
    (*node)->data_size = new_data_size;

    return NODE_IS_OKAY;
}


void gamestart(Node* root)
{
    Node* answer_node = (Node*)calloc(sizeof(Node), 1);

    akinator_func(root, &answer_node);
    assert(answer_node);

    fprintf(stderr, "\nam i right?");
    char* answer = get_user_answer();

    if(!strncmp(answer, YESANSWER, strlen(YESANSWER)))
    {
        fprintf(stderr, "i hope you enjoy being ");
        for(size_t i = strlen("you are "); i < answer_node->data_size; i++)
            fprintf(stderr, "%c", answer_node->data[i]);
        fprintf(stderr, "!\n");
    }
    else if(!strncmp(answer, NOANSWER, strlen(NOANSWER)))
    {
        change_recieved_leaf(answer_node);
        fprintf(stderr, "thank you! now i know more about matvey\n");
    }
    else
    {
        assert(0);
    }

    fprintf(stderr, "do you want to play again?\n");
    answer = get_user_answer();

    if(!strncmp(answer, YESANSWER, strlen(answer)))
    {
        free(answer);
        free(answer_node);
        gamestart(root);
    }
    else
    {
        free(answer);
        fprintf(stderr, "good luck!\n");
    }

    return;
}


// Akinator_Err comparestart(Node* root)
// {

// }


// Akinator_Err getdefine(Node* root)
// {

// }


void akinator_func(Node* node, Node** answer_node)
{
    assert(node);
    assert(answer_node);

    for(size_t i = 0; i < node->data_size; i++)
        fprintf(stderr, "%c", node->data[i]);

    if(!node->left && !node->right)
    {
        *answer_node = node;
        return;
    }

    char* answer = get_user_answer();

    if(!strncmp(answer, YESANSWER, strlen(YESANSWER)))
    {
        free(answer);
        akinator_func(node->right, answer_node);
    }
    else if(!strncmp(answer, NOANSWER, strlen(NOANSWER)))
    {
        free(answer);
        akinator_func(node->left, answer_node);
    }
    else // default
    {
        assert(0);
    }

    return;
}


char* get_user_answer()
{
    fprintf(stderr, "[yes/no]\n");
    char* userinput = nullptr;
    size_t userinputsize = 0;

    if(getline(&userinput, &userinputsize, stdin) == GETLINEERR)
        return nullptr;

    while(strncmp(userinput, YESANSWER, strlen(YESANSWER)) &&
          strncmp(userinput, NOANSWER,  strlen(NOANSWER)))
    {
        fprintf(stderr, "try again!\n");
        if(getline(&userinput, &userinputsize, stdin) == GETLINEERR)
            return nullptr;
    }

    return userinput;
}


void clean_buffer()
{
    int bufsymb = 0;
    while((bufsymb = getchar()) != '\n');
}