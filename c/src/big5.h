#ifndef __big5_h
#define __big5_h

#include "types.h"
#include "statemachine.h"
#include "chardist.h"

typedef struct {
	statemachine* codingsm;
	probingstate state;
	Big5dist distanalyser;
	char lastchar[2];
} big5prober;

int big5_init(big5prober* THIS);
int big5_reset(big5prober* THIS);
int big5_destroy(big5prober* THIS);
probingstate big5_handledata(big5prober* THIS, const char* abuf, PRUint32 alen);
const char* big5_getcharsetname(big5prober* THIS);
probingstate big5_getstate(big5prober* THIS);
float big5_getconfidence(big5prober* THIS);

#endif /* __big5_h */
