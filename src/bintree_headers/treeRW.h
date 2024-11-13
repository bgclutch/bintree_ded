#ifndef TREERW_H_
#define TREERW_H_

#include "../bintree_headers/bintree.h"
#include "../bintree_headers/akinator.h"


Akinator_Err create_data_buffer(char** buffer, Tree* tree, size_t* buffer_size);

size_t scope_checker(const char* buffer, const size_t buffer_size, size_t* tree_size);

Akinator_Err read_tree_from_file(Tree* tree);

char* find_word_begin(char* buffer, const char* bufend);

void create_new_node(Node** node, char* buffer, size_t* all_bytes);

Akinator_Err init_tree_nodes(Node* node, char* buffer, size_t* all_bytes);

size_t get_node_data_size(const char* word_beginning);

Akinator_Err write_tree_to_file(Node* root, const char* outpfilename);

void write_nodes_to_file(Node* node, FILE* file);

Akinator_Err write_tree_to_file(Node* root, FILE* file);

#endif // TREERW_H_