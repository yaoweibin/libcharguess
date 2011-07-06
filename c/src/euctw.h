#ifndef __euctw_h
#define __euctw_h

#include "types.h"
#include "statemachine.h"
#include "chardist.h"


typedef struct {
	statemachine* codingsm;
	probingstate state;
	EUCTWdist distanalyser;
	char lastchar[2];
} euctwprober;


int euctw_init(euctwprober* THIS);
int euctw_reset(euctwprober* THIS);
int euctw_destroy(euctwprober* THIS);
probingstate euctw_handledata(euctwprober* THIS, const char* abuf,
                              PRUint32 alen);
const char* euctw_getcharsetname(euctwprober* THIS);
probingstate euctw_getstate(euctwprober* THIS);
float euctw_getconfidence(euctwprober* THIS);


#endif /* __euctw_h */
