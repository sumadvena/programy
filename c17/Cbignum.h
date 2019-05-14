/* --------------------------------------------------------------
    Signed integers with unlimited range (version 2.1b).

    http://www.imach.uran.ru/cbignum

    Copyright 1999-2017 by Raul N.Shakirov, IMach of RAS(UB).
    All Rights Reserved.

    Permission has been granted to copy, distribute and modify
    software in any context without fee, including a commercial
    application, provided that the aforesaid copyright statement
    is present here as well as exhaustive description of changes.

    THE SOFTWARE IS DISTRIBUTED "AS IS". NO WARRANTY OF ANY KIND
    IS EXPRESSED OR IMPLIED. YOU USE AT YOUR OWN RISK. THE AUTHOR
    WILL NOT BE LIABLE FOR DATA LOSS, DAMAGES, LOSS OF PROFITS OR
    ANY OTHER KIND OF LOSS WHILE USING OR MISUSING THIS SOFTWARE.
-------------------------------------------------------------- */
#ifndef _CBIGNUM_H
#define _CBIGNUM_H

#ifndef __cplusplus
#error  Must use C++ for the type cBigNumber.
#endif

#ifndef _CBNL_H
#include "Cbnl.h"
#endif

#ifndef _CTTY_H
#include "Ctty.h"
#endif

#ifndef EXARRAY_H
#include "Exarray.h"
#endif

#ifndef EXDEBUG_H
#include "Exdebug.h"
#endif

#if 1
#define _CBIGNUM_MT
#endif

#if 0
#define _CBIGNUM_TOATMP
#endif

#if 0
#define _CBIGNUM_CONSTCAST
#endif

#ifndef _CBIGNUM_MT
#define _CBIGNUM_TOATMP
#define _CBIGNUM_CONSTCAST
#endif

#ifndef EXTHREAD_LOCAL
#ifndef _CBIGNUM_MT
#define EXTHREAD_LOCAL
#else
#include "Exthread.h"
#endif
#endif

#if 0
#define _CBIGNUM_DEF_ALLOC
#endif

#if 0
#define _CBIGNUM_NCHECKPTR
#endif

#ifdef  _CBIGNUM_NCHECKPTR

#define CBPTR(T)        T*
#define CBPTRTYPE(m)    ((m).base())
#define CBPTRBASE(p)    (p)
#define CBPTRINDEX(p,i) (p)

#else

#define CBPTR(T)        EXPTR(T)
#define CBPTRTYPE(m)    EXPTRTYPE(m)
#define CBPTRBASE(p)    EXPTRBASE(p)
#define CBPTRINDEX(p,i) EXPTRINDEX(p,i)

#endif

typedef exarray<char> cBigString;

class cBigNumber: public exblock<CBNL>
{
protected:
#ifndef _CBIGNUM_DEF_ALLOC
  static CBNL _stub [(EXALLOC_EXTRA_ITEMS(CBNL) > 2?
                      EXALLOC_EXTRA_ITEMS(CBNL) : 2)];
#endif

  void  create (CBNL b = 0)
  {
#ifndef _CBIGNUM_DEF_ALLOC
    if (b == 0) { e = _stub; len = 0; } else
#endif
    {
      e = (CBNL *) exmalloc (EXCALCBLOCKSIZE_1);
      len = max_size (EXCALCBLOCKSIZE_1);
      e [0] = 1; e [1] = b;
      checkindex (1);
    }
  }

  cBigNumber (exblockstack_t) {}

public:

#ifndef _CBIGNUM_DEF_ALLOC
  cBigNumber ()                 { e = _stub; len = 0; }
#else
  cBigNumber ()                 { create();   }
#endif
  cBigNumber (CBNL b)           { create (b);   }
  cBigNumber (const cBigNumber&);
  cBigNumber (const char* psz, unsigned radix);

#ifndef _CBIGNUM_DEF_ALLOC
  ~cBigNumber()                 { if (len) exfree (e);  }
#else
  ~cBigNumber()                 { exfree (e);           }
#endif

  size_t      length  () const  { return (size_t)(*e);}
  const CBNL* code    () const  { return e + 1;       }
  const CBNL& loword  () const  { return e [*e != 0]; }
  const CBNL& hiword  () const  { return e [length()];}
  size_t      isfit   () const;
  size_t      words   () const;
  size_t      exwords () const;

  CBNL        bits    () const;
  cBigNumber& setbits   (const cBigNumber&);
  cBigNumber& setbits   (CBNL);
  CBNL        exbits  () const;
  cBigNumber& setexbits (const cBigNumber&);
  cBigNumber& setexbits (CBNL);

  cBigNumber& fit   ();
  cBigNumber& tab   ();
  cBigNumber& smp   ();
  cBigNumber& gc    ();
  cBigNumber& pack  ();
#ifdef  _CBIGNUM_CONSTCAST
const cBigNumber& fit   () const { return ((cBigNumber*)this)->fit();   }
const cBigNumber& tab   () const { return ((cBigNumber*)this)->tab();   }
const cBigNumber& smp   () const { return ((cBigNumber*)this)->smp();   }
const cBigNumber& gc    () const { return ((cBigNumber*)this)->gc();    }
const cBigNumber& pack  () const { return ((cBigNumber*)this)->pack();  }
#endif

  void        clear ();

  CBNL       _testnot0() const  { return (hiword() | (e [0] >> 1)); }

  int         comp  (const cBigNumber&) const;
  int         comp  (CBNL)              const;
  int         comp0 ()                  const;

  cBigNumber& neg   ()  { return setneg  (*this); }
  cBigNumber& abs   ();
  cBigNumber& unsign();
  cBigNumber& operator ++();
  cBigNumber& operator --();
  cBigNumber& mul2  ();
  cBigNumber& div2  ();
  cBigNumber& pow2  ();
  cBigNumber& sqrt  ();

