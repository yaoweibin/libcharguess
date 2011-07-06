#include "gb2312.h"

int gb18030_init(gb18030prober* THIS) {
    int retval = 1;

    if(!(THIS->codingsm = (statemachine*)malloc(sizeof(statemachine)))) {
        return 0;
    }
    retval = retval & statemachine_init(THIS->codingsm, &GB18030smmodel);
    retval = retval & GB2312dist_analysis_init(&(THIS->distanalyser));
    retval = retval & gb18030_reset(THIS);

    return retval;
}

int gb18030_reset(gb18030prober* THIS) {
    int retval = 1;

    retval = retval & statemachine_reset(THIS->codingsm);
    retval = retval & GB2312dist_analysis_reset(&(THIS->distanalyser));
    THIS->state = eDetecting;

    return retval;
}

int gb18030_destroy(gb18030prober* THIS) {
    int retval = 1;

    free(THIS->codingsm);
    retval = retval & statemachine_destroy(THIS->codingsm);
    retval = retval & GB2312dist_analysis_destroy(&(THIS->distanalyser));

    return retval;
}

probingstate gb18030_handledata(gb18030prober* THIS, const char* abuf,
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
                GB2312dist_analysis_handleonechar(&(THIS->distanalyser),
                                                 THIS->lastchar, charlen);
            }
            else {
                GB2312dist_analysis_handleonechar(&(THIS->distanalyser),
                                                 abuf + i - 1, charlen);
            }
        }
    }

    THIS->lastchar[0] = abuf[alen - 1];

    if(THIS->state == eDetecting) {
        if(GB2312dist_analysis_gotenoughdata(&(THIS->distanalyser)) &&
           gb18030_getconfidence(THIS) > SHORTCUT_THRESHOLD) {
            THIS->state = eFoundIt;
        }
    }

    return THIS->state;
}

const char* gb18030_getcharsetname(gb18030prober* THIS) {
	return "gb18030";
}

probingstate gb18030_getstate(gb18030prober* THIS) {
	return THIS->state;
}

float gb18030_getconfidence(gb18030prober* THIS) {
    float distcf;
    distcf = GB2312dist_analysis_getconfidence(&(THIS->distanalyser));
    return (float)distcf;
}
