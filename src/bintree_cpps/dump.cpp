#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/bintree_headers/dump.h"
#include "../../src/bintree_headers/bintree.h"


Dump_Errors prepare_graphic_file(const Dump_St General_Dump)
{
    FILE* graph_dump_file = fopen(General_Dump.GRAPHIC_DUMP, "w");


    if(graph_dump_file == nullptr)
        return DUMP_FILE_OPEN_ERR;

    fprintf(graph_dump_file,
            "digraph LIST{\n"
            "node123456789 [label=\" \", shape=box, imagescale=true, image=\"%s\"]\n"
            "node1234567890 [label=\"ПОГОДИТЕ, ЭТО РЕАЛЬНО?\", textsize=40, textstyle=\"bold\", shape=\"octagon\", style=\"filled\", fillcolor=\"lightblue\"]"
            "node1234567890 -> node123456789\n"
            "harset = \"UTF-8\";\n"
            "node [shape=plaintext];\n"
            "bgcolor = \"#0000aa\";\n"
            "fontcolor = black;\n"
            "fontsize = 18;\n"
            "style = \"italic\";\n"
            "margin = 1;\n"
            "ranksep = 1;\n"
            "nodesep = 3;\n"
            "bgcolor = \"#aedeb0\";\n", DMPPIC);

    if(fclose(graph_dump_file))
        return DUMP_FILE_CLOSE_ERR;

    return DUMP_IS_OKAY;
}


void graphic_dump(Node* node, Dump_St* General_Dump)
{
    FILE* graph_dump_file = fopen(General_Dump->GRAPHIC_DUMP, "a+");

    if(graph_dump_file == nullptr)
        assert(0);

    if(node->left)
    {
        fill_file_with_number(graph_dump_file, node->left);

        fprintf(graph_dump_file, "\"%p\" -> \"%p\"[label=\"NO\"];\n", node, node->left);

        if(fclose(graph_dump_file))
            assert(0);

        graphic_dump(node->left, General_Dump);

    }
    else
    {
        fill_file_with_null(graph_dump_file, &node->left);

    fprintf(graph_dump_file, "\"%p\" -> \"%p\";\n", node, &node->left);

        if(fclose(graph_dump_file))
            assert(0);
    }


    graph_dump_file = fopen(General_Dump->GRAPHIC_DUMP, "a+");

    if(graph_dump_file == nullptr)
        assert(0);


    if(node->right)
    {
        fill_file_with_number(graph_dump_file, node->right);

        fprintf(graph_dump_file, "\"%p\" -> \"%p\"[label=\"YES\"];\n", node, node->right);


        if(fclose(graph_dump_file))
            assert(0);

        graphic_dump(node->right, General_Dump);
    }
    else
    {
        fill_file_with_null(graph_dump_file, &node->right);

        fprintf(graph_dump_file, "\"%p\" -> \"%p\";\n", node, &node->right);


        if(fclose(graph_dump_file))
            assert(0);
    }

    graph_dump_file = fopen(General_Dump->GRAPHIC_DUMP, "a+");

    if(graph_dump_file == nullptr)
        assert(0);

    fill_file_with_number(graph_dump_file, node);


    if(fclose(graph_dump_file))
            assert(0);

    return;
}


void close_graphic_dump(const Dump_St General_Dump)
{
    FILE* graph_dump_file = fopen(General_Dump.GRAPHIC_DUMP, "a+");

    if(graph_dump_file == nullptr)
        assert(0);

    fprintf(graph_dump_file, "}");

    if(fclose(graph_dump_file))
        assert(0);
}


Dump_Errors dump_is_err(Dump_Errors result, const char* name, const size_t line)
{
    assert(name);

    if(result != DUMP_IS_OKAY)
    {
        fprintf(stderr, "Error with code:%d here: %s:%lu\n\n", result, name, line);
        return MACRO_DUMP_ERR;
    }

    return MACRO_DUMP_GOOD;
}