  cBigNumber& set         (const cBigNumber&);
  cBigNumber& set         (CBNL);
  cBigNumber& setneg      (const cBigNumber&);
  cBigNumber& setneg      (CBNL);
  cBigNumber& setabs      (const cBigNumber&);
  cBigNumber& setabs      (CBNL);
  cBigNumber& setunsign   (const cBigNumber&);
  cBigNumber& setunsign   (CBNL);
  cBigNumber& setcompl    (const cBigNumber&);
  cBigNumber& setcompl    (CBNL);
  cBigNumber& setxor (const cBigNumber&, const cBigNumber&);
  cBigNumber& setxor (const cBigNumber&, CBNL);
  cBigNumber& setxor (CBNL, const cBigNumber&);
  cBigNumber& setxor (CBNL, CBNL);
  cBigNumber& setand (const cBigNumber&, const cBigNumber&);
  cBigNumber& setand (const cBigNumber&, CBNL);
  cBigNumber& setand (CBNL, const cBigNumber&);
  cBigNumber& setand (CBNL, CBNL);
  cBigNumber& setor  (const cBigNumber&, const cBigNumber&);
  cBigNumber& setor  (const cBigNumber&, CBNL);
  cBigNumber& setor  (CBNL, const cBigNumber&);
  cBigNumber& setor  (CBNL, CBNL);
  cBigNumber& setadd (const cBigNumber&, const cBigNumber&);
  cBigNumber& setadd (const cBigNumber&, CBNL);
  cBigNumber& setadd (CBNL, const cBigNumber&);
  cBigNumber& setadd (CBNL, CBNL);
  cBigNumber& setsub (const cBigNumber&, const cBigNumber&);
  cBigNumber& setsub (const cBigNumber&, CBNL);
  cBigNumber& setsub (CBNL, const cBigNumber&);
  cBigNumber& setsub (CBNL, CBNL);
  cBigNumber& setmul (const cBigNumber&, const cBigNumber&);
  cBigNumber& setmul (const cBigNumber&, CBNL);
  cBigNumber& setmul (CBNL, const cBigNumber&);
  cBigNumber& setmul (CBNL, CBNL);
  cBigNumber& setdiv (const cBigNumber&, const cBigNumber&);
  cBigNumber& setdiv (const cBigNumber&, CBNL);
  cBigNumber& setdiv (CBNL, const cBigNumber&);
  cBigNumber& setdiv (CBNL, CBNL);
  cBigNumber& setmod (const cBigNumber&, const cBigNumber&);
  cBigNumber& setmod (const cBigNumber&, CBNL);
  cBigNumber& setmod (CBNL, const cBigNumber&);
  cBigNumber& setmod (CBNL, CBNL);
  cBigNumber& setshl (const cBigNumber&, const cBigNumber&);
  cBigNumber& setshl (const cBigNumber&, CBNL);
  cBigNumber& setshl (CBNL, const cBigNumber&);
  cBigNumber& setshl (CBNL, CBNL);
  cBigNumber& setshr (const cBigNumber&, const cBigNumber&);
  cBigNumber& setshr (const cBigNumber&, CBNL);
  cBigNumber& setshr (CBNL, const cBigNumber&);
  cBigNumber& setshr (CBNL, CBNL);
  cBigNumber& setpow (const cBigNumber&, const cBigNumber&);
  cBigNumber& setpow (const cBigNumber&, CBNL);
  cBigNumber& setpow (CBNL, const cBigNumber&);
  cBigNumber& setpow (CBNL, CBNL);
  cBigNumber& setpowmod (const cBigNumber&, const cBigNumber&,
                         const cBigNumber&);
  cBigNumber& setpowmod (const cBigNumber&, const cBigNumber&, CBNL);
  cBigNumber& setpowmod (const cBigNumber&, CBNL, const cBigNumber&);
  cBigNumber& setpowmod (const cBigNumber&, CBNL, CBNL);
  cBigNumber& setpowmod (CBNL, const cBigNumber&, const cBigNumber&);
  cBigNumber& setpowmod (CBNL, const cBigNumber&, CBNL);
  cBigNumber& setpowmod (CBNL, CBNL, const cBigNumber&);
  cBigNumber& setpowmod (CBNL, CBNL, CBNL);
  cBigNumber& setsqrt   (const cBigNumber&);
  cBigNumber& setsqrt   (CBNL);
  cBigNumber& setrandom (unsigned long (*p)(), unsigned CBNL);

  cBigNumber& operator =  (const cBigNumber& b) { return set (b);       }
  cBigNumber& operator =  (CBNL b)              { return set (b);       }

  cBigNumber& operator ^= (const cBigNumber&);
  cBigNumber& operator ^= (CBNL);
  cBigNumber& operator &= (const cBigNumber&);
  cBigNumber& operator &= (CBNL);
  cBigNumber& operator |= (const cBigNumber&);
  cBigNumber& operator |= (CBNL);
  cBigNumber& operator += (const cBigNumber&);
  cBigNumber& operator += (CBNL);
  cBigNumber& operator -= (const cBigNumber&);
  cBigNumber& operator -= (CBNL);
  cBigNumber& operator *= (const cBigNumber&);
  cBigNumber& operator *= (CBNL);
  cBigNumber& operator /= (const cBigNumber&);
  cBigNumber& operator /= (CBNL);
  cBigNumber& operator %= (const cBigNumber&);
  cBigNumber& operator %= (CBNL);
  cBigNumber& operator<<= (const cBigNumber&);
  cBigNumber& operator<<= (CBNL);
  cBigNumber& operator>>= (const cBigNumber&);
  cBigNumber& operator>>= (CBNL);
  cBigNumber& pow         (const cBigNumber&);
  cBigNumber& pow         (CBNL);

  cBigNumber& powmod      (const cBigNumber&, const cBigNumber&);
  cBigNumber& powmod      (const cBigNumber&, CBNL);
  cBigNumber& powmod      (CBNL, const cBigNumber&);
  cBigNumber& powmod      (CBNL, CBNL);

  cBigNumber& set         (const cBigNumber&, size_t k);
  cBigNumber& set         (CBNL, size_t k);
  cBigNumber& setr        (const cBigNumber&, size_t k);

  cBigNumber& add         (const cBigNumber&, size_t k = 0);
  cBigNumber& add         (CBNL, size_t k = 0);
  cBigNumber& sub         (const cBigNumber&, size_t k = 0);
  cBigNumber& sub         (CBNL, size_t k = 0);

  cBigNumber& addmul      (const cBigNumber&, const cBigNumber&);
  cBigNumber& addmul      (const cBigNumber&, CBNL);
  cBigNumber& addmul      (CBNL, const cBigNumber&);
  cBigNumber& addmul      (CBNL, CBNL);
  cBigNumber& submul      (const cBigNumber&, const cBigNumber&);
  cBigNumber& submul      (const cBigNumber&, CBNL);
  cBigNumber& submul      (CBNL, const cBigNumber&);
  cBigNumber& submul      (CBNL, CBNL);

  cBigNumber& setdivmod   (cBigNumber&, const cBigNumber&);
  cBigNumber& setdivmod   (cBigNumber&, CBNL);

  cBigNumber& setsqrtrm   (cBigNumber&);

  cBigNumber& addmultab   (const cBigNumber&, const cBigNumber&, size_t k = 0);
  cBigNumber& addmultab   (const cBigNumber&, CBNL, size_t k = 0);
  cBigNumber& submultab   (const cBigNumber&, const cBigNumber&, size_t k = 0);
  cBigNumber& submultab   (const cBigNumber&, CBNL, size_t k = 0);
  cBigNumber& addmulsmp   (const cBigNumber&, const cBigNumber&, size_t k = 0);
  cBigNumber& addmulsmp   (const cBigNumber&, CBNL, size_t k = 0);
  cBigNumber& submulsmp   (const cBigNumber&, const cBigNumber&, size_t k = 0);
  cBigNumber& submulsmp   (const cBigNumber&, CBNL, size_t k = 0);

  cBigNumber& divtab      (const cBigNumber&, size_t k = 0);
  cBigNumber& divtab      (CBNL, size_t k = 0);
  cBigNumber& modtab      (const cBigNumber&, size_t k = 0);
  cBigNumber& modtab      (CBNL, size_t k = 0);
  cBigNumber& setdivtab   (const cBigNumber&, const cBigNumber&, size_t k = 0);
  cBigNumber& setdivtab   (CBNL, const cBigNumber&, size_t k = 0);
  cBigNumber& setmodtab   (const cBigNumber&, const cBigNumber&, size_t k = 0);
  cBigNumber& setmodtab   (CBNL, const cBigNumber&, size_t k = 0);

