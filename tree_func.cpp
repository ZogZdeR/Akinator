#include "tree_func.hpp"


Node *CreateNode ( char *characteristic, Node *parent)
{
    Node *node = (Node *)calloc (1, sizeof (Node));
    node->characteristic = (char *)calloc ((size_t)(my_strlen (characteristic) + 1), sizeof (char));
    my_strcpy (characteristic, node->characteristic);
    node->right = NULL;
    node->left = NULL;
    node->parent = parent;
    return node;
}

void AskForAPerson (Node *node)
{
    int ans = '\\';
    if (node->left != NULL && node->right != NULL)
    {
        fprintf (stderr, CYAN "Is it %s?\n" NORMAL, node->characteristic);
        fprintf (stderr, GREEN "y(yes)/n(no)\n" NORMAL);
        ans = GetAnswer ();
        clear_buffer ();
        switch (ans)
        {
            case 'y':
                AskForAPerson (node->left);
                break;
            case 'n':
                AskForAPerson (node->right);
                break;
            default: assert (0);
        }
        
    }
    else 
    {
        fprintf (stderr, CYAN "Is it %s?\n" NORMAL, node->characteristic);
        fprintf (stderr, GREEN "y(yes)/n(no)\n" BLUE);
        ans = GetAnswer ();
        switch (ans)
        {
            case 'y':
                fprintf (stderr, GREEN "Congratulations! You have found your person\\character!\n" NORMAL);
                break;
            case 'n': {

                fprintf (stderr, MAGENTA "It's a pitty that I don't have your character in my database." 
                    "Tell me the name of the person\\character\n" BLUE);
                char *Name = (char *)calloc (30, sizeof (char));
                clear_buffer (); 
                my_getline (Name);
                Name = (char *)realloc (Name, sizeof (char) * ((size_t)my_strlen (Name) + 1));
                
                fprintf (stderr, MAGENTA "Tell me what is the difference between %s and %s\n" BLUE, 
                    node->characteristic, Name);
                char *new_characteristic = (char *)calloc (30, sizeof (char));
                my_getline (new_characteristic);
                // new_characteristic = (char *)realloc (new_characteristic, 
                //   sizeof (char) * ((size_t)my_strlen (new_characteristic) + 1));
                
                fprintf (stderr, MAGENTA "Does %s acquire this characteristic ?\n"
                    GREEN "Enter y(yes)\\n(no)\n" BLUE, node->characteristic);
                ans = GetAnswer ();
                switch (ans)
                {
                    case 'y':
                        node->left = CreateNode (node->characteristic, node);
                        node->right = CreateNode (Name, node);
                        my_strcpy (new_characteristic, node->characteristic);
                        break;
                    case 'n':
                        node->left = CreateNode (Name, node);
                        node->right = CreateNode (node->characteristic, node);
                        if (my_strlen (new_characteristic) > my_strlen (node->characteristic))
                            node->characteristic = (char *)realloc (node->characteristic, (size_t)my_strlen (new_characteristic) + 1);
                        my_strcpy (new_characteristic, node->characteristic);
                        
                        break;
                    default: assert (0);
                }
                free (Name);
                free (new_characteristic);
                break;
            }
            default: 
                assert (0);
                break;
        }
    }
}

char GetAnswer ()
{
    char ans = '\\';
    while (ans != 'y' && ans != 'n')
    {
        ans = (char)getchar ();
        if (ans != 'y' && ans != 'n') fprintf (stderr, 
                RED "You've written an invalid answer. Try again\n" NORMAL);
    }
    return ans;
}

void WriteNode (Node *node, FILE *stream)
{
    if (node == NULL) fprintf (stream, "nil");
    else
    {
        fprintf (stream, "(\"%s\"", node->characteristic);
        WriteNode (node->left, stream);
        WriteNode (node->right, stream);
        fprintf (stream, ")");
    }
}

Node *ReadNode (FILE *stream, char *buffer, size_t *pos, Node *parent)
{
    if (buffer[*pos] == '(')
    {
        (*pos) += 2; // Skipping '(' and ' " '
        char *data = (char *)calloc (30, sizeof (char));
        int delta = 0;
        sscanf (buffer, "\"" "%[^\"]" "\"%n", data, &delta);
        Node *new_node = CreateNode (data, parent);
        (*pos) += (size_t)delta; // skipping data
        (*pos) ++; // skipping ' " '
        new_node->left = ReadNode (stream, buffer, pos, new_node);
        new_node->right = ReadNode (stream, buffer, pos, new_node);
        (*pos)++;
        free (data);
        return new_node;
    }
}



void AkinDump (Node *node)
{
    FILE *dot_file_stream = fopen(graph_dot, "w"); // FIXME: to const
    assert (dot_file_stream);

    static size_t dump_quantity = 0;
    
    fprintf (dot_file_stream, "digraph {\n");
    fprintf (dot_file_stream, "nodestep = 1;\n");
    fprintf (dot_file_stream, "splines = ortho;\n");

    GraphVizNodeInitialise (dot_file_stream, node);

    fprintf (dot_file_stream, "\n");

    GraphVizRoadsInitialise (dot_file_stream, node);
    fprintf (dot_file_stream, "\n}");
       
    fclose(dot_file_stream );

    char *pattern = "dot -Tpng graph.dot -o graph%zu.png";
    char *some_buffer = (char *)calloc (system_buffer_size, sizeof (char));

    dump_quantity++;
    snprintf (some_buffer, system_buffer_size, pattern, dump_quantity);
    system(some_buffer);
    free (some_buffer);
}

void GraphVizNodeInitialise (FILE *stream, Node *node)
{
    if (node != NULL)
    {
        if (node->left != NULL) GraphVizNodeInitialise (stream, node->left);
        assert (node->characteristic);
        fprintf (stream, "\"Node%p\" [shape = record, label = \" { <f1>  %s | { <f2> Left | <f3> Right } } \"];\n ", 
            node, node->characteristic);
        if (node->right != NULL) GraphVizNodeInitialise (stream, node->right);
    }
}
void GraphVizRoadsInitialise (FILE *stream, Node *node)
{
    if (node != NULL)
    {
        if (node->left != NULL)GraphVizRoadsInitialise (stream, node->left);
        if (node->left != NULL)fprintf (stream, "\"Node%p\":<f2> -> \"Node%p\":<f1> [weight = 1, color = red]; \n", 
            node, node->left);
        if (node->right != NULL) fprintf (stream, "\"Node%p\":<f3> -> \"Node%p\":<f1> [weight = 1, color = red]; \n", 
            node, node->right);
        if (node->right != NULL)GraphVizRoadsInitialise (stream, node->right);
    }
}
void clear_buffer () 
{
    int c = '\0';
    do
    {
        c = getchar ();
    }
    while (c != '\n' and c != EOF);
}

void NodeDestructor (Node *node)
{
    if (node->left != NULL) NodeDestructor (node->left);
    if (node->right != NULL) NodeDestructor (node->right);
    if (node->parent != NULL)
    {
        if (node == node->parent->left) node->parent->left = NULL;
        if (node == node->parent->right) node->parent->right == NULL;
        free (node->characteristic);
        free (node);
    }
}