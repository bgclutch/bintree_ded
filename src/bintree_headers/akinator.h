#ifndef AKINATOR_H_
#define AKINATOR_H_

#include "../../lib_buffer_proc/buffer.h"
#include "../../lib_file_proc/file.h"
#include "../bintree_headers/bintree.h"
#include "../../stack_ded/stack_headers/stack.h"


#define AKN_ERR(result, error) if(akinator_is_err(result, __FILE__, __LINE__) == MACRO_AK_ERR) return error


enum Akinator_Err
{
    AKINATOR_STILL_ALIVE           = 0x00,
    AKINATOR_IS_DEAD               = 0x01,
    AKINATOR_BUFFER_CTOR_ERR       = 0x02,
    AKINATOR_BUFFER_FILE_OPEN_ERR  = 0x03,
    AKINATOR_BUFFER_FILE_CLOSE_ERR = 0x04,
    AKINATOR_FILE_ERROR            = 0x05,
    CHOSEN_WORD_GETLINE_ERR        = 0x06,


    MACRO_AK_GOOD = 0x20,
    MACRO_AK_ERR  = 0x30,

};

struct Akinator_Answer
{
    int yesanswer = -1;
    int noanswer  = -1;
};

const char* const GAMEMODE     = "--play";
const char* const COMPAREMODE  = "--compare";
const char* const GETDEFINE    = "--define";
const char* const INSTRUCTIONS = "-h";

const char* const YESANSWER = "yes";
const char* const NOANSWER  = "no";

const ssize_t GETLINEERR = -1;


Akinator_Err akinator_is_err(const Akinator_Err result, const char* name, const size_t line);

Tree_Errors move_old_and_add_new_answer(Node* node);

Tree_Errors edit_node(Node** node, const NodeElem_t new_data, const size_t new_data_size);

Tree_Errors change_recieved_leaf(Node** node);

void gamestart(Node* root);

void akinator_func(Node* node, Node** answer_node);

void find_define(Node* node, const char* word, int* retval, Main_Stack_Struct* stack);

Akinator_Err getdefine(Node* root);

void print_definition(Main_Stack_Struct* stack);

char* get_user_answer();

void clean_buffer();

#endif // AKINATOR_H_