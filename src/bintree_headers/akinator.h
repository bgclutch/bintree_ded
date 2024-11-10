#ifndef AKINATOR_H_
#define AKINATOR_H_

#include "../../lib_buffer_proc/buffer.h"
#include "../../lib_file_proc/file.h"
#include "../bintree_headers/bintree.h"

#define AKN_ERR(result, error) akinator_is_err(result, __FILE__, __LINE__) return error


enum Akinator_Err
{
    AKINATOR_STILL_ALIVE           = 0x00,
    AKINATOR_IS_DEAD               = 0x01,
    AKINATOR_BUFFER_CTOR_ERR       = 0x02,
    AKINATOR_BUFFER_FILE_OPEN_ERR  = 0x03,
    AKINATOR_BUFFER_FILE_CLOSE_ERR = 0x04,


    MACRO_AK_GOOD = 0x20,
    MACRO_AK_ERR  = 0x30,

};

const char* const DATABASE = "src/database.txt";


Akinator_Err akinator_is_err(const Akinator_Err result, const char* name, const size_t line);

Akinator_Err get_data_buffer(char* buffer, Tree* tree, size_t* buffer_size);

int scope_checker(const char* buffer, const size_t buffer_size, Tree* tree);


#endif // AKINATOR_H_