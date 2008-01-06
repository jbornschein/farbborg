#include "zImage.h"

#define N		 4096	/* size of ring buffer */
#define F		   18	/* upper limit for match_length */
#define THRESHOLD	2   /* encode string into position and length
						   if match_length is greater than this */
#define NIL			N	/* index for root of binary search trees */

unsigned long int
		textsize = 0,	/* text size counter */
		codesize = 0,	/* code size counter */
		printcount = 0;	/* counter for reporting progress every 1K bytes */
unsigned char
		text_buf[N + F - 1];	/* ring buffer of size N,
			with extra F-1 bytes to facilitate string comparison */
//int		match_position, match_length,  /* of longest match.  These are
//			set by the InsertNode() procedure. 
//		lson[N + 1], rson[N + 257], dad[N + 1];  /* left & right children &
//			parents -- These constitute binary search trees.

#define RAMSTART (0x40000000 + 1024*(512 - 16))

char * pIn  = zImage;
char * pInEnd = zImage + Z_IMAGE_SIZE;
char * pOut = (char *) RAMSTART;

//void InitTree(void)  /* initialize trees */
//{
//	int  i;

	/* For i = 0 to N - 1, rson[i] and lson[i] will be the right and
	   left children of node i.  These nodes need not be initialized.
	   Also, dad[i] is the parent of node i.  These are initialized to
	   NIL (= N), which stands for 'not used.'
	   For i = 0 to 255, rson[N + i + 1] is the root of the tree
	   for strings that begin with character i.  These are initialized
	   to NIL.  Note there are 256 trees. */

//	for (i = N + 1; i <= N + 256; i++) rson[i] = NIL;
//	for (i = 0; i < N; i++) dad[i] = NIL;
//}

//void InsertNode(int r)
	/* Inserts string of length F, text_buf[r..r+F-1], into one of the
	   trees (text_buf[r]'th tree) and returns the longest-match position
	   and length via the global variables match_position and match_length.
	   If match_length = F, then removes the old node in favor of the new
	   one, because the old one will be deleted sooner.
	   Note r plays double role, as tree node and position in buffer. */
//{
/*	int  i, p, cmp;
	unsigned char  *key;

	cmp = 1;  key = &text_buf[r];  p = N + 1 + key[0];
	rson[r] = lson[r] = NIL;  match_length = 0;
	for ( ; ; ) {
		if (cmp >= 0) {
			if (rson[p] != NIL) p = rson[p];
			else {  rson[p] = r;  dad[r] = p;  return;  }
		} else {
			if (lson[p] != NIL) p = lson[p];
			else {  lson[p] = r;  dad[r] = p;  return;  }
		}
		for (i = 1; i < F; i++)
			if ((cmp = key[i] - text_buf[p + i]) != 0)  break;
		if (i > match_length) {
			match_position = p;
			if ((match_length = i) >= F)  break;
		}
	}
	dad[r] = dad[p];  lson[r] = lson[p];  rson[r] = rson[p];
	dad[lson[p]] = r;  dad[rson[p]] = r;
	if (rson[dad[p]] == p) rson[dad[p]] = r;
	else                   lson[dad[p]] = r;
	dad[p] = NIL; */  /* remove p */
//}
/*
void DeleteNode(int p)  *//* deletes node p from tree */
/*{
	int  q;
	
	if (dad[p] == NIL) return; */ /* not in tree */ /*
	if (rson[p] == NIL) q = lson[p];
	else if (lson[p] == NIL) q = rson[p];
	else {
		q = lson[p];
		if (rson[q] != NIL) {
			do {  q = rson[q];  } while (rson[q] != NIL);
			rson[dad[q]] = lson[q];  dad[lson[q]] = dad[q];
			lson[q] = lson[p];  dad[lson[p]] = q;
		}
		rson[q] = rson[p];  dad[rson[p]] = q;
	}
	dad[q] = dad[p];
	if (rson[dad[p]] == p) rson[dad[p]] = q;  else lson[dad[p]] = q;
	dad[p] = NIL;
} */

void Decode(void)	/* Just the reverse of Encode(). */
{
	int  i, j, k, r, c;
	unsigned int  flags;
	
	for (i = 0; i < N - F; i++)
		text_buf[i] = ' ';
	r = N - F;  
	flags = 0;
	for ( ; ; ) {
		if (((flags >>= 1) & 256) == 0) {
			c = *pIn++;
			if (pIn == pInEnd)
				break;
			flags = c | 0xff00;		/* uses higher byte cleverly */
		}							/* to count eight */
		if (flags & 1) {
			c = *pIn++;
			if (pIn == pInEnd)
				break;
			*pOut++ =  c;  
			text_buf[r++] = c;  
			r &= (N - 1);
		} else {
			i = *pIn++;
			if (pIn == pInEnd) 
				break;
			j = *pIn++;
			if (pIn == pInEnd) 
				break;
			i |= ((j & 0xf0) << 4);  
			j = (j & 0x0f) + THRESHOLD;
			for (k = 0; k <= j; k++) {
				c = text_buf[(i + k) & (N - 1)];
				*pOut++ =  c;
				text_buf[r++] = c;
				r &= (N - 1);
			}
		}
	}
}

void jump(unsigned int addr);

int main() {
	textsize = 0;
	codesize = 0;
	printcount = 0;
	pIn  = zImage;
	pInEnd = zImage + Z_IMAGE_SIZE;
	pOut = (char *) RAMSTART;
	
	Decode();
	jump(RAMSTART);
	return 0;
}
