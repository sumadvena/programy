/* --------------------------------------------------------------
    Signed integers with unlimited range (version 2.1c).
    Operations on machine numbers.

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
#ifndef _CBNL_H
#define _CBNL_H

#include <assert.h>
#include <limits.h>

#ifdef  _MSC_VER
#if     _MSC_VER >= 1200
#ifdef  _M_IX86
#if 0
#define _CBNL_C     __fastcall
#endif
#endif
#endif
#endif


#ifndef _CBNL_C
#define _CBNL_C
#endif

#ifndef CBNC
#define CBNC int
#endif

#ifdef  _MSC_VER
#ifdef  _WIN64
#define CBNL __int64
#define CBNL_MIN    _I64_MIN
#define CBNL_MAX    _I64_MAX
#define UCBNL_MAX   _UI64_MAX
#include "Cbnl64.inl"
#endif
#endif


#ifndef CBNL
#ifdef  __cplusplus
#if     __cplusplus >= 201103L
#define CBNL long long
#define CBNL_MIN    LLONG_MIN
#define CBNL_MAX    LLONG_MAX
#define UCBNL_MAX   ULLONG_MAX
#include "Cbnl.inl"
#endif
#endif
#endif


#ifndef CBNL
#define CBNL long
#define CBNL_MIN    LONG_MIN
#define CBNL_MAX    LONG_MAX
#define UCBNL_MAX   ULONG_MAX
#include "Cbnl.inl"
#endif

#ifdef  _MSC_VER
#if     _MSC_VER >= 1200
#ifdef  _M_IX86
#define _CBNL_CBNL  __int64
#define _CBNL_MUL
#endif
#endif
#endif


#ifdef  _MSC_VER
#if     _MSC_VER >= 1400
#ifdef  _M_IX86
#define _CBNL_MI    32
#endif
#ifdef  _WIN64
#define _CBNL_MI    64
#define _CBNL_MUL
#endif
#endif
#endif


#ifdef  _MSC_VER
#if     _MSC_VER >= 1800
#define _CBNL_CBI
#endif
#endif


#define _CBNL_MUL

#ifdef  _CBNL_MI

#include <intrin.h>


#ifdef  _CBNL_CBI

#if     _CBNL_MI == 32
#pragma intrinsic(_addcarry_u32)
#pragma intrinsic(_subborrow_u32)
#define _addCBNL(l1,l2,p)   _addcarry_u32(0,l1,l2,(unsigned*)p)
#define _adcCBNL(c,l1,l2,p) _addcarry_u32(c,l1,l2,(unsigned*)p)
#define _subCBNL(l1,l2,p)   _subborrow_u32(0,l1,l2,(unsigned*)p)
#define _sbbCBNL(c,l1,l2,p) _subborrow_u32(c,l1,l2,(unsigned*)p)
#endif

#if     _CBNL_MI == 64
#pragma intrinsic(_addcarry_u64)
#pragma intrinsic(_subborrow_u64)
#define _addCBNL(l1,l2,p)   _addcarry_u64(0,l1,l2,p)
#define _adcCBNL(c,l1,l2,p) _addcarry_u64(c,l1,l2,p)
#define _subCBNL(l1,l2,p)   _subborrow_u64(0,l1,l2,p)
#define _sbbCBNL(c,l1,l2,p) _subborrow_u64(c,l1,l2,p)
#endif

#endif


#if     _CBNL_MI == 64
#pragma intrinsic(_mul128)
#pragma intrinsic(_umul128)
#define _muldCBNL   _mul128
#define _umuldCBNL  _umul128
#endif


#if     _CBNL_MI == 64
#pragma intrinsic(__shiftleft128)
#pragma intrinsic(__shiftright128)
#define _ushldCBNL(ll,lh,sh) __shiftleft128(ll,lh,(unsigned char)(sh))
#define _ushrdCBNL(ll,lh,sh) __shiftright128(ll,lh,(unsigned char)(sh))
#if 0
#define _ushld1CBNL(ll,lh)   __shiftleft128(ll,lh,1)
#define _ushrd1CBNL(ll,lh)   __shiftright128(ll,lh,1)
#endif
#define _ushlCBNL(l,sh)  ((l) << (unsigned char)(sh))
#define _ushrCBNL(l,sh)  ((l) >> (unsigned char)(sh))
#endif


#if     _CBNL_MI == 32
#if 0
#pragma intrinsic(_bittest)
#define _btCBNL(num,sh)    _bittest(&(num),sh)
#endif
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanReverse)
#define _ubsfCBNL _BitScanForward
#define _ubsrCBNL _BitScanReverse
#endif

#if     _CBNL_MI == 64
#if 0
#pragma intrinsic(_bittest64)
#define _btCBNL(num,sh)    _bittest64(&(num),sh)
#endif
#pragma intrinsic(_BitScanForward64)
#pragma intrinsic(_BitScanReverse64)
#define _ubsfCBNL _BitScanForward64
#define _ubsrCBNL _BitScanReverse64
#endif


#ifdef  __AVX2__

#if     _CBNL_MI == 32
#pragma intrinsic(__lzcnt)
#define _ulzcntCBNL __lzcnt
#endif

#if     _CBNL_MI == 64
#pragma intrinsic(__lzcnt64)
#define _ulzcntCBNL __lzcnt64
#endif

#endif

#endif

#if 0
#undef  _muldCBNL
#undef  _umuldCBNL
#endif

#if 0
#undef  _divdCBNL
#undef  _udivdCBNL
#undef  _moddCBNL
#undef  _umoddCBNL
#endif

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef _CBNL_MUL

CBNL   _CBNL_C  _muldCBNL   (CBNL, CBNL, CBNL*);
unsigned
CBNL   _CBNL_C  _umuldCBNL  (unsigned CBNL, unsigned CBNL, unsigned CBNL*);

#endif

#ifdef  _CBNL_DIV
#ifdef  _CBNL_ML

CBNL   _CBNL_C  _divdCBNL   (CBNL, CBNL, CBNL, CBNL*);
unsigned
CBNL   _CBNL_C  _udivdCBNL  (unsigned CBNL, unsigned CBNL, unsigned CBNL,
                                                           unsigned CBNL*);
CBNL   _CBNL_C  _moddCBNL   (CBNL, CBNL, CBNL);
unsigned
CBNL   _CBNL_C  _umoddCBNL  (unsigned CBNL, unsigned CBNL, unsigned CBNL);

#endif
#endif

int    _CBNL_C  cLongBits       (CBNL);
int    _CBNL_C  cULongBits      (unsigned CBNL);
int    _CBNL_C  cLongExBits     (CBNL);
int    _CBNL_C  cLongExactLog2  (CBNL);

unsigned
CBNL   _CBNL_C  cLongRandom (unsigned long (*pfnRand)());

#ifdef  __cplusplus
}
#endif

#if UCBNL_MAX <= ULONG_MAX
inline unsigned
CBNL  _CBNL_C   cLongRandom (unsigned long (*pfnRand)())
{
  return (unsigned CBNL)(*pfnRand)();
}
#endif

inline unsigned
CBNL   _CBNL_C  cULongMul (
                unsigned CBNL   l1,
                unsigned CBNL   l2,
                unsigned CBNL   p [2]
        )
{
#ifdef  _umuldCBNL
  return (p [0] = _umuldCBNL (l1, l2, p + 1));
#else
#ifdef  _CBNL_CBNL
  *(unsigned _CBNL_CBNL *)p = (unsigned _CBNL_CBNL)l1 * l2;
#else
  p [0] = (l1 & (((CBNL)1 << (CHAR_BIT * sizeof (CBNL) / 2)) - 1)) *
          (l2 & (((CBNL)1 << (CHAR_BIT * sizeof (CBNL) / 2)) - 1));
  p [1] = (l1 >> (CHAR_BIT * sizeof (CBNL) / 2)) *
          (l2 >> (CHAR_BIT * sizeof (CBNL) / 2));
  unsigned CBNL lh = p [1];
  unsigned CBNL lmac = *(CBNL*)((unsigned char *)p + sizeof (CBNL) / 2);
  unsigned CBNL lm = lmac;
  lmac += (l1 & (((CBNL)1 << (CHAR_BIT * sizeof (CBNL) / 2)) - 1)) *
          (l2 >> (CHAR_BIT * sizeof (CBNL) / 2));
  lh += (lmac < lm ? (CBNL)1 << ((CHAR_BIT * sizeof (CBNL) / 2)) : 0);
  lm = lmac;
  lmac += (l2 & (((CBNL)1 << (CHAR_BIT * sizeof (CBNL) / 2)) - 1)) *
          (l1 >> (CHAR_BIT * sizeof (CBNL) / 2));
  lh += (lmac < lm ? (CBNL)1 << ((CHAR_BIT * sizeof (CBNL) / 2)) : 0);
  p [1] = lh;
  *(CBNL*)((unsigned char *)p + sizeof (CBNL) / 2) = lmac;
#endif
  return p [0];
#endif
}

inline
CBNL   _CBNL_C  cLongMul (
                        CBNL    l1,
                        CBNL    l2,
                        CBNL    p [2]
        )
{
#ifdef  _muldCBNL
  return (p [0] = _muldCBNL (l1, l2, p + 1));
#else
  if (l2 < 0) { l1 = -l1; l2 = -l2; }
  if (l1 >= 0)
    return cULongMul (l1, l2, (unsigned CBNL *)p);
  else {
    CBNL l = cULongMul (-l1, l2, (unsigned CBNL *)p);
    p [1] = ~p [1] + ((p[0] = l = -l) == 0);
    return l;
  }
#endif
}

#ifndef _muldCBNL
#define _muldCBNL   _muldCBNL_i
inline
CBNL   _CBNL_C  _muldCBNL_i (
                        CBNL    l1,
                        CBNL    l2,
                        CBNL    *p
        )
{
  CBNL pp [2];
  CBNL l = cLongMul (l1, l2, pp); *p = pp [1];
  return l;
}
#endif

#ifndef _umuldCBNL
#define _umuldCBNL  _umuldCBNL_i
inline unsigned
CBNL   _CBNL_C  _umuldCBNL_i (
                unsigned CBNL   l1,
                unsigned CBNL   l2,
                unsigned CBNL   *p
        )
{
  unsigned CBNL pp [2];
  unsigned CBNL l = cULongMul (l1, l2, pp); *p = pp [1];
  return l;
}
#endif

#ifndef _addCBNL
inline
unsigned char _addCBNL  (unsigned CBNL l1, unsigned CBNL l2, unsigned CBNL *p)
{
  return ((*p = l1 + l2) < l1);
}
#endif

#ifndef _adcCBNL
inline
unsigned char _adcCBNL (unsigned char c,
                        unsigned CBNL l1, unsigned CBNL l2, unsigned CBNL *p)
{
  *p = l1 + l2 + c; return (*p < l1 || *p == l1 && c != 0);
}
#endif

#ifndef _subCBNL
inline
unsigned char _subCBNL (unsigned CBNL l1, unsigned CBNL l2, unsigned CBNL *p)
{
  return ((*p = l1 - l2) > l1);
}
#endif

#ifndef _sbbCBNL
inline
unsigned char _sbbCBNL (unsigned char c,
                        unsigned CBNL l1, unsigned CBNL l2, unsigned CBNL *p)
{
  *p = l1 - l2 - c; return (*p > l1 || *p == l1 && c != 0);
}
#endif

#ifndef _ushldCBNL
inline
unsigned CBNL _ushldCBNL (unsigned CBNL ll, unsigned CBNL lh, unsigned CBNC sh)
{
#if 0
  assert (~0U % (CHAR_BIT * sizeof (CBNL)) == CHAR_BIT * sizeof (CBNL) - 1);
  return ((lh << sh) + (ll >> ((~sh) & (CHAR_BIT * sizeof (CBNL) - 1)) >> 1));
#else
  assert (sh < CHAR_BIT * sizeof (CBNL));
  return ((lh << sh) + (ll >> (CHAR_BIT * sizeof (CBNL) - 1 - sh) >> 1));
#endif
}
#endif

#ifndef _ushrdCBNL
inline
unsigned CBNL _ushrdCBNL (unsigned CBNL ll, unsigned CBNL lh, unsigned CBNC sh)
{
#if 0
  assert (~0U % (CHAR_BIT * sizeof (CBNL)) == CHAR_BIT * sizeof (CBNL) - 1);
  return ((ll >> sh) + (lh << ((~sh) && (CHAR_BIT * sizeof (CBNL) - 1)) << 1));
#else
  assert (sh < CHAR_BIT * sizeof (CBNL));
  return ((ll >> sh) + (lh << (CHAR_BIT * sizeof (CBNL) - 1 - sh) << 1));
#endif
}
#endif

#ifndef _ushld1CBNL
inline
unsigned CBNL _ushld1CBNL (unsigned CBNL ll, unsigned CBNL lh)
{
  return ((lh << 1) + (ll >> (CHAR_BIT * sizeof (CBNL) - 1)));
}
#endif

#ifndef _ushrd1CBNL
inline
unsigned CBNL _ushrd1CBNL (unsigned CBNL ll, unsigned CBNL lh)
{
  return ((ll >> 1) + (lh << (CHAR_BIT * sizeof (CBNL) - 1)));
}
#endif

#ifndef _ushrCBNL
#define _ushrCBNL(l,sh)  ((l) >> (sh))
#endif

#ifndef _ushlCBNL
#define _ushlCBNL(l,sh)  ((l) << (sh))
#endif

#ifndef _btCBNL
inline
unsigned char _btCBNL (unsigned CBNL num, unsigned CBNC sh)
{
  return ((num & ((CBNL)1 << sh)) != 0);
}
#endif

#ifndef _ulzcntCBNL
inline
unsigned CBNL _ulzcntCBNL (unsigned CBNL num)
{
#ifdef  _ubsrCBNL
  unsigned long n;
  return (CHAR_BIT * sizeof (CBNL) - 1 - (_ubsrCBNL (&n, num)? (int)n: -1));
#else
  return (CHAR_BIT * sizeof (CBNL) - cULongBits(num));
#endif
}
#endif

#endif