  cBigNumber& setdivmodtab(cBigNumber&, const cBigNumber&, size_t k = 0);

static  const char* pszConverting;
static  const char* pszFormatting;

#define MAX_RADIX 16
static  const char  numtochar [2] [MAX_RADIX + 1];
static        char  chartonum [UCHAR_MAX + 1];
static  size_t  SqrTab  (unsigned radix, size_t len = 0, size_t max_np = 0);
  cBigNumber& setunsign (const EXPTR(char) ps,
                         unsigned radix,
                         size_t   width);
  cBigNumber& set       (const char* psz,
                         unsigned radix = 10);
#define cBigNumber_uppercase    0x00040000L
#define cBigNumber_showbase     0x00080000L
#define cBigNumber_showpos      0x00200000L
#define cBigNumber_unsign       0x80000000L
  char*       toa       (cBigString& buf_,
                         unsigned radix = 10,
                         size_t   width = 0,
                         long     fill  = 0)    const;
#ifdef  _CBIGNUM_TOATMP
  char*       toatmp    (unsigned radix = 10,
                         size_t   width = 0,
                         long     fill  = 0)    const;
#endif
  void        dump      ()  const;
  void        info      ()  const;

  void        erange    ()  const;
  CBNL        toCBNL    ()  const;
  long        tolong    ()  const;
  int         toint     ()  const;
  short       toshort   ()  const;

  operator const void * ()  const       { return (comp0()? e: 0);       }
  int        operator ! ()  const       { return (comp0() == 0);        }

static  void  maskdiv0  (int);
static  int   testdiv0  ();

#ifndef _CBIGNUM_MT
static  cBigNumber& lastdivmod ();
static  cBigNumber& lastrootrm ();
#endif
};

extern  size_t      cBigNumberMaskDiv0;
#ifndef _CBIGNUM_MT
extern  cBigNumber  cBigNumberLastDivMod;
extern  cBigNumber  cBigNumberLastRootRm;
#endif

int     b_SPRP      (const cBigNumber& n, const cBigNumber& b);

int     SPRP        (const cBigNumber&);
int     SPRP        (const cBigNumber&, cBigNumber&);
int FastSPRP        (const cBigNumber&);
int FastSPRP        (const cBigNumber&, cBigNumber&);
int LastSPRP        (const cBigNumber&);
int LastSPRP        (const cBigNumber&, cBigNumber&);

int TestFactor      (const cBigNumber&);
int TestFactor      (const cBigNumber&, cBigNumber&);
int TestSmallFactor (const cBigNumber&);
int TestSmallFactor (const cBigNumber&, cBigNumber&);
int TestLargeFactor (const cBigNumber&);
int TestLargeFactor (const cBigNumber&, cBigNumber&);

int IsPrime         (const cBigNumber&);
int IsProvedPrime   (const cBigNumber&);
int IsMillerPrime   (const cBigNumber&);
int IsStrongPrime   (const cBigNumber&);

#ifdef  _CBIGNUM_HASFACTOR
inline int HasFactor(const cBigNumber& n) { return TestFactor(n);   }
#endif

cTTY& operator << (cTTY& os, const cBigNumber& b);
void operator <<= (cTTY& os, const cBigNumber& b);

extern cTTY cBigNumberMessages;
extern cTTY cBigNumberProgress;

class cHexDump
{
  char buffer [CHAR_BIT * sizeof (CBNL) * 3 / 8];
public:
  cHexDump (unsigned CBNL n);
  operator const char*() { return (buffer); }
};

class _cBigLong
{
  CBNL e [2];

public:

  _cBigLong         (CBNL n = 0) { e [0] = 1; e [1] = n;        }

  _cBigLong& operator = (CBNL n) { e [1] = n; return *this;     }

  CBNL*       base()            { return (e);                   }

#ifndef NCHECKPTR

  operator    exptr<CBNL>()     { return exptr<CBNL> (e, 2);    }

#endif
};

