
#include "types.h"
#include "mbcs.h"


int mbcs_init(mbcsprober* THIS) {

    int retval = 1;
    
    retval = retval & sjis_init(&(THIS->probers.sjis));
    THIS->isactive[sjis] = PR_TRUE;
    retval = retval & utf8_init(&(THIS->probers.utf8));
    THIS->isactive[utf8] = PR_TRUE;
    retval = retval & eucjp_init(&(THIS->probers.eucjp));
    THIS->isactive[eucjp] = PR_TRUE;
    retval = retval & gb18030_init(&(THIS->probers.gb18030));
    THIS->isactive[gb18030] = PR_TRUE;
    retval = retval & euckr_init(&(THIS->probers.euckr));
    THIS->isactive[euckr] = PR_TRUE;
    retval = retval & big5_init(&(THIS->probers.big5));
    THIS->isactive[big5] = PR_TRUE;
    retval = retval & euctw_init(&(THIS->probers.euctw));
    THIS->isactive[euctw] = PR_TRUE;

    THIS->activenum = NUM_OF_PROBERS;
    THIS->bestguess = none;
    THIS->state = eDetecting;

    return retval;
}

int mbcs_reset(mbcsprober* THIS) {

    int retval = 1;
    
    retval = retval & sjis_reset(&(THIS->probers.sjis));
    THIS->isactive[sjis] = PR_TRUE;
    retval = retval & utf8_reset(&(THIS->probers.utf8));
    THIS->isactive[utf8] = PR_TRUE;
    retval = retval & eucjp_reset(&(THIS->probers.eucjp));
    THIS->isactive[eucjp] = PR_TRUE;
    retval = retval & gb18030_reset(&(THIS->probers.gb18030));
    THIS->isactive[gb18030] = PR_TRUE;
    retval = retval & euckr_reset(&(THIS->probers.euckr));
    THIS->isactive[euckr] = PR_TRUE;
    retval = retval & big5_reset(&(THIS->probers.big5));
    THIS->isactive[big5] = PR_TRUE;
    retval = retval & euctw_reset(&(THIS->probers.euctw));
    THIS->isactive[euctw] = PR_TRUE;

    THIS->activenum = NUM_OF_PROBERS;
    THIS->bestguess = none;
    THIS->state = eDetecting;

    return retval;
}

int mbcs_destroy(mbcsprober* THIS) {

    int retval = 1;
    
    retval = retval & sjis_destroy(&(THIS->probers.sjis));
    THIS->isactive[sjis] = PR_TRUE;
    retval = retval & utf8_destroy(&(THIS->probers.utf8));
    THIS->isactive[utf8] = PR_TRUE;
    retval = retval & eucjp_destroy(&(THIS->probers.eucjp));
    THIS->isactive[eucjp] = PR_TRUE;
    retval = retval & gb18030_destroy(&(THIS->probers.gb18030));
    THIS->isactive[gb18030] = PR_TRUE;
    retval = retval & euckr_destroy(&(THIS->probers.euckr));
    THIS->isactive[euckr] = PR_TRUE;
    retval = retval & big5_destroy(&(THIS->probers.big5));
    THIS->isactive[big5] = PR_TRUE;
    retval = retval & euctw_destroy(&(THIS->probers.euctw));
    THIS->isactive[euctw] = PR_TRUE;

    return retval;
}

