#include "statemachine.h"

int statemachine_init(statemachine* THIS, smmodel* sm) {
	THIS->currstate = eStart;
	THIS->model = sm;
	return 1;
}

int statemachine_reset(statemachine* THIS) {
	THIS->currstate = eStart;
	return 1;
}

int statemachine_destroy(statemachine* THIS) {
	return 1;
}

smstate statemachine_nextstate(statemachine* THIS, char c) {
	
	/* get the class for each byte.  if it is the first byte we also get
	** the byte length */
	PRUint32 bytecls = GETCLASS(c);

	if(THIS->currstate == eStart) {
		THIS->currbytepos = 0;
		THIS->currcharlen = THIS->model->charlentable[bytecls];
	}
	/* we get its next state from the byte's class and statetable */
	THIS->currstate = (smstate)GETFROMPCK(THIS->currstate * 
	                   (THIS->model->classfactor) + bytecls,
                       THIS->model->statetable);
	THIS->currbytepos++;
	return THIS->currstate;
}

PRUint32 statemachine_getcurrcharlen(statemachine* THIS) {
	return THIS->currcharlen;
}

const char* statemachine_getsmname(statemachine* THIS) {
	return THIS->model->name;
}