#ifdef  NCHECKPTR
#ifdef  __cplusplus
extern "C" {
#endif
#endif

void    cBigNumberDump    (const CBPTR(CBNL) p1);
void    cBigNumberInfo    (const CBPTR(CBNL) p1);
void    cBigNumberERange  (const CBPTR(CBNL) p1);
size_t  cBigNumberWords   (const CBPTR(CBNL) p1);
CBNL    cBigNumberBits    (const CBPTR(CBNL) p1);
size_t  cBigNumberExWords (const CBPTR(CBNL) p1);
CBNL    cBigNumberExBits  (const CBPTR(CBNL) p1);
size_t _CBNL_C  cBigNumberIsFit
                          (const CBPTR(CBNL) p1);
size_t _CBNL_C _cBigNumberFit   (EXPTR(CBNL) p1);
size_t _CBNL_C _cBigNumberFitTo (EXPTR(CBNL) p1, size_t n);
int    _CBNL_C  cBigNumberComp
                          (const CBPTR(CBNL) p1, const CBPTR(CBNL) p2);
int    _CBNL_C  cBigNumberCompHigh
                          (const CBPTR(CBNL) p1, const CBPTR(CBNL) p2);
void   _CBNL_C  cBigNumberCopy
                          (const CBPTR(CBNL) p1, EXPTR(CBNL) p);
void    cBigNumberCompl   (const CBPTR(CBNL) p1, EXPTR(CBNL) p);
void    cBigNumberCopyShl (const CBPTR(CBNL) p1, size_t k1, EXPTR(CBNL) p);
void    cBigNumberCopyShr (const CBPTR(CBNL) p1, size_t k1, EXPTR(CBNL) p);
void   _CBNL_C  cBigNumberCopyShrToM  (const CBPTR(CBNL) p1, size_t k1, EXPTR(CBNL) p);
void   _CBNL_C  cBigNumberCopyShrUToM (const CBPTR(CBNL) p1, size_t k1, EXPTR(CBNL) p);
void   _CBNL_C  cBigNumberClearTo (CBPTR(CBNL) p1, size_t n);
void    cBigNumberCutOut  (const CBPTR(CBNL) p1, size_t k1, size_t n,
                                 EXPTR(CBNL) p);
size_t _CBNL_C _cBigNumberNeg
                          (const CBPTR(CBNL) p1, EXPTR(CBNL) p);
void    cBigNumberMNegF         (EXPTR(CBNL) p1);
size_t _cBigNumberMInc          (EXPTR(CBNL) p1);
size_t _cBigNumberMDec          (EXPTR(CBNL) p1);
size_t _cBigNumberMMul2         (EXPTR(CBNL) p1);
size_t _cBigNumberMDiv2         (EXPTR(CBNL) p1);
size_t _cBigNumberMul2    (const CBPTR(CBNL) p1, EXPTR(CBNL) p);
size_t _cBigNumberDiv2    (const CBPTR(CBNL) p1, EXPTR(CBNL) p);
void    cBigNumberTab           (EXPTR(CBNL) p1);
size_t _cBigNumberXor     (const CBPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 EXPTR(CBNL) p);
size_t _cBigNumberAnd     (const CBPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 EXPTR(CBNL) p);
size_t _cBigNumberOr      (const CBPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 EXPTR(CBNL) p);
size_t _cBigNumberAdd     (const CBPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 EXPTR(CBNL) p);
size_t _cBigNumberSub     (const CBPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 EXPTR(CBNL) p);
size_t _cBigNumberSubS    (const CBPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 EXPTR(CBNL) p);
size_t _cBigNumberMAdd          (EXPTR(CBNL) p1, const CBPTR(CBNL) p2);
size_t _cBigNumberMSub          (EXPTR(CBNL) p1, const CBPTR(CBNL) p2);
size_t _CBNL_C  cBigNumberSkipLow0
                          (const CBPTR(CBNL) p1);
size_t _CBNL_C  cBigNumberCopySkipLow0
                          (const CBPTR(CBNL) p1, EXPTR(CBNL) p);
void   _CBNL_C  cBigNumberMAddM (EXPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 size_t k2);
void   _CBNL_C  cBigNumberMSubM (EXPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 size_t k2);
size_t _CBNL_C _cBigNumberMSubD (EXPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 size_t k2);
size_t _CBNL_C _cBigNumberMMul2M(EXPTR(CBNL) p1);
size_t _CBNL_C _cBigNumberMDiv2D(EXPTR(CBNL) p1);
void _CBNL_C cBigNumberMAddMulM (EXPTR(CBNL) p1, const EXPTR(CBNL) p2,
                                 unsigned CBNL l2, size_t k2);
void _CBNL_C cBigNumberMSubMulM (EXPTR(CBNL) p1, const EXPTR(CBNL) p2,
                                 unsigned CBNL l2, size_t k2);
void    cBigNumberMShrM         (EXPTR(CBNL) p1);
void    cBigNumberMShlD         (EXPTR(CBNL) p1);
void    cBigNumberMAddShl       (EXPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 size_t k2);
void    cBigNumberMSubShl       (EXPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 size_t k2);
void    cBigNumberMAddMulShl    (EXPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 size_t k, EXPTR(CBNL) p);
void    cBigNumberMSubMulShl    (EXPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 size_t k, EXPTR(CBNL) p);
void    cBigNumberMAddMulShlTab
                          (const CBPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 size_t k, EXPTR(CBNL) p);
void    cBigNumberMSubMulShlTab
                          (const CBPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 size_t k, EXPTR(CBNL) p);
void    cBigNumberMAddMulShlKar (EXPTR(CBNL) p1, EXPTR(CBNL) p2,
                                 size_t k, EXPTR(CBNL) p);
void    cBigNumberMSubMulShlKar (EXPTR(CBNL) p1, EXPTR(CBNL) p2,
                                 size_t k, EXPTR(CBNL) p);
void    cBigNumberMAddMul (const CBPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 EXPTR(CBNL) p);
void    cBigNumberMSubMul (const CBPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 EXPTR(CBNL) p);
void    cBigNumberMul     (const CBPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 EXPTR(CBNL) p);
void    cBigNumberDiv0();
void    cBigNumberMModDivShlTab (EXPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 size_t k2, EXPTR(CBNL) p);
void    cBigNumberMModDiv       (EXPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 EXPTR(CBNL) p);
void    cBigNumberMModShlTab    (EXPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 size_t k2);
void    cBigNumberMMod          (EXPTR(CBNL) p1, const CBPTR(CBNL) p2);
void    cBigNumberPow           (EXPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                 EXPTR(CBNL) p);
void    cBigNumberPowMod        (EXPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                                 const CBPTR(CBNL) mod,
                                 EXPTR(CBNL) p);
void    cBigNumberMRmSqrt       (EXPTR(CBNL) p1, EXPTR(CBNL) p);
void    cBigNumberRandom  (unsigned long (*pfnRand)(), unsigned CBNL lBits,
                                 EXPTR(CBNL) p);

#ifdef  NCHECKPTR
#ifdef  __cplusplus
}
#endif
#endif

inline  size_t  cBigNumberNeg     (const CBPTR(CBNL) p1, EXPTR(CBNL) p)
{
  return (size_t)(*(p)=(CBNL)_cBigNumberNeg (p1, p));
}

inline  size_t  cBigNumberMInc          (EXPTR(CBNL) p1)
{
  return (size_t)(*(p1)=(CBNL)_cBigNumberMInc (p1));
}

inline  size_t  cBigNumberMDec          (EXPTR(CBNL) p1)
{
  return (size_t)(*(p1)=(CBNL)_cBigNumberMDec (p1));
}

inline  size_t  cBigNumberMMul2         (EXPTR(CBNL) p1)
{
  return (size_t)(*(p1)=(CBNL)_cBigNumberMMul2 (p1));
}

inline  size_t  cBigNumberMDiv2         (EXPTR(CBNL) p1)
{
  return (size_t)(*(p1)=(CBNL)_cBigNumberMDiv2 (p1));
}

inline  size_t  cBigNumberMul2   (const CBPTR(CBNL) p1, EXPTR(CBNL) p)
{
  return (size_t)(*(p)=(CBNL)_cBigNumberMul2 (p1, p));
}

inline  size_t  cBigNumberDiv2   (const CBPTR(CBNL) p1, EXPTR(CBNL) p)
{
  return (size_t)(*(p)=(CBNL)_cBigNumberDiv2 (p1, p));
}

inline  size_t  cBigNumberFit          (EXPTR(CBNL) p1)
{
  return (size_t)(*(p1)=(CBNL)_cBigNumberFit (p1));
}

inline  size_t  cBigNumberFitTo        (EXPTR(CBNL) p1, size_t n)
{
  return (size_t)(*(p1)=(CBNL)_cBigNumberFitTo (p1, n));
}

inline  size_t  cBigNumberXor    (const CBPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                        EXPTR(CBNL) p)
{
  return (size_t)(*(p)=(CBNL)_cBigNumberXor (p1, p2, p));
}

inline  size_t  cBigNumberAnd    (const CBPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                        EXPTR(CBNL) p)
{
  return (size_t)(*(p)=(CBNL)_cBigNumberAnd (p1, p2, p));
}

inline  size_t  cBigNumberOr     (const CBPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                        EXPTR(CBNL) p)
{
  return (size_t)(*(p)=(CBNL)_cBigNumberOr  (p1, p2, p));
}

inline  size_t  cBigNumberAdd    (const CBPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                        EXPTR(CBNL) p)
{
  return (size_t)(*(p)=(CBNL)_cBigNumberAdd (p1, p2, p));
}

inline  size_t  cBigNumberSub    (const CBPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                        EXPTR(CBNL) p)
{
  return (size_t)(*(p)=(CBNL)_cBigNumberSub (p1, p2, p));
}

inline  size_t  cBigNumberSubS   (const CBPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                        EXPTR(CBNL) p)
{
  return (size_t)(*(p)=(CBNL)_cBigNumberSubS (p1, p2, p));
}

