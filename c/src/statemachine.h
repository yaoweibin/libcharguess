#ifndef __statemachine_h
#define __statemachine_h

#include "pkgint.h"

typedef enum {
	eStart = 0,
	eError = 1,
	eItsMe = 2,
} smstate;

#define GETCLASS(c) GETFROMPCK(((unsigned char)(c)), THIS->model->classtable)

/* state machine model */
typedef struct {
	PkgInt classtable;
	PRUint32 classfactor;
	PkgInt statetable;
	const PRUint32* charlentable;
	const char* name;
} smmodel;

typedef struct {
	smstate currstate;
	PRUint32 currcharlen;
	PRUint32 currbytepos;
	smmodel* model;
} statemachine;

int statemachine_init(statemachine* THIS, smmodel* sm);
int statemachine_reset(statemachine* THIS);
int statemachine_destroy(statemachine* THIS);
smstate statemachine_nextstate(statemachine* THIS, char c);
PRUint32 statemachine_getcurrcharlen(statemachine* THIS);
const char* statemachine_getsmname(statemachine* THIS);


extern smmodel UTF8smmodel;
extern smmodel Big5smmodel;
extern smmodel EUCJPsmmodel;
extern smmodel EUCKRsmmodel;
extern smmodel EUCTWsmmodel;
extern smmodel GB18030smmodel;
extern smmodel SJISsmmodel;
extern smmodel UCS2BEsmmodel;

/* Found in escsm.c */
extern smmodel HZsmmodel;
extern smmodel ISO2022CNsmmodel;
extern smmodel ISO2022JPsmmodel;
extern smmodel ISO2022KRsmmodel;

#endif /* __statemachine_h */
