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


Akinator_Err create_data_buffer(char** buffer, Tree* tree, size_t* buffer_size)
{
    FILE* buffer_file = nullptr;

    if(file_read_open(&buffer_file, DATABASE) != ALL_GOOD_RET_F)
        return AKINATOR_BUFFER_FILE_OPEN_ERR;

    *buffer_size = symbols_number(buffer_file);

    *buffer = buffer_create(*buffer_size, sizeof(char), buffer_file);

    if(!*buffer)
    {
        if(file_close(buffer_file))
            return AKINATOR_BUFFER_FILE_CLOSE_ERR;

        return AKINATOR_BUFFER_CTOR_ERR;
    }

    size_t scope_difference = scope_checker(*buffer, *buffer_size, &tree->tree_size);

    if(scope_difference != 0)
    {
        fprintf(stderr, "begin and end scopes difference: %lu\n", scope_difference);
        free(buffer);
        if(file_close(buffer_file))
            return AKINATOR_BUFFER_FILE_CLOSE_ERR;
    }

    if(file_close(buffer_file))
        return AKINATOR_BUFFER_FILE_CLOSE_ERR;


    return AKINATOR_STILL_ALIVE;
}


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


Tree_Errors add_new_answer()
{





    return NODE_IS_OKAY;
}


Tree_Errors move_old_answer()
{





    return NODE_IS_OKAY;
}


size_t scope_checker(const char* buffer, const size_t buffer_size, size_t* tree_size)
{
    assert(buffer);


    size_t counter_scope_begin = 0;
    size_t counter_scope_end   = 0;

    for(size_t index_buffer = 0; index_buffer < buffer_size; index_buffer++)
    {
        if(buffer[index_buffer] == '{')
            counter_scope_begin++;

        if(buffer[index_buffer] == '}')
            counter_scope_end++;
    }

    if(tree_size != nullptr)
        *tree_size = counter_scope_begin;

    return counter_scope_begin - counter_scope_end;
}


Akinator_Err read_tree_from_file(Tree* tree)
{
    assert(tree);

    size_t buffer_size = 0;
    char* buffer = nullptr;

    if(create_data_buffer(&buffer, tree, &buffer_size) != AKINATOR_STILL_ALIVE)
    {
        free(buffer);
        return AKINATOR_BUFFER_CTOR_ERR;
    }

    tree->buffer = buffer;

    const char* bufend = buffer + buffer_size;
    char* root_question = find_word_begin(buffer, bufend);

    size_t all_bytes = 0;

    size_t root_question_size = get_node_data_size(root_question);
    all_bytes += root_question_size;

    node_init(&tree->root, root_question, root_question_size);

    Node* root_addr = tree->root;

    root_addr->left   = nullptr;
    root_addr->right  = nullptr;
    root_addr->parent = nullptr;

    if(init_tree_nodes(root_addr, buffer, &all_bytes) != AKINATOR_STILL_ALIVE)
    {
        free(buffer);
        return AKINATOR_IS_DEAD;
    }

    return AKINATOR_STILL_ALIVE;
}


char* find_word_begin(char* buffer, const char* bufend)
{
    size_t index_buffer = 0;

    for(; !isalpha(buffer[index_buffer]) && buffer + index_buffer < bufend; index_buffer++);
    return buffer + index_buffer;
}


void create_new_node(Node** node, char* buffer, size_t* all_bytes)
{
    assert(!*node);
    assert(buffer);
    assert(all_bytes);

    const char* bufend = buffer + strlen(buffer);
    char* word_begin = find_word_begin(buffer + *all_bytes, bufend);

    size_t elem_size = get_node_data_size(word_begin);

    *all_bytes += (size_t)(word_begin - (buffer + *all_bytes));
    *all_bytes += elem_size;

    node_init(node, word_begin, elem_size);

    if(*(buffer + *all_bytes) == '}')
    {
        return;
    }
    else
    {
        create_new_node(&(*node)->left, buffer, all_bytes);
        (*node)->left->parent = node;

        create_new_node(&(*node)->right, buffer, all_bytes);
        (*node)->right->parent = node;
    }

    return;
}


Akinator_Err init_tree_nodes(Node* node, char* buffer, size_t* all_bytes)
{
    assert(node);
    assert(buffer);
    assert(all_bytes);

    create_new_node(&node->left, buffer, all_bytes);
    node->left->parent = node;

    create_new_node(&node->right, buffer, all_bytes);
    node->left->parent = node;

    return AKINATOR_STILL_ALIVE;
}


size_t get_node_data_size(const char* word_beginning)
{
    size_t index = 0;

    for(; word_beginning[index] != '{' && word_beginning[index] != '}' && word_beginning[index] != '\0'; index++);

    return index;
}