inline  size_t  cBigNumberMAdd         (EXPTR(CBNL) p1, const CBPTR(CBNL) p2)
{
  return (size_t)(*(p1)=(CBNL)_cBigNumberMAdd (p1, p2));
}

inline  size_t  cBigNumberMSub         (EXPTR(CBNL) p1, const CBPTR(CBNL) p2)
{
  return (size_t)(*(p1)=(CBNL)_cBigNumberMSub (p1, p2));
}

inline  size_t  cBigNumberMSubD        (EXPTR(CBNL) p1, const CBPTR(CBNL) p2,
                                        size_t k2)
{
  return (size_t)(*(p1)=(CBNL)_cBigNumberMSubD (p1, p2, k2));
}

inline  size_t  cBigNumberMMul2M        (EXPTR(CBNL) p1)
{
  return (size_t)(*(p1)=(CBNL)_cBigNumberMMul2M (p1));
}

inline  size_t  cBigNumberMDiv2D        (EXPTR(CBNL) p1)
{
  return (size_t)(*(p1)=(CBNL)_cBigNumberMDiv2D (p1));
}

inline cBigNumber::cBigNumber (const cBigNumber& b)
{
  len = 0;
#ifndef _CBIGNUM_DEF_ALLOC
  if (b._testnot0() != 0)
#endif
  {
    checkexpand (b.length());
    cBigNumberCopy (CBPTRTYPE(b), EXPTRTYPE(*this));
    checkindex (length());
  }
#ifndef _CBIGNUM_DEF_ALLOC
  else e = _stub;
#endif
}

inline cBigNumber::cBigNumber (const char* psz, unsigned radix)
{
#ifndef _CBIGNUM_DEF_ALLOC
  e = _stub; len = 0;
#else
  create();
#endif
  set (psz, radix);
}

inline void   cBigNumber::dump()    const
{
  cBigNumberDump (CBPTRTYPE(*this));
}

inline void   cBigNumber::info()    const
{
  cBigNumberInfo (CBPTRTYPE(*this));
}

inline void   cBigNumber::erange()  const
{
  cBigNumberERange (CBPTRTYPE(*this));
}

inline size_t cBigNumber::isfit()   const
{
  return cBigNumberIsFit (CBPTRTYPE(*this));
}

inline size_t cBigNumber::words()   const
{
  return cBigNumberWords (CBPTRTYPE(*this));
}

inline size_t cBigNumber::exwords() const
{
  return cBigNumberExWords (CBPTRTYPE(*this));
}

inline CBNL   cBigNumber::bits()    const
{
  return cBigNumberBits (CBPTRTYPE(*this));
}

inline CBNL   cBigNumber::exbits()  const
{
  return cBigNumberExBits (CBPTRTYPE(*this));
}

inline int  cBigNumber::comp (const cBigNumber& b)  const
{
  return cBigNumberComp (CBPTRTYPE(*this), CBPTRTYPE(b));
}

inline int  cBigNumber::comp (CBNL b)               const
{
  return cBigNumberComp (CBPTRTYPE(*this), CBPTRTYPE(_cBigLong(b)));
}

inline int  cBigNumber::comp0 ()                    const
{
  return cBigNumberComp (CBPTRTYPE(*this), CBPTRTYPE(_cBigLong(0)));
}

inline cBigNumber& cBigNumber::abs()
{
  if (hiword() < 0L) neg();
  return *this;
}

inline cBigNumber& cBigNumber::unsign()
{
  if (hiword() < 0L)
  {
    size_t l = length() + 1;
    checkexpand (l); e [0] = (CBNL)l; e [l] = 0;
  }
  return *this;
}

inline cBigNumber& cBigNumber::set (const cBigNumber& b)
{
#ifndef _CBIGNUM_DEF_ALLOC
  if ((e [0] | b._testnot0()) != 0)
#endif
  {
    checkexpand (b.length());
    cBigNumberCopy (CBPTRTYPE(b), EXPTRTYPE(*this));
    checkindex (length());
  }
  return *this;
}

inline cBigNumber& cBigNumber::set (CBNL b)
{
#ifndef _CBIGNUM_DEF_ALLOC
  if ((e [0] | b) != 0)
#endif
  {
    checkexpand (1); e [0] = 1; e [1] = b;
    checkindex (1);
  }
  return *this;
}

inline cBigNumber& cBigNumber::setneg (const cBigNumber& b)
{
  checkexpand (b.length() + 1);
  cBigNumberNeg (CBPTRTYPE(b), EXPTRTYPE(*this));
  checkindex (length());
  return *this;
}

inline cBigNumber& cBigNumber::setneg (CBNL b)
{
  checkexpand (2); e [0] = 1 + (b == LONG_MIN); e [1] = -b; e [2] = 0;
  checkindex (length());
  return *this;
}

inline cBigNumber& cBigNumber::setabs (const cBigNumber& b)
{
  return (b.hiword()>=0? set (b): setneg (b));
}

inline cBigNumber& cBigNumber::setabs (CBNL b)
{
  if (b < 0) b = -b;
  checkexpand (2); e [0] = 1 + (b == LONG_MIN); e [1] = b; e [2] = 0;
  checkindex (length());
  return *this;
}

inline cBigNumber& cBigNumber::setunsign (const cBigNumber& b)
{
  set (b); return unsign();
}

inline cBigNumber& cBigNumber::setunsign (CBNL b)
{
  checkexpand (2); e [0] = 1 + (b < 0); e [1] = b; e [2] = 0;
  checkindex (length());
  return *this;
}

inline cBigNumber& cBigNumber::setcompl (const cBigNumber& b)
{
  checkexpand (b.length() + (b.length() == 0));
  cBigNumberCompl (CBPTRTYPE(b), EXPTRTYPE(*this));
  checkindex (length());
  return *this;
}

inline cBigNumber& cBigNumber::setcompl (CBNL b)
{
  checkexpand (1); e [0] = 1; e [1] = ~b;
  checkindex (length());
  return *this;
}

inline cBigNumber& cBigNumber::operator ^= (const cBigNumber& b)
{
  return setxor (*this, b);
}

inline cBigNumber& cBigNumber::operator ^= (CBNL b)
{
  return setxor (*this, b);
}

inline cBigNumber& cBigNumber::operator &= (const cBigNumber& b)
{
  return setand (*this, b);
}

inline cBigNumber& cBigNumber::operator &= (CBNL b)
{
  return setand (*this, b);
}

inline cBigNumber& cBigNumber::operator |= (const cBigNumber& b)
{
  return setor (*this, b);
}

inline cBigNumber& cBigNumber::operator |= (CBNL b)
{
  return setor (*this, b);
}

inline cBigNumber& cBigNumber::operator += (const cBigNumber& b)
{
  return add (b);
}

inline cBigNumber& cBigNumber::operator += (CBNL b)
{
  return add (b);
}

inline cBigNumber& cBigNumber::operator -= (const cBigNumber& b)
{
  return sub (b);
}

inline cBigNumber& cBigNumber::operator -= (CBNL b)
{
  return sub (b);
}

inline cBigNumber& cBigNumber::operator *= (const cBigNumber& b)
{
  return setmul (*this, b);
}

inline cBigNumber& cBigNumber::operator *= (CBNL b)
{
  return setmul (*this, b);
}

