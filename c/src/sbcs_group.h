#ifndef __sbcs_group_h
#define __sbcs_group_h

#define NUM_OF_SBCS_PROBERS 10

#include "sbcs.h"

typedef struct {
	probingstate state;
	sbcsprober probers[NUM_OF_SBCS_PROBERS];
	PRBool isactive[NUM_OF_SBCS_PROBERS];
	PRUint32 bestguess;
	PRUint32 activenum;
} sbcsgroupprober;

int sbcs_group_init(sbcsgroupprober* THIS);
int sbcs_group_reset(sbcsgroupprober* THIS);
int sbcs_group_destroy(sbcsgroupprober* THIS);
float sbcs_group_getconfidence(sbcsgroupprober* THIS);
const char* sbcs_group_getcharsetname(sbcsgroupprober* THIS);
probingstate sbcs_group_handledata(sbcsgroupprober* THIS, 
                                   const char* abuf, PRUint32 alen);
PRBool sbcs_group_filterwoutengletters (const char* abuf, PRUint32 alen,
                                        char** newbuf, PRUint32* newlen);
PRBool sbcs_group_filterwithengletters (const char* abuf, PRUint32 alen,
                                        char** newbuf, PRUint32* newlen);

#endif /* __sbcs_group_h */

