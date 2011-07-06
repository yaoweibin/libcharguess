#ifndef __euckr_h
#define __euckr_h

#include "types.h"
#include "statemachine.h"
#include "chardist.h"

typedef struct {
	statemachine* codingsm;
	probingstate state;
	EUCKRdist distanalyser;
	char lastchar[2];
} euckrprober;

int euckr_init(euckrprober* THIS);
int euckr_reset(euckrprober* THIS);
int euckr_destroy(euckrprober* THIS);
probingstate euckr_handledata(euckrprober* THIS, const char* abuf,
                              PRUint32 alen);
const char* euckr_getcharsetname(euckrprober* THIS);
probingstate euckr_getstate(euckrprober* THIS);
float euckr_getconfidence(euckrprober* THIS);

#endif /* __euckr_h */
