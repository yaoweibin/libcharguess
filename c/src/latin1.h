#ifndef __latin1_h
#define __latin1_h

#include "types.h"

#define FREQ_CAT_NUM 4


typedef struct {
	char lastcharclass;
	PRUint32 freqcounter[FREQ_CAT_NUM];
	probingstate state;
} latin1prober;

int latin1_init(latin1prober* THIS);
int latin1_reset(latin1prober* THIS);
float latin1_getconfidence(latin1prober* THIS);
const char* latin1_getcharsetname(latin1prober* THIS);
int latin1_destroy(latin1prober* THIS);
probingstate latin1_handledata(latin1prober* THIS, const char* abuf, PRUint32 alen);

PRBool latin1_filterwithenglishletters(const char* abuf, PRUint32 alen, char** newbuf, PRUint32* newlen);

#endif /* __latin_h */
