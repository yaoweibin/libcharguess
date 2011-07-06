/*
	libcharguess	-	Guess the encoding/charset of a string
    Copyright (C) 2003  Stephane Corbe <noubi@users.sourceforge.net>
	Based on Mozilla sources

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef EscCharSetProber_h__
#define EscCharSetProber_h__

#include "charsetProber.h"
#include "codingStateMachine.h"

#define NUM_OF_ESC_CHARSETS   4

class EscCharSetProber: public CharSetProber {
public:
  EscCharSetProber(void);
  virtual ~EscCharSetProber(void);
  ProbingState HandleData(const char* aBuf, PRUint32 aLen);
  const char* GetCharSetName() {return mDetectedCharset;};
  ProbingState GetState(void) {return mState;};
  void      Reset(void);
  float     GetConfidence(void){return (float)0.99;};
  void      SetOpion() {};

protected:
  void      GetDistribution(PRUint32 aCharLen, const char* aStr);
  
  CodingStateMachine* mCodingSM[NUM_OF_ESC_CHARSETS] ;
  PRUint32    mActiveSM;
  ProbingState mState;
  const char *  mDetectedCharset;
};

#endif /* EscCharSetProber_h__ */
