#ifndef __eucjp_h
#define __eucjp_h

/* for S-JIS encoding, observe characteristic:
** 1, kana character (or hankaku?) often have high frequencies of appearance
** 2, kana characters often exist in a group
** 3, certain combination of kana is never used in the japanese language */

#include "types.h"
#include "statemachine.h"
#include "jpcntx.h"
#include "chardist.h"

typedef struct {
	statemachine* codingsm;
	probingstate state;
	eucjpcontext_analysis contextanalyser;
	EUCJPdist distanalyser;
	char lastchar[2];
} eucjpprober;

int eucjp_init(eucjpprober* THIS);
int eucjp_reset(eucjpprober* THIS);
int eucjp_destroy(eucjpprober* THIS);
probingstate eucjp_handledata(eucjpprober* THIS, const char* abuf,
                              PRUint32 alen);
const char* eucjp_getcharsetname(eucjpprober* THIS);
probingstate eucjp_getstate(eucjpprober* THIS);
float eucjp_getconfidence(eucjpprober* THIS);

#endif /* __eucjp_h */