void fill_file_with_number(FILE* graph_dump_file, Node* node)
{
    if(node->left == node->right)
    {
        print_to_dump_file(node, graph_dump_file, LEAF_COLOR);
    }
    else if(node->parent != nullptr && node->left != node->right)
    {
        print_to_dump_file(node, graph_dump_file, BRANCH_COLOR);
    }
    else
    {
        print_to_dump_file(node, graph_dump_file, ROOT_COLOR);
    }
}


void fill_file_with_null(FILE* graph_dump_file, void* node)
{
   fprintf(graph_dump_file, "\"%p\" [style = \"filled\", fillcolor = \"#ff9191\", label=<\n"
                             "<table border = \"0\" cellspacing=\"2\" cellpadding=\"4\">\n"
                             "<tr><td>NULL</td></tr></table>>];\n\n",
                             node);
}


void dot_to_png(const char* name, Dump_St* General_Dump)
{
    assert(name);

    size_t pngname_size = strlen(PATH) + strlen(GRAPH) + sizeof(General_Dump->filenum) + strlen(PNGXT);

    char* pngname = (char*)calloc(pngname_size, sizeof(char));

    snprintf(pngname, pngname_size, "%s%s%d%s", PATH, GRAPH, General_Dump->filenum, PNGXT);

    size_t dtpng_size = strlen(DOT) + strlen(SPACE) * 4 + strlen(name) + strlen(TPNG) + pngname_size + sizeof(OBJXT);

    char* dot_to_png_command = (char*)calloc(dtpng_size, sizeof(char));

    snprintf(dot_to_png_command, dtpng_size, "%s%s%s%s%s%s%s%s%s", DOT, SPACE, name, SPACE, TPNG, SPACE, OBJXT,
             SPACE, pngname);

    system(dot_to_png_command);

    fill_file_html(General_Dump->HTML_DUMP, pngname + sizeof(PATH) + 3);

    General_Dump->filenum++;

    free(pngname);
    free(dot_to_png_command);
}

void make_html_file(const char* filename)
{
    FILE* dump_file = fopen(filename, "w");

    fprintf(dump_file,
            "<!DOCTYPE html>\n"
            "<html lang=\"en\">\n\n"
            "<head>\n"
            "\t<meta charset=\"utf-8\">\n"
            "\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=\">\n"
            "\t<title>list dump</title>\n"
            "\t<link rel=\"stylesheet\" href=\"styles.css\">\n"
            "</head>\n\n"
            "<body>\n\n");

    fclose(dump_file);
}


void fill_file_html(const char* filename, const char* pngname)
{
    FILE* dump_file = fopen(filename, "a+");
    fprintf(dump_file,
            "<img src=\"%s\"/>\n",
            pngname);

    fclose(dump_file);
}


void close_file_html(const char* filename)
{
    FILE* dump_file = fopen(filename, "a+");
    fprintf(dump_file,
            "</body>\n\n"
            "</html>\n");

    fclose(dump_file);
}


void print_data_string(const char* data, const size_t data_size, FILE* dump_file)
{
    for(size_t i = 0; i < data_size; i++)
        fputc(data[i], dump_file);
}


void print_to_dump_file(const Node* node, FILE* dump_file, const Colors color)
{

    fprintf(dump_file, "\"%p\" [style = \"filled\", fillcolor = ", node);

    switch (color)
    {
        case ROOT_COLOR:
            fprintf(dump_file, "\"#ced7ea\"");
            break;

        case BRANCH_COLOR:
            fprintf(dump_file, "\"#b18b62\"");
            break;

        case LEAF_COLOR:
            fprintf(dump_file, "\"lightgreen\"");
            break;

        default:
        {
            assert(0);
        }
    }

    fprintf(dump_file, ",label=<\n"
                       "<table border=\"1\" cellspacing=\"0\" cellpadding=\"4\">\n"
                       "<tr><td colspan=\"2\">Address: %p</td></tr>\n"
                       "<tr><td colspan=\"2\">", node);

    print_data_string(node->data, node->data_size, dump_file);

    fprintf(dump_file, "</td></tr>\n"
                       "<tr><td align = \"center\" >Left:%p</td><td align = \"center\" >Right:%p</td></tr></table>>];\n\n",
                       node->left, node->right);
}