inline cBigNumber& cBigNumber::operator /= (const cBigNumber& b)
{
#ifdef  _CBIGNUM_MT
  cBigNumber cBigNumberLastDivMod;
#endif
  swap (cBigNumberLastDivMod);
  return setdivmod (cBigNumberLastDivMod, b);
}

inline cBigNumber& cBigNumber::operator /= (CBNL b)
{
#ifdef  _CBIGNUM_MT
  cBigNumber cBigNumberLastDivMod;
#endif
  swap (cBigNumberLastDivMod);
  return setdivmod (cBigNumberLastDivMod, b);
}

inline cBigNumber& cBigNumber::operator %= (const cBigNumber& b)
{
  fit();
  cBigNumberMMod (EXPTRTYPE(*this), CBPTRTYPE(b));
  checkindex (length());
  return *this;
}

inline cBigNumber& cBigNumber::operator %= (CBNL b)
{
  fit();
  cBigNumberMMod (EXPTRTYPE(*this), CBPTRTYPE(_cBigLong(b)));
  checkindex (length());
  return *this;
}

inline cBigNumber& cBigNumber::set (const cBigNumber& b, size_t k)
{
  checkexpand (b.length() + (b.length() == 0) + k);
  cBigNumberCopyShl (CBPTRTYPE(b), k, EXPTRTYPE(*this));
  checkindex (length());
  return *this;
}

inline cBigNumber& cBigNumber::set (CBNL b, size_t k)
{
  checkexpand (k + 1);
  cBigNumberCopyShl (CBPTRTYPE(_cBigLong(b)), k, EXPTRTYPE(*this));
  checkindex (length());
  return *this;
}

inline cBigNumber& cBigNumber::setr (const cBigNumber& b, size_t k)
{
  checkexpand (b.length() > k? b.length() - k: 1);
  cBigNumberCopyShr (CBPTRTYPE(b), k, EXPTRTYPE(*this));
  checkindex (length());
  return *this;
}

inline cBigNumber& cBigNumber::setdivmod (cBigNumber& a, const cBigNumber& b)
{
  checkexpand (a.fit().length() + 2);
  cBigNumberMModDiv (EXPTRTYPE(a), CBPTRTYPE(b), EXPTRTYPE(*this));
  a.checkindex (a.length());
  checkindex (length());
  return *this;
}

inline cBigNumber& cBigNumber::setdivmod (cBigNumber& a, CBNL b)
{
  checkexpand (a.fit().length() + 2);
  cBigNumberMModDiv (EXPTRTYPE(a), CBPTRTYPE(_cBigLong(b)), EXPTRTYPE(*this));
  a.checkindex (a.length());
  checkindex (length());
  return *this;
}

inline cBigNumber& cBigNumber::divtab (const cBigNumber& b, size_t k)
{
#ifdef  _CBIGNUM_MT
  cBigNumber cBigNumberLastDivMod;
#endif
  swap (cBigNumberLastDivMod);
  return setdivmodtab (cBigNumberLastDivMod, b, k);
}

inline cBigNumber& cBigNumber::divtab (CBNL b, size_t k)
{
#ifdef  _CBIGNUM_MT
  cBigNumber cBigNumberLastDivMod;
#endif
  swap (cBigNumberLastDivMod);
  return setdivmodtab (cBigNumberLastDivMod, b, k);
}

inline cBigNumber& cBigNumber::modtab (const cBigNumber& b, size_t k)
{
  fit();
  cBigNumberMModShlTab (EXPTRTYPE(*this), CBPTRTYPE(b), k);
  checkindex (length());
  return *this;
}

inline cBigNumber& cBigNumber::modtab (CBNL b, size_t k)
{
  fit();
  cBigNumberMModShlTab (EXPTRTYPE(*this), CBPTRTYPE(_cBigLong(b)), k);
  checkindex (length());
  return *this;
}

inline cBigNumber& cBigNumber::setdivmodtab (cBigNumber& a,
                                       const cBigNumber& b, size_t k)
{
  checkexpand (a.fit().length() + 2);
  cBigNumberMModDivShlTab (EXPTRTYPE(a), CBPTRTYPE(b), k, EXPTRTYPE(*this));
  a.checkindex (a.length());
  checkindex (length());
  return *this;
}

inline  CBNL    cBigNumber::toCBNL()  const
{
  if (bits() >= CHAR_BIT * sizeof (CBNL)) erange();
  return (loword());
}

inline  long    cBigNumber::tolong()  const
{
  CBNL n = toCBNL();
  if ((long) n != n)  erange();
  return ((long)n);
}

inline  int     cBigNumber::toint()   const
{
  CBNL n = toCBNL();
  if ((int) n != n)   erange();
  return ((int)n);
}

inline  short   cBigNumber::toshort() const
{
  CBNL n = toCBNL();
  if ((short) n != n) erange();
  return ((short)n);
}

inline void cBigNumber::maskdiv0 (int mask) { cBigNumberMaskDiv0 = mask << 1; }
inline int  cBigNumber::testdiv0()          { return cBigNumberMaskDiv0 == 1; }

#ifndef _CBIGNUM_MT
inline cBigNumber& cBigNumber::lastdivmod() { return cBigNumberLastDivMod; }
inline cBigNumber& cBigNumber::lastrootrm() { return cBigNumberLastRootRm; }
#endif

inline  int  operator == (const cBigNumber& a, const cBigNumber& b)
{
  return (a.comp(b) == 0);
}

inline  int  operator == (const cBigNumber& a, CBNL b)
{
  return (a.comp(b) == 0);
}

inline  int  operator == (CBNL b, const cBigNumber& a)
{
  return (a.comp(b) == 0);
}

inline  int  operator != (const cBigNumber& a, const cBigNumber& b)
{
  return (a.comp(b) != 0);
}

inline  int  operator != (const cBigNumber& a, CBNL b)
{
  return (a.comp(b) != 0);
}

inline  int  operator != (CBNL b, const cBigNumber& a)
{
  return (a.comp(b) != 0);
}

inline  int  operator >= (const cBigNumber& a, const cBigNumber& b)
{
  return (a.comp(b) >= 0);
}

inline  int  operator >= (const cBigNumber& a, CBNL b)
{
  return (a.comp(b) >= 0);
}

inline  int  operator >= (CBNL b, const cBigNumber& a)
{
  return (a.comp(b) <= 0);
}

inline  int  operator <= (const cBigNumber& a, const cBigNumber& b)
{
  return (a.comp(b) <= 0);
}

inline  int  operator <= (const cBigNumber& a, CBNL b)
{
  return (a.comp(b) <= 0);
}

inline  int  operator <= (CBNL b, const cBigNumber& a)
{
  return (a.comp(b) >= 0);
}

inline  int  operator >  (const cBigNumber& a, const cBigNumber& b)
{
  return (a.comp(b) > 0);
}

inline  int  operator >  (const cBigNumber& a, CBNL b)
{
  return (a.comp(b) > 0);
}

inline  int  operator >  (CBNL b, const cBigNumber& a)
{
  return (a.comp(b) < 0);
}

