#ifndef _ERROR_H
#define _ERROR_H

enum {
        I_INITIALIZATION_ERROR,
        I_UNDEFINED_EVENT_HANDLER_ERROR,
        I_NULL_LINKED_ERROR,
        I_BAD_TRIPLET_ERROR,
        I_FRONTEND_INITIALIZATION_ERROR,
        I_ERROR_ERROR,
        I_IMPOSSIBLE_ERROR
} inferno_errors;

int ishowerrors;

int idie(int error);

int ierror(int error);

#endif
