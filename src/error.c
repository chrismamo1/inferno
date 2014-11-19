#include "error.h"
#include "iprettyconsole.h"
#include <stdlib.h>
#include <stdio.h>

int ishowerrors = 1;

int idie(int error);

///local
void print_error(int error, char *message, int iswarning)
{
        if (iswarning)
                fprintf(stdout, KYEL "WARNING %d" RESET, error);
        else
                fprintf(stdout, KRED "ERROR %d" RESET, error);
        fprintf(stdout, ": %s\n", message);
        return;
}

int ierror(int error)
{
        switch (error) {
        case I_INITIALIZATION_ERROR:
                if(ishowerrors) 
                        print_error(I_INITIALIZATION_ERROR, 
                                    "problem initializing inferno object.",
                                    0
                                   );
                return 1;
        case I_UNDEFINED_EVENT_HANDLER_ERROR:
                if(ishowerrors) 
                        print_error(I_UNDEFINED_EVENT_HANDLER_ERROR,
                                    "trying to call an undefined event handler.",
                                    1
                                   );
                return 1;
        case I_NULL_LINKED_ERROR:
                if(ishowerrors) 
                        print_error(I_NULL_LINKED_ERROR,
                                    "trying to access a null linked list element node.",
                                    1
                                   );
                return 2;
        case I_BAD_TRIPLET_ERROR:
                if(ishowerrors) 
                        print_error(I_BAD_TRIPLET_ERROR,
                                    "trying to use a defective triplet, maybe two of the points accidentally have the same address or value?",
                                    0
                                   );
                return 1;
        case I_FRONTEND_INITIALIZATION_ERROR:
                if(ishowerrors) 
                        print_error(I_FRONTEND_INITIALIZATION_ERROR,
                                    "problem setting up your frontend.",
                                    0
                                   );
                return 2;
        case I_ERROR_ERROR:
                if(ishowerrors) 
                        print_error(I_ERROR_ERROR,
                                    "trying to throw a nonexistent error.",
                                    1
                                   );
                return 1;
        case I_IMPOSSIBLE_ERROR:
                if (ishowerrors)
                        print_error(I_IMPOSSIBLE_ERROR,
                                    "the program has just reached a point that you thought was impossible to reach, and you decided to not handle it just in case, instead opting to make it throw the \"" KRED "I_IMPOSSIBLE_ERROR" RESET "\" just to tempt fate.",
                                    0);
                return 2;
        default:
                return ierror(I_ERROR_ERROR);
        }
}
