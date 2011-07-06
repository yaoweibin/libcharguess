

#include <stdio.h>
#include "sbcs.h"


int sbcs_init(sbcsprober* THIS, sequencemodel* model) {

	THIS->model = model;
	return sbcs_reset(THIS);
}


int sbcs_reset(sbcsprober* THIS) {

	PRUint32 i;
	
	THIS->state = eDetecting;
	THIS->lastorder = 255;
	for(i = 0; i < NUMBER_OF_SEQ_CAT; i++)  THIS->seqcounters[i] = 0;
	THIS->totalseqs = 0;
	THIS->totalchar = 0;
	THIS->freqchar = 0;

	return 1;
}


/* #define NEGATIVE_APPROACH 1 */

float sbcs_getconfidence(sbcsprober* THIS) {

	float r;
#ifdef NEGATIVE_APPROACH
	if(THIS->totalseqs > 0) {
		if(THIS->totalseqs > THIS->seqcounter[NEGATIVE_CAT] * 10) {
			return (float)(THIS->totalseqs - THIS->seqcounters[NEGATIVE_CAT] * 10)/THIS->totalseqs * THIS->freqchar / THIS->totalchar;
		}
		return (float)0.01;
	}
#else
	if(THIS->totalseqs > 0) {
		r = ((float)1.0) * THIS->seqcounters[POSITIVE_CAT] / THIS->totalseqs / THIS->model->typical_positive_ratio;
		r = r * THIS->freqchar / THIS->totalchar;
		if(r >= (float)1.00) {
			r = (float)0.99;
		}
		return r;
	}
	return (float)0.01;
#endif
}


const char* sbcs_getcharsetname(sbcsprober* THIS) {

	return THIS->model->charset_name;
}


int sbcs_destroy(sbcsprober* THIS) {

	return 1;
}


probingstate sbcs_handledata(sbcsprober* THIS, const char* abuf, PRUint32 alen) {
	unsigned char order;
	float cf;
	PRUint32 i;

	for(i = 0; i < alen; i++) {
		order = THIS->model->ordermap[(unsigned char)abuf[i]];
		if(order < SYMBOL_CAT_ORDER) {
			THIS->totalchar++;
		}
		if(order < SAMPLE_SIZE) {
			THIS->freqchar++;
			if(THIS->lastorder < SAMPLE_SIZE) {
				THIS->totalseqs++;
				++(THIS->seqcounters[THIS->model->precedence_matrix[THIS->lastorder * SAMPLE_SIZE + order]]);
			}
		}
		THIS->lastorder = order;
	}

	if(THIS->state == eDetecting) {
		if(THIS->totalseqs > SB_ENOUGH_REL_THRESHOLD) {
			cf = sbcs_getconfidence(THIS);
			if(cf > POSITIVE_SHORTCUT_THRESHOLD) {
				THIS->state = eFoundIt;
			}
			else if(cf < NEGATIVE_SHORTCUT_THRESHOLD) {
				THIS->state = eNotMe;
			}
		}
	}

	return THIS->state;
}
