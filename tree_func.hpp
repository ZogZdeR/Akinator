#include <stdio.h>
#include <stdlib.h>

#include "string_functions.hpp"
#include "colour.hpp"



/*
struct data_t
{
    char *characteristic;
    size_t size;
};
*/

struct Node 
{
    //data_t data;
    char* characteristic;
    Node *left;
    Node *right;
    Node *parent;
};

size_t const system_buffer_size = 35;
static char const *graph_dot = "graph.dot";

Node *CreateNode (char *characteristic, Node *parent);
void AskForAPerson (Node *node);
char GetAnswer ();
void WriteNode (Node *node, FILE *stream);
Node *ReadNode (FILE *stream, char *buffer, size_t *pos, Node *parent);
void AkinDump (Node *node);
void GraphVizNodeInitialise (FILE *stream, Node *node);
void GraphVizRoadsInitialise (FILE *stream, Node *node);
void clear_buffer ();
void NodeDestructor (Node *node);
