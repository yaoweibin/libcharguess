#include "euckr.h"

int euckr_init(euckrprober* THIS) {
    int retval = 1;

    if(!(THIS->codingsm = (statemachine*)malloc(sizeof(statemachine)))) {
        return 0;
    }
    retval = retval & statemachine_init(THIS->codingsm, &EUCKRsmmodel);
    retval = retval & EUCKRdist_analysis_init(&(THIS->distanalyser));
    retval = retval & euckr_reset(THIS);

    return retval;
}

int euckr_reset(euckrprober* THIS) {
    int retval = 1;

    retval = retval & statemachine_reset(THIS->codingsm);
    retval = retval & EUCKRdist_analysis_reset(&(THIS->distanalyser));
    THIS->state = eDetecting;

    return retval;
}

int euckr_destroy(euckrprober* THIS) {
    int retval = 1;

    free(THIS->codingsm);
    retval = retval & statemachine_destroy(THIS->codingsm);
    retval = retval & EUCKRdist_analysis_destroy(&(THIS->distanalyser));

    return retval;
}

probingstate euckr_handledata(euckrprober* THIS, const char* abuf,
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
				EUCKRdist_analysis_handleonechar(&(THIS->distanalyser),
				                                 THIS->lastchar, charlen);
			}
			else {
				EUCKRdist_analysis_handleonechar(&(THIS->distanalyser),
				                                 abuf + i - 1, charlen);
			}
		}
	}

	THIS->lastchar[0] = abuf[alen - 1];

	if(THIS->state == eDetecting) {
		if(EUCKRdist_analysis_gotenoughdata(&(THIS->distanalyser)) &&
		   euckr_getconfidence(THIS) > SHORTCUT_THRESHOLD) {
			THIS->state = eFoundIt;
		}
	}

	return THIS->state;
}

const char* euckr_getcharsetname(euckrprober* THIS) {
	return "EUC-KR";
}

probingstate euckr_getstate(euckrprober* THIS) {
	return THIS->state;
}

float euckr_getconfidence(euckrprober* THIS) {
    float distcf;
    distcf = EUCKRdist_analysis_getconfidence(&(THIS->distanalyser));
    return (float)distcf;
}
