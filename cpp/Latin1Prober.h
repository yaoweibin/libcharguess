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

#ifndef Latin1Prober_h__
#define Latin1Prober_h__

#include "charsetProber.h"

#define FREQ_CAT_NUM    4

class Latin1Prober: public CharSetProber {
public:
  Latin1Prober(void){Reset();};
  virtual ~Latin1Prober(void){};
  ProbingState HandleData(const char* aBuf, PRUint32 aLen);
  const char* GetCharSetName() {return "windows-1252";};
  ProbingState GetState(void) {return mState;};
  void      Reset(void);
  float     GetConfidence(void);
  void      SetOpion() {};

protected:
  PRBool FilterWithEnglishLetters(const char* aBuf, PRUint32 aLen, char** newBuf, PRUint32& newLen);
  
  ProbingState mState;
  char mLastCharClass;
  PRUint32 mFreqCounter[FREQ_CAT_NUM];
};


#endif /* Latin1Prober_h__ */

