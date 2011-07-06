#include "esccs.h"

int esccs_init(esccsprober* THIS) {
	statemachine_init(&(THIS->codingsm[0]), &HZsmmodel);
	statemachine_init(&(THIS->codingsm[1]), &ISO2022CNsmmodel);
	statemachine_init(&(THIS->codingsm[2]), &ISO2022JPsmmodel);
	statemachine_init(&(THIS->codingsm[3]), &ISO2022KRsmmodel);
	THIS->activesm = NUM_OF_ESC_CHARSETS;
	THIS->state = eDetecting;
	THIS->detectedcharset = nsnull;
	return 1;
}

int esccs_reset(esccsprober* THIS) {
	PRUint32 i;

	THIS->state = eDetecting;
	for(i = 0; i < NUM_OF_ESC_CHARSETS; i++) {
		statemachine_reset(&(THIS->codingsm[i]));
	}
	THIS->activesm = NUM_OF_ESC_CHARSETS;
	THIS->detectedcharset = nsnull;
	return 1;
}

int esccs_destroy(esccsprober* THIS) {
	return 1;
}

probingstate esccs_handledata(esccsprober* THIS, const char* abuf,
                              PRUint32 alen) {
	smstate codingstate;
	PRInt32 j;
	PRUint32 i;
	statemachine t; /* temporary */

	for(i = 0; i < alen && THIS->state == eDetecting; i++) {
		for(j = THIS->activesm - 1; j >= 0; j--) {
			/* the byte is fed to all active state machines */
			codingstate = statemachine_nextstate(&(THIS->codingsm[j]), abuf[i]);
			if(codingstate == eError) {
				/* got a negative answer froom this state machine, deactivate */
				THIS->activesm--;
				if(THIS->activesm == 0) {
					THIS->state = eNotMe;
					return THIS->state;
				}
				else if(j != (PRInt32)THIS->activesm) {
					t = THIS->codingsm[THIS->activesm];
					THIS->codingsm[THIS->activesm] = THIS->codingsm[j];
					THIS->codingsm[j] = t;
				}
			}
			else if (codingstate == eItsMe) {
				THIS->state = eFoundIt;
				THIS->detectedcharset = statemachine_getsmname(
                                         &(THIS->codingsm[j]));
				return THIS->state;
			}
		}
	}
	return THIS->state;
}

const char* esccs_getcharsetname(esccsprober* THIS) {
	return THIS->detectedcharset;
}

probingstate esccs_getstate(esccsprober* THIS) {
	return THIS->state;
}

float esccs_getconfidence(esccsprober* THIS) {
	return (float)0.99;
}

