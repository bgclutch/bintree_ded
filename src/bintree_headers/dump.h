#ifndef DUMP_H_
#define DUMP_H_

#include <stdio.h>

#include "../../src/bintree_headers/dump.h"
#include "../../src/bintree_headers/bintree.h"


static const char* const PATH   = "dumps/html/";
static const char* const GRAPH  = "graph_dump";
static const char* const DOT    = "dot";
static const char* const DOTXT  = ".dot";
static const char* const SPACE  = " ";
static const char* const OBJXT  = "-o";
static const char* const HTMLXT = ".html";
static const char* const PNGXT  = ".png";
static const char* const TPNG   = "-Tpng";
static const char* const DMPPIC = "dumps/html/dump_monya.png";



#define DUMP_ERR(result, error) if(dump_is_err(result, __FILE__, __LINE__) == MACRO_ERR) return error

struct Dump_St
{
    const char* const GRAPHIC_DUMP = "dumps/graphic_dump.dot";
    const char* const HTML_DUMP    = "dumps/html/html_dump.html";
    int filenum = 0;
};


enum Dump_Errors
{
    DUMP_IS_OKAY        = 0x00,
    DUMP_FILE_OPEN_ERR  = 0x01,
    DUMP_FILE_CLOSE_ERR = 0x02,

    MACRO_DUMP_GOOD = 0x20,
    MACRO_DUMP_ERR  = 0x30,
};



Dump_Errors dump_is_err(Dump_Errors result, const char* name, const size_t line);

void graphic_dump(Node* root, Dump_St* General_Dump);

Dump_Errors prepare_graphic_file(const Dump_St General_Dump);

void close_graphic_dump(const Dump_St General_Dump);

void fill_file_with_number(FILE* graph_dump_file, Node* node);

void fill_file_with_null(FILE* graph_dump_file, void* node);


void dot_to_png(const char* name, Dump_St* General_Dump);

void make_html_file(const char* filename);

void fill_file_html(const char* filename, const char* pngname);

void close_file_html(const char* filename);

#endif // DUMP_H_