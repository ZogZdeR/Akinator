#include <stdio.h>
#include "tree_func.hpp"

int main ()
{
    char nothing_str[9] = "Nothing";
    Node nothing = {nothing_str, NULL, NULL, NULL};
    Node *node_ptr = &nothing;
    int continue_flag = 'y';
    while (continue_flag == 'y')
    {
        AskForAPerson (node_ptr);
        clear_buffer ();
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
    FILE *repository = fopen ("repository.txt", "w");
    WriteNode (node_ptr, repository);

    NodeDestructor (node_ptr);
    return 0;
}