inline  int  operator <  (const cBigNumber& a, const cBigNumber& b)
{
  return (a.comp(b) < 0);
}

inline  int  operator <  (const cBigNumber& a, CBNL b)
{
  return (a.comp(b) < 0);
}

inline  int  operator <  (CBNL b, const cBigNumber& a)
{
  return (a.comp(b) > 0);
}

#ifdef  EXTHREAD_LOCAL
extern exblockstack_t EXTHREAD_LOCAL cBigTemp_stack;
extern exblockstack<long>            cBigTemp_sfict;
#else
extern exblockstack_t                cBigTemp_stack;
#endif

class cBigTemp: public cBigNumber
{
public:

#ifdef  EXTHREAD_LOCAL

  cBigTemp (): cBigNumber (cBigTemp_stack)
              { (*(exblockstack<CBNL>*)&cBigTemp_stack).get (*this); }
  ~cBigTemp() { (*(exblockstack<CBNL>*)&cBigTemp_stack).put (*this); }
#else

  cBigTemp (): cBigNumber (cBigTemp_stack)
               { e = NULL; len = 0; }
  ~cBigTemp()  {}

#endif

private:

  cBigNumber& operator ++ ();
  cBigNumber& operator -- ();
  cBigNumber& operator =  (const cBigNumber&);
  cBigNumber& operator =  (CBNL);
  cBigNumber& operator ^= (const cBigNumber&);
  cBigNumber& operator ^= (CBNL);
  cBigNumber& operator &= (const cBigNumber&);
  cBigNumber& operator &= (CBNL);
  cBigNumber& operator |= (const cBigNumber&);
  cBigNumber& operator |= (CBNL);
  cBigNumber& operator += (const cBigNumber&);
  cBigNumber& operator += (CBNL);
  cBigNumber& operator -= (const cBigNumber&);
  cBigNumber& operator -= (CBNL);
  cBigNumber& operator *= (const cBigNumber&);
  cBigNumber& operator *= (CBNL);
  cBigNumber& operator /= (const cBigNumber&);
  cBigNumber& operator /= (CBNL);
  cBigNumber& operator %= (const cBigNumber&);
  cBigNumber& operator %= (CBNL);
  cBigNumber& operator<<= (const cBigNumber&);
  cBigNumber& operator<<= (CBNL);
  cBigNumber& operator>>= (const cBigNumber&);
  cBigNumber& operator>>= (CBNL);
};

struct cBigInc: public cBigTemp
{
  cBigInc   (cBigNumber& a)                             { set (a); ++a; }
};

struct cBigDec: public cBigTemp
{
  cBigDec   (cBigNumber& a)                             { set (a); --a; }
};

struct cBigPos: public cBigTemp
{
  cBigPos   (const cBigNumber& a)                       { set (a);      }
  cBigPos   (CBNL a)                                    { set (a);      }
};

struct cBigNeg: public cBigTemp
{
  cBigNeg   (const cBigNumber& a)                       { setneg (a);   }
  cBigNeg   (CBNL a)                                    { setneg (a);   }
};

struct cBigAbs: public cBigTemp
{
  cBigAbs   (const cBigNumber& a)                       { setabs (a);   }
  cBigAbs   (CBNL a)                                    { setabs (a);   }
};

struct cBigUnsign: public cBigTemp
{
  cBigUnsign(const cBigNumber& a)                       { setunsign (a);}
  cBigUnsign(CBNL a)                                    { setunsign (a);}
};

struct cBigCompl: public cBigTemp
{
  cBigCompl (const cBigNumber& a)                       { setcompl (a); }
  cBigCompl (CBNL a)                                    { setcompl (a); }
};

struct cBigXor: public cBigTemp
{
  cBigXor (const cBigNumber& a, const cBigNumber& b)    { setxor (a, b); }
  cBigXor (const cBigNumber& a, CBNL b)                 { setxor (a, b); }
  cBigXor (CBNL a, const cBigNumber& b)                 { setxor (a, b); }
  cBigXor (CBNL a, CBNL b)                              { setxor (a, b); }
};

struct cBigAnd: public cBigTemp
{
  cBigAnd (const cBigNumber& a, const cBigNumber& b)    { setand (a, b); }
  cBigAnd (const cBigNumber& a, CBNL b)                 { setand (a, b); }
  cBigAnd (CBNL a, const cBigNumber& b)                 { setand (a, b); }
  cBigAnd (CBNL a, CBNL b)                              { setand (a, b); }
};

struct cBigOr: public cBigTemp
{
  cBigOr  (const cBigNumber& a, const cBigNumber& b)    { setor  (a, b); }
  cBigOr  (const cBigNumber& a, CBNL b)                 { setor  (a, b); }
  cBigOr  (CBNL a, const cBigNumber& b)                 { setor  (a, b); }
  cBigOr  (CBNL a, CBNL b)                              { setor  (a, b); }
};

struct cBigAdd: public cBigTemp
{
  cBigAdd (const cBigNumber& a, const cBigNumber& b)    { setadd (a, b); }
  cBigAdd (const cBigNumber& a, CBNL b)                 { setadd (a, b); }
  cBigAdd (CBNL a, const cBigNumber& b)                 { setadd (a, b); }
  cBigAdd (CBNL a, CBNL b)                              { setadd (a, b); }
};

struct cBigSub: public cBigTemp
{
  cBigSub (const cBigNumber& a, const cBigNumber& b)    { setsub (a, b); }
  cBigSub (const cBigNumber& a, CBNL b)                 { setsub (a, b); }
  cBigSub (CBNL a, const cBigNumber& b)                 { setsub (a, b); }
  cBigSub (CBNL a, CBNL b)                              { setsub (a, b); }
};

struct cBigMul: public cBigTemp
{
  cBigMul (const cBigNumber& a, const cBigNumber& b)    { setmul (a, b); }
  cBigMul (const cBigNumber& a, CBNL b)                 { setmul (a, b); }
  cBigMul (CBNL a, const cBigNumber& b)                 { setmul (a, b); }
  cBigMul (CBNL a, CBNL b)                              { setmul (a, b); }
};

struct cBigDiv: public cBigTemp
{
  cBigDiv (const cBigNumber& a, const cBigNumber& b)    { setdiv (a, b); }
  cBigDiv (const cBigNumber& a, CBNL b)                 { setdiv (a, b); }
  cBigDiv (CBNL a, const cBigNumber& b)                 { setdiv (a, b); }
  cBigDiv (CBNL a, CBNL b)                              { setdiv (a, b); }
};

struct cBigDivMod: public cBigTemp
{
  cBigDivMod (cBigNumber& a, const cBigNumber& b)       { setdivmod (a, b); }
  cBigDivMod (cBigNumber& a, CBNL b)                    { setdivmod (a, b); }
};

struct cBigMod: public cBigTemp
{
  cBigMod (const cBigNumber& a, const cBigNumber& b)    { setmod (a, b); }
  cBigMod (const cBigNumber& a, CBNL b)                 { setmod (a, b); }
  cBigMod (CBNL a, const cBigNumber& b)                 { setmod (a, b); }
  cBigMod (CBNL a, CBNL b)                              { setmod (a, b); }
};

