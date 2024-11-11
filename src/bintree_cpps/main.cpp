#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../bintree_headers/bintree.h"
#include "../bintree_headers/dump.h"
#include "../bintree_headers/akinator.h"


int main(void)
{
    Tree tree = {};

    read_tree_from_file(&tree);

    Dump_St General_Dump = {};

    prepare_graphic_file(General_Dump);
    graphic_dump(tree.root, &General_Dump);
    close_graphic_dump(General_Dump);
    make_html_file(General_Dump.HTML_DUMP);
    dot_to_png(General_Dump.GRAPHIC_DUMP, &General_Dump);
    close_file_html(General_Dump.HTML_DUMP);

    tree_branch_dtor(tree.root);
    free(tree.buffer);


    return 0;
}