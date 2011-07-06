#include "big5.h"

int big5_init(big5prober* THIS) {
    int retval = 1;

    if(!(THIS->codingsm = (statemachine*)malloc(sizeof(statemachine)))) {
        return 0;
    }
    retval = retval & statemachine_init(THIS->codingsm, &Big5smmodel);
    retval = retval & Big5dist_analysis_init(&(THIS->distanalyser));
    retval = retval & big5_reset(THIS);

    return retval;
}

int big5_reset(big5prober* THIS) {
    int retval = 1;

    retval = retval & statemachine_reset(THIS->codingsm);
    retval = retval & Big5dist_analysis_reset(&(THIS->distanalyser));
    THIS->state = eDetecting;

    return retval;
}

int big5_destroy(big5prober* THIS) {
    int retval = 1;

    free(THIS->codingsm);
    retval = retval & statemachine_destroy(THIS->codingsm);
    retval = retval & Big5dist_analysis_destroy(&(THIS->distanalyser));

    return retval;
}

probingstate big5_handledata(big5prober* THIS, const char* abuf, PRUint32 alen){
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
                Big5dist_analysis_handleonechar(&(THIS->distanalyser),
                                                 THIS->lastchar, charlen);
            }
            else {
                Big5dist_analysis_handleonechar(&(THIS->distanalyser),
                                                 abuf + i - 1, charlen);
            }
        }
    }

    THIS->lastchar[0] = abuf[alen - 1];

    if(THIS->state == eDetecting) {
        if(Big5dist_analysis_gotenoughdata(&(THIS->distanalyser)) &&
           big5_getconfidence(THIS) > SHORTCUT_THRESHOLD) {
            THIS->state = eFoundIt;
        }
    }

    return THIS->state;
}

const char* big5_getcharsetname(big5prober* THIS) {
	return "Big5";
}

probingstate big5_getstate(big5prober* THIS) {
	return THIS->state;
}

float big5_getconfidence(big5prober* THIS) {
    float distcf;
    distcf = Big5dist_analysis_getconfidence(&(THIS->distanalyser));
    return (float)distcf;
}
