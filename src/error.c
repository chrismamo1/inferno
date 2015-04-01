#include "error.h"
#include "iprettyconsole.h"
#include <stdlib.h>
#include <stdio.h>

int ishowerrors = 1;

int idie(int error);

/** Handles printing of errors. Static function, only to be called by ierror
 * @param error the integer code of the error to be printed
 * @param *message the message to tell the user
 * @param iswarning bool, determines the message prefix and its color
 */
static void print_error(int error, char *message, int iswarning)
{
        if (iswarning)
                fprintf(stderr, KYEL "WARNING %d" RESET, error);
        else
                fprintf(stderr, KRED "ERROR %d" RESET, error);
        fprintf(stdout, ": %s\n", message);
        return;
}

/** Inferno's error handling function
 * @param error the error code that has been encountered
 * @return an integer specifying the severity of the error
 */
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
