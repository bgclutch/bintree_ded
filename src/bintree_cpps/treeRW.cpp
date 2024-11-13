#include "../bintree_headers/treeRW.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../bintree_headers/bintree.h"
#include "../bintree_headers/akinator.h"
#include "../../lib_buffer_proc/buffer.h"
#include "../../lib_file_proc/file.h"


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


size_t get_node_data_size(const char* word_beginning)
{
    size_t index = 0;

    for(; word_beginning[index] != '{' && word_beginning[index] != '}' && word_beginning[index] != '\0'; index++);

    return index;
}


Akinator_Err init_tree_nodes(Node* node, char* buffer, size_t* all_bytes)
{
    assert(node);
    assert(buffer);
    assert(all_bytes);

    create_new_node(&node->left, buffer, all_bytes);
    node->left->parent = node;

    create_new_node(&node->right, buffer, all_bytes);
    node->right->parent = node;

    return AKINATOR_STILL_ALIVE;
}


Akinator_Err write_tree_to_file(Node* root, const char* outputfilename)
{
    assert(root);
    assert(outputfilename);

    FILE* write_tree_file = nullptr;

    if(file_write_open(&write_tree_file, outputfilename) == FILE_W_OPEN_ERR)
        return AKINATOR_FILE_ERROR;

    char* buffer = nullptr;
    size_t current_pos = 0;
    write_tree_to_buffer(root, buffer, &current_pos, write_tree_file);

    //fwrite(buffer, current_pos, sizeof(char), write_tree_file);
    free(buffer);

    file_close(write_tree_file);

    return AKINATOR_STILL_ALIVE;
}


Akinator_Err write_tree_to_buffer(Node* root, char* buffer, size_t* current_pos, FILE* file) // FIXME WTF REALLOC
{
    assert(root);
    assert(!buffer);

    size_t prefix_sum = REALLOC_ADDITION + sizeof('\0');
    // buffer = (char*)calloc(root->data_size + prefix_sum, sizeof(char)); // root node to buffer (const instead of 3?)
    // buffer[(*current_pos)++] = '{';
    fprintf(file, "{");


    // strncpy(buffer + *current_pos, root->data, root->data_size);
    *current_pos += root->data_size;

    for(size_t i = 0; i < root->data_size; i++)
        fprintf(file, "%c", root->data[i]);

    write_nodes_to_buffer(root->left,  buffer, current_pos, &prefix_sum, file);
    write_nodes_to_buffer(root->right, buffer, current_pos, &prefix_sum, file);

    // buffer[(*current_pos)++] = '}';
    fprintf(file, "}");
    return AKINATOR_STILL_ALIVE;
}


void write_nodes_to_buffer(Node* node, char* buffer, size_t* current_pos, size_t* prefix_sum, FILE* file)
{
    assert(node);
    //assert(buffer);
    assert(current_pos);
    assert(prefix_sum);

    fprintf(file, "{");
    *prefix_sum += REALLOC_ADDITION;
    // fprintf(stderr, "realloc size:%lu\n", (*prefix_sum + *current_pos + node->data_size));
    // fprintf(stderr, "node:%p node left:%p node right:%p", node, node->left, node->right);
    // fprintf(stderr, "word size:%lu cur pos:%lu prefix sum:%lu\n", node->data_size, *current_pos, *prefix_sum);
    // fprintf(stderr, "buffer %p\n", buffer);
    // fprintf(stderr, "%s %lu\n", buffer, strlen(buffer));

    // char* new_buffer = (char*)realloc(buffer, *current_pos + node->data_size + *prefix_sum);

        // if(new_buffer)
        //     buffer = new_buffer;
        // else
        //     assert(0);


    // memset(buffer + *current_pos + node->data_size, '\0', *prefix_sum);

    for(size_t i = 0; i < node->data_size; i++)
        fprintf(file, "%c", node->data[i]);

    // buffer[(*current_pos)++] = '{';

    // memcpy(buffer + *current_pos, node->data, node->data_size);
    *current_pos += node->data_size;

    if(!node->left && !node->right)
    {
        // buffer[(*current_pos)++] = '}';
        fprintf(file, "}");

        return;
    }
    else
    {
        write_nodes_to_buffer(node->left,  buffer, current_pos, prefix_sum, file);
        write_nodes_to_buffer(node->right, buffer, current_pos, prefix_sum, file);
    }

    fprintf(file, "}");
    //buffer[(*current_pos)++] = '}';
    return;
}