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
#include "../../stack_ded/stack_headers/colorised_output.h"


static char NOTSTRING[] = "!NOT!";


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
    node->left->parent = node;                               // move old answer

    char* new_answer_data = nullptr;
    fprintf(stderr, "\nplease, input your expected object(starts with you are... optionally):\n");
    size_t new_answer_size = 0;

    if(getline(&new_answer_data, &new_answer_size, stdin) == GETLINEERR)
        return NEWNODE_CRE_ERR;

    new_answer_size = strlen(new_answer_data) - 1;

    NODE_ERR(node_init(&node->right, new_answer_data, new_answer_size), NODE_INIT_ERR);
    node->right->parent = node;

    return NODE_IS_OKAY;
}


Tree_Errors change_recieved_leaf(Node** node)
{
    assert(node);

    if((*node)->left || (*node)->right)
        return NODE_IS_BRANCH;

    move_old_and_add_new_answer(*node);

    char* new_branch_data = nullptr;
    fprintf(stderr, "\nplease, input difference between wrong(old) and right(new) answers(w/o '?' sign and starts with 'you are' optionally):\n");
    size_t new_branch_size = 0;

    if(getline(&new_branch_data, &new_branch_size, stdin) == -1)
        return NEWNODE_CRE_ERR;

    new_branch_size = strlen(new_branch_data) - 1;

    NODE_ERR(edit_node(node, new_branch_data, new_branch_size), DENY_TO_EDIT);
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

    (*node)->data = new_data;
    (*node)->data_size = new_data_size;

    return NODE_IS_OKAY;
}


void gamestart(Node* root)
{
    Node** answer_node = (Node**)calloc(sizeof(Node*), 1);
    akinator_func(root, answer_node);
    assert(answer_node);

    fprintf(stderr, "\nam i right?");
    char* answer = get_user_answer();

    if(!strncmp(answer, YESANSWER, strlen(YESANSWER)))
    {
        fprintf(stderr, "i hope you enjoy being ");
        for(size_t i = 0; i < (*answer_node)->data_size; i++)
            fprintf(stderr, "%c", (*answer_node)->data[i]);
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

    free(answer);
    fprintf(stderr, "do you want to play again?\n");
    answer = get_user_answer();

    if(!strncmp(answer, YESANSWER, strlen(YESANSWER)))
    {
        free(answer);
        free(answer_node);
        gamestart(root);
    }
    else
    {
        free(answer);
        free(answer_node);
        fprintf(stderr, "good luck!\n");
    }

    return;
}


Akinator_Err comparestart(Node* root)
{
    assert(root);

    Main_Stack_Struct first = {};
    Main_Stack_Struct second = {};

    find_define();
    find_define();




}



Akinator_Err getdefine(Node* root)
{
    assert(root);

    fprintf(stderr, "input your word/sentense\n");
    char* chosen_word = nullptr;
    size_t chosen_word_size = 0;
    if(getline(&chosen_word, &chosen_word_size, stdin) == GETLINEERR)
    {
        free(chosen_word);
        return CHOSEN_WORD_GETLINE_ERR;
    }
    Main_Stack_Struct stack;
    ctor_stack(&stack);

    int retval = -1;

    find_define(root, chosen_word, &retval, &stack);

    if(retval == 0)
    {
        fprintf(stderr, "DEFINITION IS HERE\n");
        print_definition(&stack);
    }
    else
    {
        fprintf(stderr, "NO DEFINE TODAY\n");
    }

    fprintf(stderr, "do you want to play again?\n");
    char* answer = get_user_answer();

    if(!strncmp(answer, YESANSWER, strlen(YESANSWER)))
    {
        free(answer);
        getdefine(root);
    }
    else
    {
        free(answer);
        fprintf(stderr, "good luck!\n");
    }

    free(chosen_word);
    dtor_stack(&stack);
    return AKINATOR_STILL_ALIVE;
}


void find_define(Node* node, const char* word, int* retval, Main_Stack_Struct* stack)
{
    char* pop_elem = 0;

    stack_push(stack, node->data);

    if(strncmp(node->data, word, node->data_size) == 0)
    {
        *retval = 0;
        stack_pop(stack, &pop_elem);
    }

    if(!node->left && !node->right)
        return;

    if(*retval == 0)
    {
        return;
    }
    else
    {
        stack_push(stack, NOTSTRING);
        find_define(node->left, word, retval, stack);
    }
    if(*retval != 0)
    {
        stack_pop(stack, &pop_elem);
        stack_pop(stack, &pop_elem);
    }

    if(*retval == 0)
    {
        return;
    }
    else
    {
        find_define(node->right, word, retval, stack);
    }

     if(*retval != 0)
    {
        stack_pop(stack, &pop_elem);
    }

    return;
}

void print_definition(Main_Stack_Struct* stack)
{
    while(stack->size > 0)
    {
        char* elem = nullptr;
        stack_pop(stack, &elem);
        for(int i = 0; elem[i] != '}' && elem[i] != '{' && elem[i] != '\0'; i++)
            fprintf(stderr, BLUE_TEXT("%c"), elem[i]);
        fprintf(stderr, "\n");
    }
}


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