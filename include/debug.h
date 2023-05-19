#include <stdio.h>

#ifndef NDEBUG 
#define DEBUGPRINT(...) \
do \
{ \
    fprintf(stdout, "\033[0;36m"); \
    fprintf(stdout ,__VA_ARGS__); \
    fprintf(stdout, "\033[0m"); \
} while (0) 
#else
# define DEBUGPRINT(...) do {} while (0)
#endif 

#ifndef NDEBUG 
#define ERRORPRINT(...) \
do \
{ \
    fprintf(stderr, "\033[0;31m"); \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\033[0m"); \
} while (0) 
#else
# define ERRORPRINT(...) do {} while (0)
#endif
