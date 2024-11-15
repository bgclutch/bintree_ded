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


static char NOTSTRING[] = "NOT";


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

    fprintf(stderr, "\nplease, input your expected object(w/o 'you are' ):\n");

    char* new_answer_data = get_user_sentence();
    if(!new_answer_data)
        return NEWNODE_CRE_ERR;

    size_t new_answer_size = strlen(new_answer_data) - 1;

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

    fprintf(stderr, "\nplease, input difference between wrong(old) and right(new) answers(w/o '?' and 'you are'):\n");

    char* new_branch_data = get_user_sentence();
    if(!new_branch_data)
        return NEWNODE_CRE_ERR;
    size_t new_branch_size = strlen(new_branch_data) - 1;
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
            fprintf(stderr, GREEN_TEXT("%c"), (*answer_node)->data[i]);
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

    if(!strncmp(answer, YESANSWER, strlen(YESANSWER)))  // copypaste(((
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

    fprintf(stderr, "input your " BLUE_TEXT("first ") "word/sentence\n");
    char* first  = get_user_sentence();
    fprintf(stderr, "input your " BLUE_TEXT("second ") "word/sentence\n");
    char* second = get_user_sentence();

    if(!first && !second)
    {
        free(first);
        free(second);
    }

    Main_Stack_Struct first_stack  = {};
    Main_Stack_Struct second_stack = {};

    ctor_stack(&first_stack);
    ctor_stack(&second_stack);

    Find_Res retval_first  = NOTFOUND;
    Find_Res retval_second = NOTFOUND;
    find_define(root, first,  &retval_first,  &first_stack);
    find_define(root, second, &retval_second, &second_stack);

    if(retval_first == NOTFOUND || retval_second == NOTFOUND)
    {
        fprintf(stderr, "NOTHING TO COMPARE\n");
    }
    else
    {
        // fprintf(stderr, "first definition:\n");
        // print_definition(&first_stack);
        // fprintf(stderr, "second definition:\n");
        // print_definition(&second_stack);
        compare_definitions_print(&first_stack, &second_stack);
    }

    free(first);
    free(second);
    dtor_stack(&first_stack);
    dtor_stack(&second_stack);

    fprintf(stderr, "do you want to play again?\n"); // copypaste(((
    char* answer = get_user_answer();

    if(!strncmp(answer, YESANSWER, strlen(YESANSWER)))
    {
        free(answer);
        comparestart(root);
    }
    else
    {
        free(answer);
        fprintf(stderr, "good luck!\n");
    }

    return AKINATOR_STILL_ALIVE;
}



Akinator_Err getdefine(Node* root)
{
    assert(root);

    fprintf(stderr, BLUE_TEXT("input your word/sentence:\n"));
    char* chosen_sent = get_user_sentence();

    Main_Stack_Struct stack;
    ctor_stack(&stack);

    Find_Res retval = NOTFOUND;
    find_define(root, chosen_sent, &retval, &stack);

    if(retval == FOUND)
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

    if(!strncmp(answer, YESANSWER, strlen(YESANSWER))) // copypaste(((
    {
        free(answer);
        getdefine(root);
    }
    else
    {
        free(answer);
        fprintf(stderr, "good luck!\n");
    }

    free(chosen_sent);
    dtor_stack(&stack);
    return AKINATOR_STILL_ALIVE;
}


void find_define(Node* node, const char* word, Find_Res* retval, Main_Stack_Struct* stack)
{
    char* pop_elem = 0;

    stack_push(stack, node->data);

    if(strncmp(node->data, word, node->data_size) == 0)
    {
        *retval = FOUND;
        stack_pop(stack, &pop_elem);
    }

    if(!node->left && !node->right)
        return;

    if(*retval == FOUND)
    {
        return;
    }
    else
    {
        stack_push(stack, NOTSTRING);
        find_define(node->left, word, retval, stack);
    }
    if(*retval == NOTFOUND)
    {
        stack_pop(stack, &pop_elem);
        stack_pop(stack, &pop_elem);
    }

    if(*retval == FOUND)
    {
        return;
    }
    else
    {
        find_define(node->right, word, retval, stack);
    }

    if(*retval == NOTFOUND)
    {
        stack_pop(stack, &pop_elem);
    }
}

void print_definition(Main_Stack_Struct* stack)
{
    while(stack->size > 0)
    {
        fprintf(stderr, "you are ");
        char* elem = nullptr;
        stack_pop(stack, &elem);
        for(int i = 0; elem[i] != '}' && elem[i] != '{' && elem[i] != '\0'; i++)
            fprintf(stderr, BLUE_TEXT("%c"), elem[i]);

        if(strcmp(elem, NOTSTRING) == 0)
            fprintf(stderr, " ");
        else
            fprintf(stderr, "\n");
    }
}


