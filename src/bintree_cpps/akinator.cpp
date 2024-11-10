#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "../bintree_headers/akinator.h"
#include "../bintree_headers/bintree.h"
#include "../bintree_headers/dump.h"
#include "../../lib_file_proc/file.h"
#include "../../lib_buffer_proc/buffer.h"


Akinator_Err get_data_buffer(char* buffer, Tree* tree, size_t* buffer_size)
{
    FILE* buffer_file = nullptr;

    if(file_read_open(&buffer_file, DATABASE) != ALL_GOOD_RET_F)
        return AKINATOR_BUFFER_FILE_OPEN_ERR;

    *buffer_size = symbols_number(buffer_file);

    char* buffer = buffer_create(*buffer_size, sizeof(char), buffer_file);

    if(!buffer)
    {
        if(file_close(buffer_file))
            return AKINATOR_BUFFER_FILE_CLOSE_ERR;

        return AKINATOR_BUFFER_CTOR_ERR;
    }

    int scope_difference = scope_checker(buffer, *buffer_size, tree);

    if(scope_difference != 0)
    {
        fprintf(stderr, "begin and end scopes difference: %d\n", scope_difference);
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


int scope_checker(const char* buffer, const size_t buffer_size, Tree* tree)
{
    size_t counter_scope_begin = 0;
    size_t counter_scope_end   = 0;

    for(size_t index_buffer = 0; index_buffer < buffer_size; index_buffer++)
    {
        if(buffer[index_buffer] == '{')
            counter_scope_begin++;

        if(buffer[index_buffer] == '}')
            counter_scope_end++;
    }

    tree->tree_size = counter_scope_begin;

    return counter_scope_begin - counter_scope_end;
}


Akinator_Err init_nodes(char* buffer)
{
    assert(buffer);

    Tree* tree = (Tree*)calloc(sizeof(Tree), 1);

    size_t buffer_size = 0;

    char* buffer = nullptr;

    AKN_ERR(get_data_buffer(buffer, tree, &buffer_size), AKINATOR_BUFFER_CTOR_ERR);

    Node* root_addr = tree->root;

    root_addr->left   = nullptr;
    root_addr->right  = nullptr;
    root_addr->parent = nullptr;

    const char* bufend = buffer + buffer_size;


    const char* root_question = find_word_begin(buffer, bufend);

    root_addr->data_size = strlen(root_question);


    elem_ctor(&root_addr->data, root_addr->data_size);


    if(init_tree(root_addr->left) != NODE_IS_OKAY)
    {

    }


    return AKINATOR_STILL_ALIVE;
}


const char* find_word_begin(const char* buffer, const char* bufend)
{
    size_t index_buffer = 0;

    while(buffer[index_buffer] == '{' || buffer[index_buffer] == '}' && buffer + index_buffer < bufend)
    {
        index_buffer++;
    }

    return buffer + index_buffer;
}


Akinator_Err init_tree()
{
    init_free_node()

}