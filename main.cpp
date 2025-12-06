#include "tree_func.hpp"
#include "extraction.hpp"

int main ()
{
    FILE *repository = fopen ("repository.txt", "r");
    size_t pos = 0;
    char *buffer = (char*)calloc (file_length (repository) + 1, 1);
    file_to_buffer (repository, buffer);
    Node *node_ptr = ReadNode (repository, buffer, &pos, NULL); 
    fclose (repository);
    int continue_flag = 'y';
    free (buffer);
    while (continue_flag == 'y')
    {
        AskForAPerson (node_ptr);
        // clear_buffer ();
        fprintf (stderr, GREEN "If you wanna continue Akinator prigramm? Enter y(yes)\\n(no)\n" BLUE);
        continue_flag = GetAnswer ();
        clear_buffer();
        switch (continue_flag)
        {
            case 'n':
                fprintf (stderr, MAGENTA "Whell, thank you for using my Akinator programm although\n" 
                    YELLOW "Have a good day!\n" NORMAL);
                break;
            case 'y':
                break;
            default:
                assert (0);
        } 
        AkinDump (node_ptr);
    }
    repository = fopen ("repository.txt", "w");
    WriteNode (node_ptr, repository);
    fclose (repository);
    NodeDestructor (node_ptr);
    return 0;
}