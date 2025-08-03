// Libraries
#include <stdio.h>

// Allocation logging
#ifdef RESOURCE_LOG
    #define alloc_log(ptr) printf("[ALLOC]: %p in %s:%d\n",ptr, __FILE__, __LINE__)
    #define free_log(ptr)  printf("[FREE]: %p in %s:%d\n",ptr, __FILE__, __LINE__)
#else
    #define alloc_log(ptr)
    #define free_log(ptr)
#endif

// Error logging
#ifdef ERROR_LOG
    #define error_log(mesg) printf("[ERR]: %s", mesg)
#else
    #define error_log(mesg)
#endif
