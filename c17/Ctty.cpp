/* --------------------------------------------------------------
    Compiler independent stream output (version 2.0).

    http://www.imach.uran.ru/cbignum

    Copyright 2005-2010 by Raul N.Shakirov, IMach of RAS(UB).
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
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "Cios.h"
#include "Ctty.h"
#define BITS    (CHAR_BIT * sizeof (CBNL))

#ifdef  _MSC_VER
#pragma auto_inline (off)
#endif

#if 0
#define _CTTY_R
#endif

#ifdef  __cplusplus
extern "C" {
#endif

static  const char*   pszOf1 = " of";
static  const char*   pszOf2 = "";
static  size_t        nOf    = 3;

void    cTTY_SetMaxOutput (const char* psz1, const char* psz2)
{
  nOf = strlen (pszOf1 = psz1) + strlen (pszOf2 = psz2);
}

char* cTTY_ltoa10 (CBNL num, char* buf)
{
  char *b = buf;
  if (num < 0) { *b++ = '-'; num = -num; }
  {
    char *p = b;

    do *p++ = (char) ((int) (num % 10) + '0');
    while ((num /= 10) > 0);

    *p-- = '\0';

    do { char c = *p; *p-- = *b; *b++ = c; }
    while (b < p);
  }
  return buf;
}

size_t  cTTY_PutStream (_CIOSTD ostream& os,
                        const   char*   psz,
                                CBNL    num,
                                CBNL    max,
                                long    cop)
{
  size_t n = 0;
  unsigned width = (unsigned) os.width (0);

  if (psz != NULL) { n = strlen (psz); os << psz; }

  char c = 0;
  if ((num | max | (cop & cTTY_numflags)) != 0L)
  {
    if ((cop & cTTY_numpercent) != 0L)
    {
      if (max != 0 && max != 100) num = (num * 100 + max/2) / max;
      max = 0; c = '%';
    }
    else
    if ((cop & cTTY_numscale) != 0L)
    {
      CBNL abs = num;
      if (abs < 0) abs = -abs;
      if (abs < max) abs = max;
      if (abs >= 0x2600) { num >>=  9; max >>=  9; c = 'K'; abs >>=  9;
      if (abs >= 0x4C00) { num >>= 10; max >>= 10; c = 'M'; abs >>= 10;
      if (abs >= 0x4C00) { num >>= 10; max >>= 10; c = 'G'; abs >>= 10;
      if (abs >= 0x4C00) { num >>= 10; max >>= 10; c = 'T'; abs >>= 10;
      if (abs >= 0x4C00) { num >>= 10; max >>= 10; c = 'P'; }}}}
                           ++num /= 2; ++max /= 2; }
    }

    long unit = os.setf (_CIOSTD ios::unitbuf) & _CIOSTD ios::unitbuf;
    char fill = os.fill (' ');
    char buf [BITS + 8];

    {
      size_t nn = strlen (cTTY_ltoa10 (num, buf));
      nn += cTTY_getnumspace (cop);
      size_t mm = cTTY_getnumwidth (cop);
      if (psz != NULL)
      {
        size_t m = cTTY_getstrwidth (cop) + mm;
        if (n + nn < m) nn = m - n;
      }
      if (nn < mm) nn = mm;
      n += nn;
      os << _CIOSTD setw ((unsigned)nn) << buf;
    }

    if (max != 0L)
    {
      size_t nn = strlen (cTTY_ltoa10 (max, buf));
      nn += cTTY_getnumspace (cop);
      size_t mm = cTTY_getnumwidth (cop);
      if (nn < mm) nn = mm;
      n += nn + nOf;
      os << pszOf1 << _CIOSTD setw ((unsigned)nn) << buf << pszOf2;
    }

    if (unit == 0) os.unsetf (_CIOSTD ios::unitbuf);
    os.fill (fill);

    if (c != 0) { ++n; os << c; }
  }

  c = (char) cop;
  if (c != 0 && c != '\r') { ++n; os << c; }

  os.width (width);
  return n;
}

void    cTTY_CleanStream (_CIOSTD ostream& os,
                                size_t  nSpace,
                                size_t  nBS)
{
  long unit = os.setf (_CIOSTD ios::unitbuf) & _CIOSTD ios::unitbuf;
  char fill = os.fill (' ');
  unsigned width = (unsigned) os.width (0);

  for (; nSpace > 0; nSpace--) os << ' ';
#ifndef _CTTY_R
  for (; nBS    > 0; nBS--   ) os << '\b';
#else
  if (nBS) { nBS = 0; os << '\r'; }
#endif
  os << _CIOSTD flush;

  if (unit == 0) os.unsetf (_CIOSTD ios::unitbuf);
  os.fill (fill);
  os.width (width);
}

void   cTTY_FlushStream (_CIOSTD ostream& os)
{
  os << _CIOSTD flush;
}

#define cTTY_out        _CIOSTD cout
#define cTTY_err        _CIOSTD cerr

static  void*   pWidth  = NULL;
static  size_t  nWidth  = 0;
static  void*   pChars  = NULL;
static  size_t  nChars  = 0;
static  size_t  nClean  = 0;

int     cTTY_StandardOutput    (void*   pt,
                        const   char*   psz,
                                CBNL    num,
                                CBNL    max,
                                long    cop)
{
  while (nClean != 0) cTTY_ProgressOutput (NULL, NULL, 0, 0, '\r');
  const char *p = NULL;
  if (psz)
  {
    if ((p = strrchr (psz, '\r')) != NULL)
    do
      cTTY_StandardOutput (pt, NULL, 0, 0, *psz);
    while (psz++ != p);

    if ((p = strrchr (psz, '\n')) != NULL)
    {
      nWidth = (size_t)(psz - p - 1);
    }
  }

  size_t n = cTTY_PutStream (cTTY_out, psz, num, max, cop);
  if (n != 0) { pWidth = pt; nWidth += n; }

  switch ((char)cop)
  {
    case '\r':  if (nWidth == 0 || pWidth != pt && pt != NULL) break;
                cTTY_PutStream (cTTY_out, NULL, 0, 0, '\n');
    case '\n':  n = nWidth = 0;
    default:    if (n == 0 || p != NULL) cTTY_FlushStream (cTTY_out);
  }

  return 0;
}

int     cTTY_ProgressOutput    (void*   pt,
                        const   char*   psz,
                                CBNL    num,
                                CBNL    max,
                                long    cop)
{
  const char *p = NULL;
  if (psz)
  {
    if ((p = strrchr (psz, '\r')) != NULL)
    do
      cTTY_ProgressOutput (pt, NULL, 0, 0, *psz);
    while (psz++ != p);

    if ((p = strrchr (psz, '\n')) != NULL)
    {
      nWidth = 0; nChars = (size_t)(psz - p - 1); nClean = 0;
    }
    else
    {
      size_t n = strlen (psz);
      if (n + nChars > 79)
      {
        psz += n;
        if (nChars <= 77)
        {
          psz += nChars - 77;
          nChars += cTTY_PutStream (cTTY_err, "..", 0, 0, 0);
          pChars = pt;
        }
      }
    }
  }

  size_t n = cTTY_PutStream (cTTY_err, psz, num, max, cop);
  if (n != 0) { pChars = pt; nChars += n; }

  switch ((char)cop)
  {
    case '\r': if (nClean < nChars) nClean = nChars;
               if (nClean == 0 || pChars != pt && pt != NULL) break;
               cTTY_CleanStream (cTTY_err, nClean - nChars, nClean);
               nClean = nChars; nChars = 0;
               if (nClean > 79) nClean = 0;
               break;
    case '\n': n = nWidth = nChars = nClean = 0;
    default:   if (n == 0 || p != NULL) cTTY_FlushStream (cTTY_err);
  }

  return 0;
}

#ifdef  __cplusplus
}
#endif
