#ifndef __sjis_h
#define __sjis_h

#include "types.h"
#include "statemachine.h"
#include "jpcntx.h"
#include "chardist.h"

typedef struct {
	statemachine* codingsm;
	probingstate state;
	sjiscontext_analysis contextanalyser;
	SJISdist distanalyser;
	char lastchar[2];
} sjisprober;

int sjis_init(sjisprober* THIS);
int sjis_reset(sjisprober* THIS);
int sjis_destroy(sjisprober* THIS);
probingstate sjis_handledata(sjisprober* THIS, const char* abuf, PRUint32 alen);
const char* sjis_getcharsetname(sjisprober* THIS);
probingstate sjis_getstate(sjisprober* THIS);
float sjis_getconfidence(sjisprober* THIS);


#endif /* __sjis_h */
