#ifndef ERR_MSG_H
#define ERR_MSG_H
#include<stdio.h>

/*-------*/
/* Macro */
/*-------*/

#define LOG_MSG(LEVEL, MSG, VA_LIST) fprintf( stderr,	\
                                              "%s: " MSG,  \
					      (LEVEL),	\
					      (VA_LIST) );

#define ERR_MSG(MSG, VA_LIST) LOG_MSG("ERR", MSG, VA_LIST)
#define DBG_MSG(MSG, VA_LIST) LOG_MSG("DBG", MSG, VA_LIST)

/* Define for easy format variables */
#define STR(t) #t

/*----------*/
/* Function */
/*----------*/
void err_msg_init();


#endif
    
