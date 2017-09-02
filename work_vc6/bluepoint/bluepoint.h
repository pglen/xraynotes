//# -------------------------------------------------------------------------
//# Bluepoint encryption routines.
//#
//#   How it works:
//#
//#     Strings are walked chr by char with the loop:
//#         {
//#         $aa = ord(substr($_[0], $loop, 1));
//#         do something with $aa
//#         substr($_[0], $loop, 1) = pack("c", $aa);
//#         }
//#
//#   Flow:
//#         generate vector
//#         generate pass

//#         walk forward with password cycling loop
//#         walk backwards with feedback encryption
//#         walk forward with feedback encryption
//#
//#  The process guarantees that a single bit change in the original text
//#  will change every byte in the resulting block.
//#
//#  The bit propagation is such a high quality, that it beats current
//#  industrial strength encryptions.
//#
//#  Please see bit distribution study.
//#
//# -------------------------------------------------------------------------

// Soft define

#ifndef ulong
typedef  unsigned long ulong;
#endif

#ifndef	ROTATE_LONG_RIGHT

#define     ROTATE_LONG_RIGHT(x, n) (((x) >> (n))  | ((x) << (32 - (n))))
#define     ROTATE_LONG_LEFT(x, n) (((x) << (n))  | ((x) >> (32 - (n))))
#define     ROTATE_CHAR_RIGHT(x, n) (((x) >> (n))  | ((x) << (8 - (n))))
#define     ROTATE_CHAR_LEFT(x, n) (((x) << (n))  | ((x) >> (8 - (n))))

#endif

// Soft define ends

#define PASSLIM  32						//# maximum key length (bytes)

void	bluepoint_encrypt(char *buff, int blen, const char *pass, int plen);
void	bluepoint_decrypt(char *str, int slen, const char *pass, int plen);
ulong   bluepoint_hash(char *buff, int blen);
ulong   bluepoint_crypthash(char *buff, int blen, char *pass, int plen);

#ifdef DEF_DUMPHEX
char 	*bluepoint_dumphex(const char *str, int len);
#endif

char    *bluepoint_tohex(char *str, int len, char *out, int *olen);
char    *bluepoint_fromhex(char *str, int len, char *out, int *olen);


