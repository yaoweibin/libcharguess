#include "utf8.h"

int utf8_init(utf8prober* THIS) {
    int retval = 1;

    if(!(THIS->codingsm = (statemachine*)malloc(sizeof(statemachine)))) {
        return 0;
    }
    retval = retval & statemachine_init(THIS->codingsm, &UTF8smmodel);
    retval = retval & utf8_reset(THIS);

    return retval;
}

int utf8_reset(utf8prober* THIS) {
    THIS->state = eDetecting;
	THIS->numofmbchar = 0;

    return statemachine_reset(THIS->codingsm);
}

int utf8_destroy(utf8prober* THIS) {
    free(THIS->codingsm);
    return statemachine_destroy(THIS->codingsm);
}

probingstate utf8_handledata(utf8prober* THIS, const char* abuf, PRUint32 alen){
	smstate codingstate;
	PRUint32 i;

	for(i = 0; i < alen; i++) {
		codingstate = statemachine_nextstate(THIS->codingsm, abuf[i]);
		if(codingstate == eError) {
			THIS->state = eNotMe;
			break;
		}
		if(codingstate == eItsMe) {
			THIS->state = eFoundIt;
			break;
		}
		if(codingstate == eStart) {
			if(statemachine_getcurrcharlen(THIS->codingsm) >= 2) {
				THIS->numofmbchar++;
			}
		}
	}

	if(THIS->state == eDetecting) {
		if(utf8_getconfidence(THIS) > SHORTCUT_THRESHOLD) {
			THIS->state = eFoundIt;
		}
	}
	return THIS->state;
}

const char* utf8_getcharsetname(utf8prober* THIS) {
	return "UTF-8";
}

probingstate utf8_getstate(utf8prober* THIS) {
	return THIS->state;
}

float utf8_getconfidence(utf8prober* THIS) {
	float unlike = (float)0.99;
	PRUint32 i;

	if(THIS->numofmbchar < 6) {
		for(i = 0; i < THIS->numofmbchar; i++) {
			unlike *= ONE_CHAR_PROB;
		}
		return (float)1.0 - unlike;
	}
	else {
		return (float)0.99;
	}
}
