
#include "sbcs_group.h"

int sbcs_group_init(sbcsgroupprober* THIS) {

	sbcs_init(&THIS->probers[0], &win1251_model);
	sbcs_init(&THIS->probers[1], &KOI8R_model);
	sbcs_init(&THIS->probers[2], &latin5_model);
	sbcs_init(&THIS->probers[3], &maccyrillic_model);
	sbcs_init(&THIS->probers[4], &IBM866_model);
	sbcs_init(&THIS->probers[5], &IBM855_model);
	sbcs_init(&THIS->probers[6], &latin7_model);
	sbcs_init(&THIS->probers[7], &win1253_model);
	sbcs_init(&THIS->probers[8], &latin5_bulgarian_model);
	sbcs_init(&THIS->probers[9], &win1251_bulgarian_model);

	return sbcs_group_reset(THIS);
}

int sbcs_group_reset(sbcsgroupprober* THIS) {

	PRUint32 i;

	for(i = 0; i < NUM_OF_SBCS_PROBERS; i++) {
		sbcs_reset(&THIS->probers[i]);
		THIS->isactive[i] = PR_TRUE;
	}
	THIS->bestguess = -1;
	THIS->state = eDetecting;

	return 1;
}

int sbcs_group_destroy(sbcsgroupprober* THIS) {

	PRUint32 i;

	for(i = 0; i < NUM_OF_SBCS_PROBERS; i++) {
		sbcs_destroy(&THIS->probers[i]);
	}

	return 1;
}

float sbcs_group_getconfidence(sbcsgroupprober* THIS) {

	PRUint32 i;
	float bestconf = 0.0, cf;

	switch(THIS->state) {

	case eFoundIt:
		return (float)0.99;  /* sure yes */
	case eNotMe:
		return (float)0.01;  /* sure no */
	default:
		for(i = 0; i < NUM_OF_SBCS_PROBERS; i++) {
			if(!(THIS->isactive[i])) {
				continue;
			}
			cf = sbcs_getconfidence(&THIS->probers[i]);
			if(bestconf < cf) {
				bestconf = cf;
				THIS->bestguess = i;
			}
		}
		break;
	}

	return bestconf;
}

const char* sbcs_group_getcharsetname(sbcsgroupprober* THIS) {

	/* no answer yet */
	if(THIS->bestguess == -1) {
		sbcs_group_getconfidence(THIS);
		/* no best guess */
		if(THIS->bestguess == -1) {
			THIS->bestguess = 0;  /* use default */
		}
	}

	return sbcs_getcharsetname(&THIS->probers[THIS->bestguess]);
}

probingstate sbcs_group_handledata(sbcsgroupprober* THIS, 
                                   const char* abuf, PRUint32 alen) {

	probingstate st;
	PRUint32 i;
	char* newbuf1;
	PRUint32 newlen1;

	/* apply filter to original buffer, and we got new buffer back
	** depend on what script it is, we will feed them to the new buffer
	** we got after applying proper filter */
	sbcs_group_filterwithengletters(abuf, alen, &newbuf1, &newlen1);

	for(i = 0; i < NUM_OF_SBCS_PROBERS; i++) {
		if(!(THIS->isactive[i])) {
			continue;
		}

		st = sbcs_handledata(&THIS->probers[i], newbuf1, newlen1);

		if(st == eFoundIt) {
			THIS->bestguess = i;
			THIS->state = eFoundIt;
			break;
		}
		else if(st == eNotMe) {
			THIS->isactive[i] = PR_FALSE;
			THIS->activenum--;
			if(THIS->activenum <= 0) {
				THIS->state = eNotMe;
				break;
			}
		}
	}

	PR_FREEIF(newbuf1);

	return THIS->state;
}


/* This filter apply to all scripts that do not use latin letters */
PRBool sbcs_group_filterwoutengletters (const char* abuf, PRUint32 alen,
                                        char** newbuf, PRUint32 *newlen) {

	/* do filtering to reduce load to probers */
	char* newptr;
	char* prevptr;
	char* curptr;

	PRBool meetmsb = PR_FALSE;

	newptr = *newbuf = (char*)PR_MALLOC(alen);
	if(!newptr) {
		return PR_FALSE;
	}

	/* more ugly pointer arithmetic to clean up */
	for( curptr = prevptr = (char*)abuf; curptr < abuf + alen; curptr++) {
		if(*curptr & 0x80) {
			meetmsb = PR_TRUE;
		}
		else if(*curptr < 'A' || (*curptr > 'Z' && *curptr < 'a') || 
		        *curptr > 'z') {
			/* current char is a symbol, most likely a punctuation.
			** we treat it as a segment delimiter */
			if(meetmsb && curptr > prevptr) {
				/* this segment contains more than a single symbol, and it has
				** upper ascii, we need to keep it */
				while(prevptr < curptr) *newptr++ = *prevptr++;
				prevptr++;
				*newptr++ = ' ';
				meetmsb = PR_FALSE;
			}
			else {
				/* ignore the current segment, either because it is just a
				** symbol or just an english word */
				prevptr = curptr + 1;
			}
		}
	}

	*newlen = newptr - *newbuf;

	return PR_TRUE;
}


/* This filter applies to all scripts that do use latin letters */
PRBool sbcs_group_filterwithengletters (const char* abuf, PRUint32 alen,
                                        char** newbuf, PRUint32* newlen) {

	/* do filtering to reduce load to probers */
	char* newptr;
	char* prevptr;
	char* curptr;

	PRBool isintag = PR_FALSE;

	newptr = *newbuf = (char*)PR_MALLOC(alen);
	if(!newptr) {
		return PR_FALSE;
	}

	for(curptr = prevptr = (char*)abuf; curptr < abuf + alen; curptr++) {
		if(*curptr = '>') {
			isintag = PR_FALSE;
		}
		else if(*curptr == '<') {
			isintag = PR_TRUE;
		}

		if(!(*curptr & 0x80 && (*curptr > 'Z' && *curptr < 'a') ||
		   *curptr > 'z')) {
			/* current segment contains more than just a symbol 
			** and it is not inside a tag, keep it */
			if(curptr > prevptr && !isintag) {
				while(prevptr < curptr) *newptr++ = *prevptr++;
				prevptr++;
				*newptr++ = ' ';
			}
			else {
				prevptr = curptr + 1;
			}
		}
	}

	*newlen = newptr - *newbuf;

	return PR_TRUE;
}
