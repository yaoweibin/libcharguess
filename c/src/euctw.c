#include "euctw.h"

int euctw_init(euctwprober* THIS) {
    int retval = 1;

    if(!(THIS->codingsm = (statemachine*)malloc(sizeof(statemachine)))) {
        return 0;
    }
    retval = retval & statemachine_init(THIS->codingsm, &EUCTWsmmodel);
    retval = retval & EUCTWdist_analysis_init(&(THIS->distanalyser));
    retval = retval & euctw_reset(THIS);

    return retval;
}

int euctw_reset(euctwprober* THIS) {
    int retval = 1;

    retval = retval & statemachine_reset(THIS->codingsm);
    retval = retval & EUCTWdist_analysis_reset(&(THIS->distanalyser));
    THIS->state = eDetecting;

    return retval;
}

int euctw_destroy(euctwprober* THIS) {
    int retval = 1;

    free(THIS->codingsm);
    retval = retval & statemachine_destroy(THIS->codingsm);
    retval = retval & EUCTWdist_analysis_destroy(&(THIS->distanalyser));

    return retval;
}

probingstate euctw_handledata(euctwprober* THIS, const char* abuf,
                              PRUint32 alen) {
	smstate codingstate;
	PRUint32 i;
	PRUint32 charlen;

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
			charlen = statemachine_getcurrcharlen(THIS->codingsm);
			if(i == 0) {
				THIS->lastchar[1] = abuf[0];
				EUCTWdist_analysis_handleonechar(&(THIS->distanalyser),
				                                 THIS->lastchar, charlen);
			}
			else {
				EUCTWdist_analysis_handleonechar(&(THIS->distanalyser),
				                                 abuf + i - 1, charlen);
			}
		}
	}

	THIS->lastchar[0] = abuf[alen - 1];

	if(THIS->state == eDetecting) {
		if(EUCTWdist_analysis_gotenoughdata(&(THIS->distanalyser)) &&
		   euctw_getconfidence(THIS) > SHORTCUT_THRESHOLD) {
			THIS->state = eFoundIt;
		}
	}

	return THIS->state;
}

const char* euctw_getcharsetname(euctwprober* THIS) {
	return "x-euc-tw";
}

probingstate euctw_getstate(euctwprober* THIS) {
	return THIS->state;
}

float euctw_getconfidence(euctwprober* THIS) {
	float distcf;
    distcf = EUCTWdist_analysis_getconfidence(&(THIS->distanalyser));
	return (float)distcf;
}
