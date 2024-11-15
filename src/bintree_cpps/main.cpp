#include "../bintree_headers/callgame.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../bintree_headers/dump.h"
#include "../../stack_ded/stack_headers/colorised_output.h"




int main(const int argc, const char** argv)
{
    if(argc < 2)
    {
        fprintf(stderr, RED_TEXT("WRONG INPUT!\n\n"));
        print_akinator_instruction();
        return EXIT_FAILURE;
    }

    return call_game(argv[1]);
}