void akinator_func(Node* node, Node** answer_node)
{
    assert(node);
    assert(answer_node);

    fprintf(stderr, "you are ");
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
    fprintf(stderr, "["GREEN_TEXT("yes")"/"RED_TEXT("no")"]\n");
    char* userinput = get_user_sentence();
    if(!userinput)
        return nullptr;


    while(strncmp(userinput, YESANSWER, strlen(YESANSWER)) &&
          strncmp(userinput, NOANSWER,  strlen(NOANSWER)))
    {
        fprintf(stderr, "try again!\n");
        free(userinput);
        userinput = get_user_sentence();
        if(!userinput)
            return nullptr;
    }

    return userinput;
}


void clean_buffer()
{
    int bufsymb = 0;
    while((bufsymb = getchar()) != '\n');
}


char* get_user_sentence()
{
    char* sentence = nullptr;
    size_t size = 0;
    if(getline(&sentence, &size, stdin) == GETLINEERR)
    {
        free(sentence);
        return nullptr;
    }

    // for(size_t i = 0; i < size - 1; i++) // FIXME
    // {
    //     if(!isalnum(sentence[i]))
    //         return nullptr;
    // }

    return sentence;
}


void compare_definitions_print(Main_Stack_Struct* first, Main_Stack_Struct* second)
{
    assert(first);
    assert(second);

    char* first_elem  = nullptr;
    char* second_elem = nullptr;

    fprintf(stderr, "first:\tsecond:\n");

    while(first->size != 0 && second->size != 0)
    {
        if(first->size)
            stack_pop(first, &first_elem);
        else
            first_elem = nullptr;

        if(second->size)
            stack_pop(second, &second_elem);
        else
            second_elem = nullptr;

        Text_Colors color = BLUETEXT;

        if(first_elem)
        {
            if(first_elem == second_elem)
            {
                stack_elem_outp(first_elem, BLUETEXT);
                color = BLUETEXT;
            }
            else
            {
                stack_elem_outp(first_elem, GREENTEXT);
                color = GREENTEXT;
            }

            if(strcmp(first_elem, NOTSTRING) == 0 && first->size > 0)
            {
                fprintf(stderr, " ");
                stack_pop(first, &first_elem);
                stack_elem_outp(first_elem, color);
            }
        }

        fprintf(stderr, "\t");

        if(second_elem)
        {
            if(first_elem == second_elem)
            {
                // stack_elem_outp(second_elem, BLUETEXT);
                color = BLUETEXT;
            }
            else
            {
                stack_elem_outp(second_elem, REDTEXT);
                color = REDTEXT;
            }

            if(strcmp(second_elem, NOTSTRING) == 0 && second->size > 0)
            {
                fprintf(stderr, " ");
                stack_pop(second, &second_elem);
                stack_elem_outp(second_elem, color);
            }
        }

        fprintf(stderr, "\n");
    }
}


void stack_elem_outp(const StackElem_t elem, const Text_Colors color)
{
    switch(color)
    {
        case REDTEXT:
        {
            for(int i = 0; elem[i] != '}' && elem[i]  != '{' && elem[i]  != '\0'; i++)
                fprintf(stderr, RED_TEXT("%c"), elem[i]);
            break;
        }
        case BLUETEXT:
        {
            for(int i = 0; elem[i] != '}' && elem[i]  != '{' && elem[i]  != '\0'; i++)
                fprintf(stderr, BLUE_TEXT("%c"), elem[i]);
            break;
        }
        case GREENTEXT:
        {
            for(int i = 0; elem[i] != '}' && elem[i]  != '{' && elem[i]  != '\0'; i++)
                fprintf(stderr, GREEN_TEXT("%c"), elem[i]);
            break;
        }
        case YELLOWTEXT:
        {
            for(int i = 0; elem[i] != '}' && elem[i]  != '{' && elem[i]  != '\0'; i++)
                fprintf(stderr, YELLOW_TEXT("%c"), elem[i]);
            break;
        }
        case MAGENTATEXT:
        {
            for(int i = 0; elem[i] != '}' && elem[i]  != '{' && elem[i]  != '\0'; i++)
                fprintf(stderr, MAGENTA_TEXT("%c"), elem[i]);
            break;
        }
        default:
        {
            assert(0);
            break;
        }

    }
}