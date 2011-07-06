#include "eucjp.h"

int eucjp_init(eucjpprober* THIS) {
	int retval = 1;
	
	if(!(THIS->codingsm = (statemachine*)malloc(sizeof(statemachine)))) {
        return 0;
    }
    retval = retval & statemachine_init(THIS->codingsm, &EUCJPsmmodel);
	retval = retval & eucjpcontext_analysis_init(&(THIS->contextanalyser));
	retval = retval & EUCJPdist_analysis_init(&(THIS->distanalyser));
    retval = retval & eucjp_reset(THIS);

	return retval;
}

int eucjp_reset(eucjpprober* THIS) {
	int retval = 1;
	
	retval = retval & statemachine_reset(THIS->codingsm);
    retval = retval & eucjpcontext_analysis_reset(&(THIS->contextanalyser));
    retval = retval & EUCJPdist_analysis_reset(&(THIS->distanalyser));
    THIS->state = eDetecting;

    return retval;
}

int eucjp_destroy(eucjpprober* THIS) {
	int retval = 1;
	
	free(THIS->codingsm);
	retval = retval & statemachine_destroy(THIS->codingsm);
	retval = retval & eucjpcontext_analysis_destroy(&(THIS->contextanalyser));
	retval = retval & EUCJPdist_analysis_destroy(&(THIS->distanalyser));
	
	return retval;
}

probingstate eucjp_handledata(eucjpprober* THIS, const char* abuf,
                              PRUint32 alen) {
	smstate codingstate;
	PRUint32 i;
	PRUint32 charlen;

	for(i = 0; i < alen; i++) {
		codingstate = statemachine_nextstate(THIS->codingsm, abuf[1]);
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
				eucjpcontext_analysis_handleonechar(&(THIS->contextanalyser),
				                                    THIS->lastchar, charlen);
				EUCJPdist_analysis_handleonechar(&(THIS->distanalyser),
				                                 THIS->lastchar, charlen);
			}
			else {
				eucjpcontext_analysis_handleonechar(&(THIS->contextanalyser),
				                                    abuf + i - 1, charlen);
				EUCJPdist_analysis_handleonechar(&(THIS->distanalyser),
				                                 abuf + i - 1, charlen);
			}
		}
	}

	THIS->lastchar[0] = abuf[alen - 1];

	if(THIS->state == eDetecting) {
		if(eucjpcontext_analysis_gotenoughdata(&(THIS->contextanalyser)) &&
		   eucjp_getconfidence(THIS) > SHORTCUT_THRESHOLD) {
			THIS->state = eFoundIt;
		}
	}

	return THIS->state;
}

const char* eucjp_getcharsetname(eucjpprober* THIS) {
	return "EUC-JP";
}

probingstate eucjp_getstate(eucjpprober* THIS) {
	return THIS->state;
}

float eucjp_getconfidence(eucjpprober* THIS) {
    float contextcf;
    float distcf;

    contextcf = eucjpcontext_analysis_getconfidence(&(THIS->contextanalyser));
    distcf = EUCJPdist_analysis_getconfidence(&(THIS->distanalyser));

    return (contextcf > distcf ? contextcf : distcf);
}
