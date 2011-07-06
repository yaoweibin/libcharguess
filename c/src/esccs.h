#ifndef __esccs_h
#define __esccs_h

#include "statemachine.h"

#define NUM_OF_ESC_CHARSETS 4

typedef struct {
	PRUint32 activesm;
	probingstate state;
	const char* detectedcharset;
	statemachine codingsm[NUM_OF_ESC_CHARSETS];
} esccsprober;


int esccs_init(esccsprober* THIS);
int esccs_reset(esccsprober* THIS);
int esccs_destroy(esccsprober* THIS);
probingstate esccs_handledata(esccsprober* THIS, const char* abuf, 
                              PRUint32 alen);
const char* esccs_getcharsetname(esccsprober* THIS);
probingstate esccs_getstate(esccsprober* THIS);
float esccs_getconfidence(esccsprober* THIS);

#endif /* __esccs_h */
