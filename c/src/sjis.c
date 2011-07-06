#include "sjis.h"

/* For S-JIS encoding, observe characteristic:
** 1, kana character (or hankaku?) often have a high frequency of appearance
** 2, kana character often exist in a group
** 3, certain combinations of kana are never used in the japanese language */

int sjis_init(sjisprober* THIS) {
    int retval = 1;

    if(!(THIS->codingsm = (statemachine*)malloc(sizeof(statemachine)))) {
        return 0;
    }
    retval = retval & statemachine_init(THIS->codingsm, &SJISsmmodel);
    retval = retval & sjiscontext_analysis_init(&(THIS->contextanalyser));
    retval = retval & SJISdist_analysis_init(&(THIS->distanalyser));
    retval = retval & sjis_reset(THIS);

    return retval;
}

int sjis_reset(sjisprober* THIS) {
    int retval = 1;

    retval = retval & statemachine_reset(THIS->codingsm);
    retval = retval & sjiscontext_analysis_reset(&(THIS->contextanalyser));
    retval = retval & SJISdist_analysis_reset(&(THIS->distanalyser));
    THIS->state = eDetecting;

    return retval;
}

int sjis_destroy(sjisprober* THIS) {
    int retval = 1;

    free(THIS->codingsm);
    retval = retval & statemachine_destroy(THIS->codingsm);
    retval = retval & sjiscontext_analysis_destroy(&(THIS->contextanalyser));
    retval = retval & SJISdist_analysis_destroy(&(THIS->distanalyser));

    return retval;
}

probingstate sjis_handledata(sjisprober* THIS, const char* abuf, PRUint32 alen){
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
				sjiscontext_analysis_handleonechar(&(THIS->contextanalyser),
				                                   THIS->lastchar + 2 - charlen,
				                                   charlen);
				SJISdist_analysis_handleonechar(&(THIS->distanalyser),
				                                THIS->lastchar, charlen);
			}
			else {
				sjiscontext_analysis_handleonechar(&(THIS->contextanalyser),
				                                   abuf + i - charlen,
				                                   charlen);
				SJISdist_analysis_handleonechar(&(THIS->distanalyser),
				                                abuf + i - 1, charlen);
			}
		}
	}

	THIS->lastchar[0] = abuf[alen - 1];

	if(THIS->state == eDetecting) {
		if(sjiscontext_analysis_gotenoughdata(&(THIS->contextanalyser)) &&
		   sjis_getconfidence(THIS) > SHORTCUT_THRESHOLD) {
			THIS->state = eFoundIt;
		}
	}

	return THIS->state;
}

const char* sjis_getcharsetname(sjisprober* THIS) {
	return "Shift_JIS";
}

probingstate sjis_getstate(sjisprober* THIS) {
	return THIS->state;
}

float sjis_getconfidence(sjisprober* THIS) {
	float contextcf;
	float distcf;

	contextcf = sjiscontext_analysis_getconfidence(&(THIS->contextanalyser));
	distcf = SJISdist_analysis_getconfidence(&(THIS->distanalyser));

	return (contextcf > distcf ? contextcf : distcf);
}
