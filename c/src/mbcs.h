#ifndef __mbcs_h
#define __mbcs_h

#include "sjis.h"
#include "utf8.h"
#include "eucjp.h"
#include "gb2312.h"
#include "euckr.h"
#include "big5.h"
#include "euctw.h"

#define NUM_OF_PROBERS 7

typedef enum {
    sjis    = 0,
    utf8    = 1,
    eucjp   = 2,
    gb18030 = 3,
    euckr   = 4,
    big5    = 5,
    euctw   = 6,
    none    = 7,
} mbcsnames;

typedef struct {
	sjisprober sjis;
	utf8prober utf8;
	eucjpprober eucjp;
	gb18030prober gb18030;
	euckrprober euckr;
	big5prober big5;
	euctwprober euctw;
} mbcsgroup;

typedef struct {
	probingstate state;
	mbcsnames bestguess;
	PRUint32 activenum;
	mbcsgroup probers;
	PRBool isactive[NUM_OF_PROBERS];
} mbcsprober;


int mbcs_init(mbcsprober* THIS);
int mbcs_reset(mbcsprober* THIS);
int mbcs_destroy(mbcsprober* THIS);
probingstate mbcs_handledata(mbcsprober* THIS, const char* abuf,
                             PRUint32 alen);
const char* mbcs_getcharsetname(mbcsprober* THIS);
probingstate mbcs_getstate(mbcsprober* THIS);
float mbcs_getconfidence(mbcsprober* THIS);

#endif /* __mbcs_h */
