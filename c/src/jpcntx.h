#ifndef __jpcntx_h
#define __jpcntx_h

#define NUM_OF_CATEGORY 6

#include "types.h"

#define ENOUGH_REL_THRESHOLD 100
#define MAX_REL_THRESHOLD 1000

/* hiragana frequency catagory table */
extern char jp2char_context[83][83];

typedef struct {
	PRUint32 totalrel; /* total sequence received */
	PRInt32  lastcharorder; /* the order of the previous char */
	PRUint32 needtoskipnum; /* last byte in buffer not last byte of a char.
                            ** need to know # bytes to skip in next buffer */
	PRBool done; /* set to PR_TRUE when detection is done */
	PRBool issjis; /* set to PR_TRUE if using sjis, PR_FALSE if using EUCJP */
	PRUint32 relsample[NUM_OF_CATEGORY]; /* category counters */
} jpcontext_analysis;

int jpcontext_analysis_init(jpcontext_analysis* THIS);
int jpcontext_analysis_reset(jpcontext_analysis* THIS);
int jpcontext_analysis_destroy(jpcontext_analysis* THIS);
void jpcontext_analysis_handledata(jpcontext_analysis* THIS, const char* abuf,
                                   PRUint32 alen);
void jpcontext_analysis_handleonechar(jpcontext_analysis* THIS,
                                      const char* astr, PRUint32 acharlen);
float jpcontext_analysis_getconfidence(jpcontext_analysis* THIS);
PRBool jpcontext_analysis_gotenoughdata(jpcontext_analysis* THIS);



/* SJIS function wrappers and prototypes */
typedef jpcontext_analysis sjiscontext_analysis;
int sjiscontext_analysis_init(sjiscontext_analysis* THIS);
#define sjiscontext_analysis_reset jpcontext_analysis_reset
#define sjiscontext_analysis_destroy jpcontext_analysis_destroy
#define sjiscontext_analysis_handledata jpcontext_analysis_handledata
#define sjiscontext_analysis_handleonechar jpcontext_analysis_handleonechar
#define sjiscontext_analysis_getconfidence jpcontext_analysis_getconfidence
#define sjiscontext_analysis_gotenoughdata jpcontext_analysis_gotenoughdata
PRInt32 sjiscontext_analysis_ngetorder(const char* str, PRUint32 *charlen);
PRInt32 sjiscontext_analysis_getorder(const char* str);

/* EUCJP function wrappers and prototypes */
typedef jpcontext_analysis eucjpcontext_analysis;
int eucjpcontext_analysis_init(eucjpcontext_analysis* THIS);
#define eucjpcontext_analysis_reset jpcontext_analysis_reset
#define eucjpcontext_analysis_destroy jpcontext_analysis_destroy
#define eucjpcontext_analysis_handledata jpcontext_analysis_handledata
#define eucjpcontext_analysis_handleonechar jpcontext_analysis_handleonechar
#define eucjpcontext_analysis_getconfidence jpcontext_analysis_getconfidence
#define eucjpcontext_analysis_gotenoughdata jpcontext_analysis_gotenoughdata
PRInt32 eucjpcontext_analysis_ngetorder(const char* str, PRUint32 *charlen);
PRInt32 eucjpcontext_analysis_getorder(const char* str);


#endif /* __jpcntx_h */

