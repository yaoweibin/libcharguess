#ifndef __sbcs_h
#define __sbcs_h

#include "types.h"

#define SAMPLE_SIZE 64
#define SB_ENOUGH_REL_THRESHOLD  1024
#define POSITIVE_SHORTCUT_THRESHOLD  (float)0.95
#define NEGATIVE_SHORTCUT_THRESHOLD  (float)0.05
#define SYMBOL_CAT_ORDER  250
#define NUMBER_OF_SEQ_CAT 4
#define POSITIVE_CAT   (NUMBER_OF_SEQ_CAT-1)
#define NEGATIVE_CAT   0


typedef struct {
	unsigned char* ordermap;       /* [256] teble used to find a char's order */
	char* precedence_matrix;       /* [SAMPLE_SIZE][SAMPLE_SIZE] table to find a
	                               ** 2 character sequence's frequency */
	float typical_positive_ratio;  /* equal to freq_seqs / total_seqs */
	PRBool keep_english_letters;   /* this script contains latin letters */
	const char* charset_name;
} sequencemodel;

typedef struct {
	probingstate state;
	sequencemodel* model;
	unsigned char lastorder;  /* character order of last character */
	PRUint32 totalseqs;
	PRUint32 seqcounters[NUMBER_OF_SEQ_CAT];
	PRUint32 totalchar;  /* characters that fall in our sampling range */
	PRUint32 freqchar;
} sbcsprober;

extern sequencemodel KOI8R_model;
extern sequencemodel win1251_model;
extern sequencemodel latin5_model;
extern sequencemodel maccyrillic_model;
extern sequencemodel IBM866_model;
extern sequencemodel IBM855_model;
extern sequencemodel latin7_model;
extern sequencemodel win1253_model;
extern sequencemodel latin5_bulgarian_model;
extern sequencemodel win1251_bulgarian_model;
extern sequencemodel latin2_hungarian_model;
extern sequencemodel win1250_hungarian_model;


int sbcs_init(sbcsprober* THIS, sequencemodel* model);
int sbcs_reset(sbcsprober* THIS);
float sbcs_getconfidence(sbcsprober* THIS);
const char* sbcs_getcharsetname(sbcsprober* THIS);
int sbcs_destroy(sbcsprober* THIS);
probingstate sbcs_handledata(sbcsprober* THIS, const char* abuf, PRUint32 alen);


#endif /* __sbcs_h */