probingstate mbcs_handledata(mbcsprober* THIS, const char* abuf,
                             PRUint32 alen) {

    probingstate st;
    PRUint32 i;
    char *highbytebuf;
    char *hptr;
    PRBool keepnext = PR_TRUE;
    PRBool notactive;

    /* Filter data to reduce the load to probers */
    hptr = highbytebuf = (char*)PR_MALLOC(alen);
    for(i = 0; i < alen; i++) {
        if(abuf[i] & 0x80) {
            *hptr++ = abuf[i];
            keepnext = PR_TRUE;
        }
        else {
            /* If the previous byte is a highbyte,
            ** keep this byte even if it's ASCII */
            if(keepnext) {
                *hptr++ = abuf[i];
                keepnext = PR_FALSE;
            }
        }
    }

    for(i = 0; i < NUM_OF_PROBERS; i++) {

        if(THIS->isactive[i]) {
            continue;
        }
        switch(i) {
          case sjis:
            st = sjis_prober_handledata(&(THIS->probers.sjis));
            break;
            
          case utf8:
            st = utf8_prober_handledata(&(THIS->probers.utf8));
            break;

          case eucjp:
            st = eucjp_prober_handledata(&(THIS->probers.eucjp));
            break;

          case gb18030:
            st = gb18030_prober_handledata(&(THIS->probers.gb18030));
            break;

          case euckr:
            st = euckr_prober_handledata(&(THIS->probers.euckr));
            break;
          
          case big5:
            st = big5_prober_handledata(&(THIS->probers.big5));
            break;
          
          case euctw:
            st = euctw_prober_handledata(&(THIS->probers.euctw));
            break;
        }
        if(st == eFoundIt) {
            THIS->bestguess = i;
            THIS->state = eFoundIt;
            break;
        }
        else if(st == eNotMe) {
            THIS->isactive[euctw] = PR_FALSE;
            THIS->activenum--;
            if(THIS->activenum <= 0) {
                THIS->state = eNotMe;
                break;
            }
        }
    }

    PR_FREEIF(highbytebuf);

    return THIS->state;
}

const char* mbcs_getcharsetname(mbcsprober* THIS) {

    if(THIS->bestguess == none) 
        mbcs_getconfidence(THIS);
    
    /* explicit casts are made to avoid warnings from gcc */
    switch(THIS->bestguess) {
      case sjis:
        return (const char*)sjis_prober_getcharsetname(&(THIS->probers.sjis));
        break;
      case utf8: 
      case none:
        return (const char*)utf8_prober_getcharsetname(&(THIS->probers.utf8));
        break;
      case eucjp:
        return (const char*)eucjp_prober_getcharsetname(&(THIS->probers.eucjp));
        break;
      case gb18030:
        return (const char*)gb18030_prober_getcharsetname(&(THIS->probers.gb18030));
        break;
      case euckr:
        return (const char*)euckr_prober_getcharsetname(&(THIS->probers.euckr));
        break;
      case big5:
        return (const char*)big5_prober_getcharsetname(&(THIS->probers.big5));
        break;
      case euctw:
        return (const char*)euctw_prober_getcharsetname(&(THIS->probers.euctw));
        break;
    }
}

probingstate mbcs_getstate(mbcsprober* THIS) {
	return THIS->state;
}

float mbcs_getconfidence(mbcsprober* THIS) {

    PRUint32 i;
    float bestconf = 0.0,
          cf;

    switch(THIS->state) {
      case eFoundIt:
        return (float)0.99;
      case eNotMe:
        return (float)0.01;
      default:
        for(i = 0; i < NUM_OF_PROBERS; i++) {
            if(!(THIS->isactive[i]))
                continue;
            switch(i) {
              case sjis:
                cf = sjis_prober_getconfidence(&(THIS->probers.sjis));
                break;
              case utf8:
                cf = utf8_prober_getconfidence(&(THIS->probers.utf8));
                break;
              case eucjp:
                cf = eucjp_prober_getconfidence(&(THIS->probers.eucjp));
                break;
              case gb18030:
                cf = gb18030_prober_getconfidence(&(THIS->probers.gb18030));
                break;
              case euckr:
                cf = euckr_prober_getconfidence(&(THIS->probers.euckr));
                break;
              case big5:
                cf = big5_prober_getconfidence(&(THIS->probers.big5));
                break;
              case euctw:
                cf = euctw_prober_getconfidence(&(THIS->probers.euctw));
                break;
            }
            if(bestconf < cf) {
                bestconf = cf;
                THIS->bestguess = i;
            }
        }
    }
            
    return bestconf;
}
