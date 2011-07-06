
#include "chardist.h"

#include "tables/JISFreq.h"
#include "tables/Big5Freq.h"
#include "tables/EUCKRFreq.h"
#include "tables/EUCTWFreq.h"
#include "tables/GB2312Freq.h"

#define SURE_YES 0.99f
#define SURE_NO  0.01f

int chardist_analysis_init(chardist* THIS) {
	return chardist_analysis_reset(THIS);
}

int chardist_analysis_reset(chardist* THIS) {
	THIS->done = PR_FALSE;
	THIS->totalchars = 0;
	THIS->freqchars = 0;

	return 1;
}

int chardist_analysis_destroy(chardist* THIS) {
	return 1;
}

void chardist_analysis_handledata(chardist* THIS, const char* abuf,
                                  PRUint32 alen) {
	return;
}

void chardist_analysis_handleonechar(chardist* THIS, const char* astr,
                                     PRUint32 acharlen) {
	PRInt32 order;

	/* we only care about 2-byte characters in our distribution analysis */
	switch(THIS->charset) {
		case EUCTW:
			order = (acharlen == 2) ? EUCTWdist_analysis_getorder(astr) : -1;
			break;
		case EUCKR:
			order = (acharlen == 2) ? EUCKRdist_analysis_getorder(astr) : -1;
			break;
		case GB2312:
			order = (acharlen == 2) ? GB2312dist_analysis_getorder(astr) : -1;
			break;
		case Big5:
			order = (acharlen == 2) ? Big5dist_analysis_getorder(astr) : -1;
			break;
		case SJIS:
			order = (acharlen == 2) ? SJISdist_analysis_getorder(astr) : -1;
			break;
		case EUCJP:
			order = (acharlen == 2) ? EUCJPdist_analysis_getorder(astr) : -1;
			break;
	}

	if(order >= 0) {
		THIS->totalchars++;
		/* order is valid */
		if((PRUint32)order < THIS->tablesize) {
			if(512 > THIS->chartofreqorder[order]) THIS->freqchars++;
		}
	}
}

float chardist_analysis_getconfidence(chardist* THIS) {
	
	float r;

	/* if we didn't receive any character in our consideration range,
	** return a negative answer */
	if(THIS->totalchars <= 0) {
		return SURE_NO;
	}

	if(THIS->totalchars != THIS->freqchars) {
		r = THIS->freqchars / ((THIS->totalchars - THIS->freqchars) *
		                       THIS->typicaldistratio);
		if(r < SURE_YES) {
			return r;
		}
	}
	/* normalize the confidence (we don't want to be 100% sure) */
	return SURE_YES;
}

PRBool chardist_analysis_gotenoughdata(chardist* THIS) {
	return THIS->totalchars > ENOUGH_DATA_THRESHOLD;
}


int EUCTWdist_analysis_init(EUCTWdist* THIS) {
	THIS->charset = EUCTW;
	THIS->chartofreqorder = EUCTWCharToFreqOrder;
	THIS->tablesize = EUCTW_TABLE_SIZE;
	THIS->typicaldistratio = EUCTW_TYPICAL_DISTRIBUTION_RATIO;
	return chardist_analysis_init((chardist*)THIS);
}

PRInt32 EUCTWdist_analysis_getorder(const char* str) {
	/* For euc-TW encoding, we are interested in
	** first  byte range: 0xc4 -- 0xfe
	** second byte range: 0xa1 -- 0xfe
	** No validation is needed. The state machine has done that */
	if((unsigned char)*str >= (unsigned char)0xc4) {
		return 94 * ((unsigned char)str[0] - (unsigned char)0xc4) +
               (unsigned char)str[1] - (unsigned char)0xa1;
	}
	else {
		return -1;
	}
}


int EUCKRdist_analysis_init(EUCKRdist* THIS) {
	THIS->charset = EUCKR;
	THIS->chartofreqorder = EUCKRCharToFreqOrder;
	THIS->tablesize = EUCKR_TABLE_SIZE;
	THIS->typicaldistratio = EUCKR_TYPICAL_DISTRIBUTION_RATIO;
	return chardist_analysis_init((chardist*)THIS);
}

PRInt32 EUCKRdist_analysis_getorder(const char* str) {
	/* For euc-KR encoding, we are interested in
	** first  byte range: 0xb0 -- 0xfe
	** second byte range: 0xa1 -- 0xfe
	** No validation is needed. The state machine has done that */
	if((unsigned char)*str >= (unsigned char)0xb0) {
		return 94 * ((unsigned char)str[0] - (unsigned char)0xb0) +
		       (unsigned char)str[1] - (unsigned char)0xa1;
	}
	else {
		return -1;
	}
}


