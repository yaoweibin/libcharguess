#include "latin1.h"

#define UDF    0        /*  undefined */
#define OTH    1        /* other */
#define ASC    2        /*  ascii capital letter */
#define ASS    3        /*  ascii small letter */
#define ACV    4        /*  accent capital vowel */
#define ACO    5        /*  accent capital other */
#define ASV    6        /*  accent small vowel */
#define ASO    7        /*  accent small other */
#define CLASS_NUM   8    /*  total classes */

static unsigned char latin1_chartoclass[] =
{
  OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   /*  00 - 07 */
  OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   /*  08 - 0F */
  OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   /*  10 - 17 */
  OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   /*  18 - 1F */
  OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   /*  20 - 27 */
  OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   /*  28 - 2F */
  OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   /*  30 - 37 */
  OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   /*  38 - 3F */
  OTH, ASC, ASC, ASC, ASC, ASC, ASC, ASC,   /*  40 - 47 */
  ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC,   /*  48 - 4F */
  ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC,   /*  50 - 57 */
  ASC, ASC, ASC, OTH, OTH, OTH, OTH, OTH,   /*  58 - 5F */
  OTH, ASS, ASS, ASS, ASS, ASS, ASS, ASS,   /*  60 - 67 */
  ASS, ASS, ASS, ASS, ASS, ASS, ASS, ASS,   /*  68 - 6F */
  ASS, ASS, ASS, ASS, ASS, ASS, ASS, ASS,   /*  70 - 77 */
  ASS, ASS, ASS, OTH, OTH, OTH, OTH, OTH,   /*  78 - 7F */
  OTH, UDF, OTH, ASO, OTH, OTH, OTH, OTH,   /*  80 - 87 */
  OTH, OTH, ACO, OTH, ACO, UDF, ACO, UDF,   /*  88 - 8F */
  UDF, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   /*  90 - 97 */
  OTH, OTH, ASO, OTH, ASO, UDF, ASO, ACO,   /*  98 - 9F */
  OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   /*  A0 - A7 */
  OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   /*  A8 - AF */
  OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   /*  B0 - B7 */
  OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   /*  B8 - BF */
  ACV, ACV, ACV, ACV, ACV, ACV, ACO, ACO,   /*  C0 - C7 */
  ACV, ACV, ACV, ACV, ACV, ACV, ACV, ACV,   /*  C8 - CF */
  ACO, ACO, ACV, ACV, ACV, ACV, ACV, OTH,   /*  D0 - D7 */
  ACV, ACV, ACV, ACV, ACV, ACO, ACO, ACO,   /*  D8 - DF */
  ASV, ASV, ASV, ASV, ASV, ASV, ASO, ASO,   /*  E0 - E7 */
  ASV, ASV, ASV, ASV, ASV, ASV, ASV, ASV,   /*  E8 - EF */
  ASO, ASO, ASV, ASV, ASV, ASV, ASV, OTH,   /*  F0 - F7 */
  ASV, ASV, ASV, ASV, ASV, ASO, ASO, ASO,   /*  F8 - FF */
};


/* 0 : illegal 
   1 : very unlikely 
   2 : normal 
   3 : very likely
*/
static unsigned char latin1_classmodel[] =
{
/*      UDF OTH ASC ASS ACV ACO ASV ASO  */
/*UDF*/  0,  0,  0,  0,  0,  0,  0,  0,
/*OTH*/  0,  3,  3,  3,  3,  3,  3,  3,
/*ASC*/  0,  3,  3,  3,  3,  3,  3,  3,
/*ASS*/  0,  3,  3,  3,  1,  1,  3,  3,
/*ACV*/  0,  3,  3,  3,  1,  2,  1,  2,
/*ACO*/  0,  3,  3,  3,  3,  3,  3,  3,
/*ASV*/  0,  3,  1,  3,  1,  1,  1,  3,
/*ASO*/  0,  3,  1,  3,  1,  1,  3,  3,
};



int latin1_init(latin1prober* THIS) {

	return latin1_reset(THIS);

}

int latin1_reset(latin1prober* THIS) {

	PRUint32 i;

	THIS->state = eDetecting;
	THIS->lastcharclass = OTH;
	for (i = 0; i < FREQ_CAT_NUM; i++) {
		THIS->freqcounter[i] = 0;
	}

	return 1;
}

float latin1_getconfidence(latin1prober* THIS) {

	float confidence;
	PRUint32 total = 0, i;

	if (THIS->state == eNotMe) {
		return 0.01f;
	}

	for (i = 0; i < FREQ_CAT_NUM; i++) {
		total += THIS->freqcounter[i];
	}

	confidence = THIS->freqcounter[3] * 1.0f / total;
	confidence -= THIS->freqcounter[1] * 20.0f / total;

	if (confidence < 0.0f) {
		confidence = 0.0f;
	}

	/* lower the confidence of latin1 so that other more accurate detector
    ** can take priority. */
	confidence = 0.50f;

	return confidence;

}

const char* latin1_getcharsetname(latin1prober* THIS) {

	return "windows-1252";
}

/* place holder function to clean up structures */
int latin1_destroy(latin1prober* THIS) {

	return 1;
}

probingstate latin1_handledata(latin1prober* THIS, 
                               const char* abuf, PRUint32 alen) {

	char *newbuf;
	PRUint32 newlen, i;
	
	unsigned char charclass;
	unsigned char freq;

	if(!latin1_filterwithenglishletters(abuf, alen, &newbuf, &newlen)) {
		newbuf = (char*)abuf;
		newlen = alen;
	}

	for (i = 0; i < newlen; i++) {
		charclass = latin1_chartoclass[(unsigned char)newbuf[i]];
		freq = latin1_classmodel[THIS->lastcharclass * CLASS_NUM + charclass];
		if (freq == 0) {
			THIS->state = eNotMe;
			break;
		}
		THIS->freqcounter[freq]++;
		THIS->lastcharclass = charclass;
	}

	if (newbuf != abuf) {
		PR_FREEIF(newbuf);
	}

	return THIS->state;
}


PRBool latin1_filterwithenglishletters(const char* abuf, PRUint32 alen, 
                                       char** newbuf, PRUint32* newlen) {

	/* Do filtering to reduce the load the the probers */
	char* newptr;
	char* prevptr;
	char* curptr;

	newptr = *newbuf = (char*)PR_MALLOC(alen);
	if (!newptr) {
		return PR_FALSE;
	}

	/* DANGEROUS ptr arithmetic, find cleaner solution */
	for (curptr = prevptr = (char*)abuf; curptr < abuf + alen; curptr++) {
		
		if(!(*curptr & 0x80) &&
		  (*curptr < 'A' || (*curptr > 'Z' && *curptr < 'a' || *curptr >'z'))) {
			/* current segment contains more than just a symbol, keep it */
			if (curptr > prevptr) {
				while(prevptr < curptr) *newptr++ = *prevptr++;
				prevptr++;
				*newptr++ = ' '; /* this code is also ambiguous */
			}
			else {
				prevptr = curptr + 1;
			}
		}
	}

	/* more ptr arithmetic, I'm in hell ;) */
	*newlen = newptr - *newbuf;

	return PR_TRUE;

}

