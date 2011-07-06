#ifndef __utf8_h
#define __utf8_h

#include "types.h"
#include "statemachine.h"

#define ONE_CHAR_PROB (float)0.50

typedef struct {
	statemachine* codingsm;
	probingstate state;
	PRUint32 numofmbchar;
} utf8prober;

int utf8_init(utf8prober* THIS);
int utf8_reset(utf8prober* THIS);
int utf8_destroy(utf8prober* THIS);
probingstate utf8_handledata(utf8prober* THIS);
const char* utf8_getcharsetname(utf8prober* THIS);
probingstate utf8_getstate(utf8prober* THIS);
float utf8_getconfidence(utf8prober* THIS);

#endif /* __utf8_h */