int GB2312dist_analysis_init(GB2312dist* THIS) {
	THIS->charset = GB2312;
	THIS->chartofreqorder = GB2312CharToFreqOrder;
	THIS->tablesize = GB2312_TABLE_SIZE;
	THIS->typicaldistratio = GB2312_TYPICAL_DISTRIBUTION_RATIO;
	return chardist_analysis_init((chardist*)THIS);
}

PRInt32 GB2312dist_analysis_getorder(const char* str) {
	/* For GB2312 encoding, we are interested in
	** first  byte range: 0xb0 -- 0xfe
	** second byte range: 0xa1 -- 0xfe
	** No validation is needed. The state machine has done that */
	if((unsigned char)*str >= (unsigned char)0xb0 &&
	   (unsigned char)str[1] >= (unsigned char)0xa1) {
	   	return 94 * ((unsigned char)str[0] - (unsigned char)0xb0) +
		       (unsigned char)str[1] - (unsigned char)0xa1;
	}
	else {
		return -1;
	}
}


int Big5dist_analysis_init(Big5dist* THIS) {
	THIS->charset = Big5;
	THIS->chartofreqorder = Big5CharToFreqOrder;
	THIS->tablesize = BIG5_TABLE_SIZE;
	THIS->typicaldistratio = BIG5_TYPICAL_DISTRIBUTION_RATIO;
	return chardist_analysis_init((chardist*)THIS);
}

PRInt32 Big5dist_analysis_getorder(const char* str) {
	/* For big5 encoding, we are interested in
	** first  byte range: 0xa4 -- 0xfe
	** second byte range: 0x40 -- 0x7e, 0xa1 -- 0xfe
	** No validation is needed. The state machine has done that */
	if((unsigned char)*str >= (unsigned char)0xa4) {
		if((unsigned char)str[1] >= (unsigned char)0xa1) {
			return 157 * ((unsigned char)str[0] - (unsigned char)0xa4) +
			       (unsigned char)str[1] - (unsigned char)0xa1 + 63; 
		}
		else {
			return 157 * ((unsigned char)str[0] - (unsigned char)0xa4) +
			       (unsigned char)str[1] - (unsigned char)0xa1 + 0x40;
		}
	}
	else {
		return -1;
	}
}


int SJISdist_analysis_init(SJISdist* THIS) {
	THIS->charset = SJIS;
	THIS->chartofreqorder = JISCharToFreqOrder;
	THIS->tablesize = JIS_TABLE_SIZE;
	THIS->typicaldistratio = JIS_TYPICAL_DISTRIBUTION_RATIO;
	return chardist_analysis_init((chardist*)THIS);
}

PRInt32 SJISdist_analysis_getorder(const char* str) {
	/* For sjis encoding, we are interested in
	** first  byte range: 0x81 -- 0x9f, 0xe0 -- 0xfe
	** second byte range: 0x40 -- 0x7e, 0x81 -- 0xfe
	** No validation is needed. The state machine has done that */
	PRInt32 order;
	if((unsigned char)*str >= (unsigned char)0x81 &&
	   (unsigned char)*str <= (unsigned char)0x9f) {
		order = 188 * ((unsigned char)str[0] - (unsigned char)0x81);
	}
	else if ((unsigned char)*str >= (unsigned char)0xe0 &&
	         (unsigned char)*str <= (unsigned char)0xef) {
		order = 188 * ((unsigned char)str[0] - (unsigned char)0xe0 + 31);
	}
	else {
		return -1;
	}
	order += (unsigned char)*(str+1) - 0x40;
	if((unsigned char)str[1] > (unsigned char)0x7f) {
		order--;
	}
	return order;
}


int EUCJPdist_analysis_init(EUCJPdist* THIS) {
	THIS->charset = EUCJP;
	THIS->chartofreqorder = JISCharToFreqOrder;
	THIS->tablesize = JIS_TABLE_SIZE;
	THIS->typicaldistratio = JIS_TYPICAL_DISTRIBUTION_RATIO;
	return chardist_analysis_init((chardist*)THIS);
}

PRInt32 EUCJPdist_analysis_getorder(const char* str) {
	/* For euc-JP encoding, we are interested in
	** first  byte range: 0xa0 -- 0xfe
	** second byte range: 0xa1 -- 0xfe
	** No validation is needed. The state machine has done that */
	if((unsigned char)*str >= (unsigned char)0xa0) {
		return 94 * ((unsigned char)str[0] - (unsigned char)0xa1) +
		       (unsigned char)str[1] - (unsigned char)0xa1;
	}
	else {
		return -1;
	}
}
