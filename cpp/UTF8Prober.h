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

#ifndef UTF8Prober_h__
#define UTF8Prober_h__

#include "charsetProber.h"
#include "codingStateMachine.h"

class UTF8Prober: public CharSetProber {
public:
  UTF8Prober(){mNumOfMBChar = 0; 
                mCodingSM = new CodingStateMachine(&UTF8SMModel);
                Reset(); };
  virtual ~UTF8Prober(){delete mCodingSM;};
  ProbingState HandleData(const char* aBuf, PRUint32 aLen);
  const char* GetCharSetName() {return "UTF-8";};
  ProbingState GetState(void) {return mState;};
  void      Reset(void);
  float     GetConfidence(void);
  void      SetOpion() {};

protected:
  CodingStateMachine* mCodingSM;
  ProbingState mState;
  PRUint32 mNumOfMBChar;
};

#endif /* UTF8Prober_h__ */
