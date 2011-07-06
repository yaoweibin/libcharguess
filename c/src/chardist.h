#ifndef __chardist_h
#define __chardist_h

#include "types.h"

#define ENOUGH_DATA_THRESHOLD 1024

typedef enum {
	EUCTW,
	EUCKR,
	GB2312,
	Big5,
	SJIS,
	EUCJP
} disttype;

typedef struct {
	PRBool done;
	PRUint32 freqchars;
	PRUint32 totalchars;
	PRUint32 tablesize;
	float typicaldistratio;
	disttype charset;
	const PRInt16* chartofreqorder;
} chardist;

int chardist_analysis_init(chardist* THIS);
int chardist_analysis_reset(chardist* THIS);
int chardist_analysis_destroy(chardist* THIS);
void chardist_analysis_handledata(chardist* THIS, const char* abuf, 
                                  PRUint32 alen);
void chardist_analysis_handleonechar(chardist* THIS, const char* astr,
                                     PRUint32 acharlen);
float chardist_analysis_getconfidence(chardist* THIS);
PRBool chardist_analysis_gotenoughdata(chardist* THIS);


/* EUCTW function wrappers and prototypes */
typedef chardist EUCTWdist;
int EUCTWdist_analysis_init(EUCTWdist* THIS);
#define EUCTWdist_analysis_reset chardist_analysis_reset
#define EUCTWdist_analysis_destroy chardist_analysis_destroy
#define EUCTWdist_analysis_handledata chardist_analysis_handledata
#define EUCTWdist_analysis_handleonechar chardist_analysis_handleonechar
#define EUCTWdist_analysis_getconfidence chardist_analysis_getconfidence
#define EUCTWdist_analysis_gotenoughdata chardist_analysis_gotenoughdata
PRInt32 EUCTWdist_analysis_getorder(const char* str);


/* EUCKR function wrappers and prototypes */
typedef chardist EUCKRdist;
int EUCKRdist_analysis_init(EUCKRdist* THIS);
#define EUCKRdist_analysis_reset chardist_analysis_reset
#define EUCKRdist_analysis_destroy chardist_analysis_destroy
#define EUCKRdist_analysis_handledata chardist_analysis_handledata
#define EUCKRdist_analysis_handleonechar chardist_analysis_handleonechar
#define EUCKRdist_analysis_getconfidence chardist_analysis_getconfidence
#define EUCKRdist_analysis_gotenoughdata chardist_analysis_gotenoughdata
PRInt32 EUCKRdist_analysis_getorder(const char* str);


/* GB2312 function wrappers and prototypes */
typedef chardist GB2312dist;
int GB2312dist_analysis_init(GB2312dist* THIS);
#define GB2312dist_analysis_reset chardist_analysis_reset
#define GB2312dist_analysis_destroy chardist_analysis_destroy
#define GB2312dist_analysis_handledata chardist_analysis_handledata
#define GB2312dist_analysis_handleonechar chardist_analysis_handleonechar
#define GB2312dist_analysis_getconfidence chardist_analysis_getconfidence
#define GB2312dist_analysis_gotenoughdata chardist_analysis_gotenoughdata
PRInt32 GB2312dist_analysis_getorder(const char* str);


/* Big5 function wrappers and prototypes */
typedef chardist Big5dist;
int Big5dist_analysis_init(Big5dist* THIS);
#define Big5dist_analysis_reset chardist_analysis_reset
#define Big5dist_analysis_destroy chardist_analysis_destroy
#define Big5dist_analysis_handledata chardist_analysis_handledata
#define Big5dist_analysis_handleonechar chardist_analysis_handleonechar
#define Big5dist_analysis_getconfidence chardist_analysis_getconfidence
#define Big5dist_analysis_gotenoughdata chardist_analysis_gotenoughdata
PRInt32 Big5dist_analysis_getorder(const char* str);


/* SJIS function wrappers and prototypes */
typedef chardist SJISdist;
int SJISdist_analysis_init(SJISdist* THIS);
#define SJISdist_analysis_reset chardist_analysis_reset
#define SJISdist_analysis_destroy chardist_analysis_destroy
#define SJISdist_analysis_handledata chardist_analysis_handledata
#define SJISdist_analysis_handleonechar chardist_analysis_handleonechar
#define SJISdist_analysis_getconfidence chardist_analysis_getconfidence
#define SJISdist_analysis_gotenoughdata chardist_analysis_gotenoughdata
PRInt32 SJISdist_analysis_getorder(const char* str);


/* EUCJP function wrappers and prototypes */
typedef chardist EUCJPdist;
int EUCJPdist_analysis_init(EUCJPdist* THIS);
#define EUCJPdist_analysis_reset chardist_analysis_reset
#define EUCJPdist_analysis_destroy chardist_analysis_destroy
#define EUCJPdist_analysis_handledata chardist_analysis_handledata
#define EUCJPdist_analysis_handleonechar chardist_analysis_handleonechar
#define EUCJPdist_analysis_getconfidence chardist_analysis_getconfidence
#define EUCJPdist_analysis_gotenoughdata chardist_analysis_gotenoughdata
PRInt32 EUCJPdist_analysis_getorder(const char* str);

#endif /* __chardist_h */
