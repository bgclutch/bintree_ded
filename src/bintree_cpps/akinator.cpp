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
    clean_buffer();
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
    clean_buffer();
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


void gamestart(Node* root)
{
    Node* answer_node = (Node*)calloc(sizeof(Node), 1);

    akinator_func(root, answer_node);
    assert(answer_node);

    fprintf(stderr, "\nam i right?");
    Akinator_Answer answers = get_simple_answer();

    if(!answers.yesanswer)
    {
        fprintf(stderr, "i hope you enjoy being ");
        for(size_t i = strlen("you are "); i < answer_node->data_size; i++)
            fprintf(stderr, "%c", answer_node->data[i]);
        fprintf(stderr, "!\n");
    }
    else if(!answers.noanswer)
    {
        change_recieved_leaf(answer_node);
        fprintf(stderr, "thank you! now i know more about matvey\n");
    }
    else
    {
        assert(0);
    }

    fprintf(stderr, "do you want to play again?\n");
    Akinator_Answer continue_or_finish = get_simple_answer();

    if(!continue_or_finish.yesanswer)
        gamestart(root);
    else
        fprintf(stderr, "good luck!\n");

    free(answer_node);

    return;
}


// Akinator_Err comparestart(Node* root)
// {

// }


// Akinator_Err getdefine(Node* root)
// {

// }


void akinator_func(Node* node, Node* answer_node)
{

    for(size_t i = 0; i < node->data_size; i++)
        fprintf(stderr, "%c", node->data[i]);

    if(!node->left && !node->right)
    {
        answer_node = node;
        return;
    }

    Akinator_Answer answers = get_simple_answer();

    if(!answers.yesanswer)
        akinator_func(node->right, answer_node);
    else if(!answers.noanswer)
        akinator_func(node->left, answer_node);
    else // default
        assert(0);

    return;
}


Akinator_Answer get_simple_answer()
{
    char ans = '0';
    char* answer = &ans;
    memcpy(&answer, ANSPTR, sizeof(char));
    fprintf(stderr, "\ninput 1 symbol[y/n]\n");
    clean_buffer();
    getline(&answer, NULL, stdin);
    Akinator_Answer answers = {};
    fprintf(stderr, "yes %s// answ %d\n", YESANSWER, *answer);

    answers.yesanswer = strncmp(answer, YESANSWER, strlen(YESANSWER));
    answers.noanswer  = strncmp(answer, NOANSWER,  strlen(NOANSWER));
    fprintf(stderr, "y %d n %d\n", answers.yesanswer, answers.noanswer);

    while((answers.yesanswer = strncmp(answer, YESANSWER, strlen(YESANSWER)))  != 0 ||
          (answers.noanswer  = strncmp(answer, NOANSWER,  strlen(NOANSWER)))   != 0)
    {
        printf("try again!\n");
        clean_buffer();
        getline(&answer, NULL, stdin);
        fprintf(stderr, "yes %s// answ %d\n", YESANSWER, *answer);

    }
    return answers;
}


void clean_buffer()
{
    int bufsymb = 0;
    while((bufsymb = getchar()) != '\n');
}