struct cBigShl: public cBigTemp
{
  cBigShl (const cBigNumber& a, const cBigNumber& b)    { setshl (a, b); }
  cBigShl (const cBigNumber& a, CBNL b)                 { setshl (a, b); }
  cBigShl (CBNL a, const cBigNumber& b)                 { setshl (a, b); }
  cBigShl (CBNL a, CBNL b)                              { setshl (a, b); }
};

struct cBigShr: public cBigTemp
{
  cBigShr (const cBigNumber& a, const cBigNumber& b)    { setshr (a, b); }
  cBigShr (const cBigNumber& a, CBNL b)                 { setshr (a, b); }
  cBigShr (CBNL a, const cBigNumber& b)                 { setshr (a, b); }
  cBigShr (CBNL a, CBNL b)                              { setshr (a, b); }
};

struct cBigPow: public cBigTemp
{
  cBigPow (const cBigNumber& a, const cBigNumber& b)    { setpow (a, b); }
  cBigPow (const cBigNumber& a, CBNL b)                 { setpow (a, b); }
  cBigPow (CBNL a, const cBigNumber& b)                 { setpow (a, b); }
  cBigPow (CBNL a, CBNL b)                              { setpow (a, b); }
};

struct cBigPowMod: public cBigTemp
{
  cBigPowMod (const cBigNumber& a, const cBigNumber& b, cBigNumber& mod)
                                                { setpowmod (a, b, mod); }
  cBigPowMod (const cBigNumber& a, const cBigNumber& b, CBNL mod)
                                                { setpowmod (a, b, mod); }
  cBigPowMod (const cBigNumber& a, CBNL b, cBigNumber& mod)
                                                { setpowmod (a, b, mod); }
  cBigPowMod (const cBigNumber& a, CBNL b, CBNL mod)
                                                { setpowmod (a, b, mod); }
  cBigPowMod (CBNL a, const cBigNumber& b, cBigNumber& mod)
                                                { setpowmod (a, b, mod); }
  cBigPowMod (CBNL a, const cBigNumber& b, CBNL mod)
                                                { setpowmod (a, b, mod); }
  cBigPowMod (CBNL a, CBNL b, cBigNumber& mod)
                                                { setpowmod (a, b, mod); }
  cBigPowMod (CBNL a, CBNL b, CBNL mod)
                                                { setpowmod (a, b, mod); }
};

struct cBigSqrt: public cBigTemp
{
  cBigSqrt (const cBigNumber& a)                        { setsqrt (a);   }
  cBigSqrt (CBNL a)                                     { setsqrt (a);   }
};

struct cBigSqrtRm: public cBigTemp
{
  cBigSqrtRm (cBigNumber& a)                            { setsqrtrm (a); }
};

struct cBigBits: public cBigTemp
{
  cBigBits (cBigNumber& a)                              { setbits (a);   }
};

struct cBigExBits: public cBigTemp
{
  cBigExBits (cBigNumber& a)                            { setexbits (a); }
};

struct cBigRandom: public cBigTemp
{
  cBigRandom (unsigned long (*p)(), unsigned CBNL n)    { setrandom (p, n); }
};

inline cBigInc   operator ++ (cBigNumber &a, int)
{
  return cBigInc (a);
}

inline cBigDec   operator -- (cBigNumber &a, int)
{
  return cBigDec (a);
}

inline cBigPos   operator + (const cBigNumber &a)
{
  return cBigPos (a);
}

inline cBigNeg   operator - (const cBigNumber &a)
{
  return cBigNeg (a);
}

inline cBigCompl operator ~ (const cBigNumber &a)
{
  return cBigCompl (a);
}

inline cBigXor operator ^ (const cBigNumber& a, const cBigNumber& b)
{
  return cBigXor (a, b);
}

inline cBigXor operator ^ (const cBigNumber& a, CBNL b)
{
  return cBigXor (a, b);
}

inline cBigXor operator ^ (CBNL b, const cBigNumber& a)
{
  return cBigXor (a, b);
}

inline cBigAnd operator & (const cBigNumber& a, const cBigNumber& b)
{
  return cBigAnd (a, b);
}

inline cBigAnd operator & (const cBigNumber& a, CBNL b)
{
  return cBigAnd (a, b);
}

inline cBigAnd operator & (CBNL b, const cBigNumber& a)
{
  return cBigAnd (a, b);
}

inline cBigOr  operator | (const cBigNumber& a, const cBigNumber& b)
{
  return cBigOr  (a, b);
}

inline cBigOr  operator | (const cBigNumber& a, CBNL b)
{
  return cBigOr  (a, b);
}

inline cBigOr  operator | (CBNL b, const cBigNumber& a)
{
  return cBigOr  (a, b);
}

inline cBigAdd operator + (const cBigNumber& a, const cBigNumber& b)
{
  return cBigAdd (a, b);
}

inline cBigAdd operator + (const cBigNumber& a, CBNL b)
{
  return cBigAdd (a, b);
}

inline cBigAdd operator + (CBNL b, const cBigNumber& a)
{
  return cBigAdd (a, b);
}

inline cBigSub operator - (const cBigNumber& a, const cBigNumber& b)
{
  return cBigSub (a, b);
}

inline cBigSub operator - (const cBigNumber& a, CBNL b)
{
  return cBigSub (a, b);
}

inline cBigSub operator - (CBNL a, const cBigNumber& b)
{
  return cBigSub (a, b);
}

inline cBigMul operator * (const cBigNumber& a, const cBigNumber& b)
{
  return cBigMul (a, b);
}

inline cBigMul operator * (const cBigNumber& a, CBNL b)
{
  return cBigMul (a, b);
}

inline cBigMul operator * (CBNL b, const cBigNumber& a)
{
  return cBigMul (a, b);
}

inline cBigDiv operator / (const cBigNumber& a, const cBigNumber& b)
{
  return cBigDiv (a, b);
}

inline cBigDiv operator / (const cBigNumber& a, CBNL b)
{
  return cBigDiv (a, b);
}

inline cBigDiv operator / (CBNL a, const cBigNumber& b)
{
  return cBigDiv (a, b);
}

inline cBigMod operator % (const cBigNumber& a, const cBigNumber& b)
{
  return cBigMod (a, b);
}

inline cBigMod operator % (const cBigNumber& a, CBNL b)
{
  return cBigMod (a, b);
}

inline cBigMod operator % (CBNL a, const cBigNumber& b)
{
  return cBigMod (a, b);
}

inline cBigShl operator << (const cBigNumber& a, const cBigNumber& b)
{
  return cBigShl (a, b);
}

inline cBigShl operator << (const cBigNumber& a, CBNL b)
{
  return cBigShl (a, b);
}

inline cBigShl operator << (CBNL a, const cBigNumber& b)
{
  return cBigShl (a, b);
}

inline cBigShr operator >> (const cBigNumber& a, const cBigNumber& b)
{
  return cBigShr (a, b);
}

inline cBigShr operator >> (const cBigNumber& a, CBNL b)
{
  return cBigShr (a, b);
}

inline cBigShr operator >> (CBNL a, const cBigNumber& b)
{
  return cBigShr (a, b);
}

#endif
