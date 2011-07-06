#ifndef __gb2312_h
#define __gb2312_h

#include "types.h"
#include "statemachine.h"
#include "chardist.h"

/* we use gb18030 to replace gb2312 because 18030 is a superset. */

typedef struct {
	statemachine* codingsm;
	probingstate state;
	GB2312dist distanalyser;
	char lastchar[2];
} gb18030prober;

int gb18030_init(gb18030prober* THIS);
int gb18030_reset(gb18030prober* THIS);
int gb18030_destroy(gb18030prober* THIS);
probingstate gb18030_handledata(gb18030prober* THIS, const char* abuf,
                                PRUint32 alen);
const char* gb18030_getcharsetname(gb18030prober* THIS);
probingstate gb18030_getstate(gb18030prober* THIS);
float gb18030_getconfidence(gb18030prober* THIS);

#endif /* __gb2312_h */
