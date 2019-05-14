/* --------------------------------------------------------------
    Integral arithmetical calculator with unlimited range.

    http://www.imach.uran.ru/cbignum

    Copyright 2002-2017 by Raul N.Shakirov, IMach of RAS(UB).
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
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "Cbignum.h"
#include "Cbignumf.h"
#include "Cbignums.h"
#include "Cthr.h"
#include "Gettimer.h"
#include "Random3.h"
#define BITS    (CHAR_BIT * sizeof (CBNL))

#ifdef  _CIOS_STDSTREAM
#include <fstream>
#else //_CIOS_STDSTREAM
#include <fstream.h>
#endif//_CIOS_STDSTREAM

#ifdef  _CIOS_STDSTREAM
using namespace std;
#endif//_CIOS_STDSTREAM

int nSize;              // Report size of numbers.
int nVerbose;           // Write operands.
int nCheck;             // Check result of operation.

static cTTY ComparingMessages;  // Channel for comparing messages.

static void GenerateRandom (cBigNumber& a, size_t radix, size_t maxexp = 0);
static void GenerateRandom (long a, size_t radix, size_t maxexp = 0);
static void GenerateRandomExp (size_t maxexp = 0);
static void ShowStat (cBigNumber& a, cBigNumber& na);
static void ShowCycles (cBigNumber& nt, long nKHz);

long SetTimer (long &timer)
{
  long temp = GetTimer (0);
  long time = temp - timer;
  timer = temp;

  return (time >= 0? time: 0L);
}

int GetNumberOp (_CIOSTD istream& is, cBigNumber& a, size_t dn)
{
  is >> a >> ws;

  int ch = is.get();
  switch (ch)
  {
    case 'R': case 'r':
          CBNL la = a.toCBNL();
          a.setrandom (lRand3, (la < 0 ? -la : la));
          if (la < 0) a.neg();
          is >> ws; ch = is.get();
          break;
  }
  if (isxdigit(ch))
  {
    is.putback ((char)ch); ch = ' ';    // Error
  }

// Normalization or denormalization for debug purposes.

  if (a.length() < dn) {
    a.checkexpand (dn), a.checkindex (cBigNumberFitTo (EXPTRTYPE(a), dn));
  }
  else if (dn == 0 && a == 0) a.clear();

  return ch;
}

int WriteNumber (_CIOSTD ostream& os, const cBigNumber& b)
{
  assert (&os);
  assert (&b);
  long fill  = 0;
  long flags = (&os? os.flags(): 0);
  unsigned radix = ((flags & _CIOSTD ios::hex)? 16:
                    (flags & _CIOSTD ios::oct)? 8: 10);
  unsigned width = 0;

  if (flags & _CIOSTD ios::internal)
  {
    width = (unsigned) os.width();
    fill  = (long)     os.fill();
  }
#ifdef  _CBIGNUM_UNSIGN_OCT_HEX
  if (radix != 10) fill |= cBigNumber_unsign;
#endif//_CBIGNUM_UNSIGN_OCT_HEX
  if (flags & _CIOSTD ios::uppercase) fill |= cBigNumber_uppercase;
  if (flags & _CIOSTD ios::showbase)  fill |= cBigNumber_showbase;
  if (flags & _CIOSTD ios::showpos)   fill |= cBigNumber_showpos;

  cBigString str;
  os << b.toa (str, radix, width, fill) << '\n';

  if (nCheck)
#ifdef  _CBIGNUM_UNSIGN_OCT_HEX
  if (radix != 10 || b >= 0)
#endif//_CBIGNUM_UNSIGN_OCT_HEX
  {
    cBigNumber c (str, (flags & _CIOSTD ios::showbase)? 0: radix);
    if (c != b)
    {
      os << "NO MATCH IN OUTPUT: DIFFERENCE " << (c -= b) << '\n';
      ComparingMessages <<= "FAIL!";
      return 1;
    }
  }
  return 0;
}

inline size_t size_max (size_t n1, size_t n2)
{
  return ((n1 > n2)? n1 : n2);
}

inline size_t size_min (size_t n1, size_t n2)
{
  return ((n1 < n2)? n1 : n2);
}

cBigNumber& csetmul (const cBigNumber& a, const cBigNumber& b, cBigNumber& c)
{
  size_t la = a.length(), lb = b.length();
  c.checkexpand (la + lb + 2);
  if (la <= lb)
        cBigNumberMul (CBPTRTYPE(a), CBPTRTYPE(b), EXPTRTYPE(c));
  else  cBigNumberMul (CBPTRTYPE(b), CBPTRTYPE(a), EXPTRTYPE(c));
  c.checkindex (c.length());
  return c;
}

cBigNumber& caddmul (const cBigNumber& a, const cBigNumber& b, cBigNumber& c)
{
  size_t la = a.length(), lb = b.length();
  c.checkexpand (size_max (c.length(), la + lb + 1) + 1);
  if (la <= lb)
        cBigNumberMAddMul (CBPTRTYPE(a), CBPTRTYPE(b), EXPTRTYPE(c));
  else  cBigNumberMAddMul (CBPTRTYPE(b), CBPTRTYPE(a), EXPTRTYPE(c));
  c.checkindex (c.length());
  return c;
}

cBigNumber& csubmul (const cBigNumber& a, const cBigNumber& b, cBigNumber& c)
{
  size_t la = a.length(), lb = b.length();
  c.checkexpand (size_max (c.length(), la + lb + 1) + 1);
  if (la <= lb)
        cBigNumberMSubMul (CBPTRTYPE(a), CBPTRTYPE(b), EXPTRTYPE(c));
  else  cBigNumberMSubMul (CBPTRTYPE(b), CBPTRTYPE(a), EXPTRTYPE(c));
  c.checkindex (c.length());
  return c;
}

long total_r = 0;       // Time to read
long total_c = 0;       // Time to calculate
long total_w = 0;       // Time to write
long total = 0;         // Total time

int retcode_w = 0;      // Return code for output thread

struct tpar             // Item of cyclic data buffer
{
  int pp;               // Prefix operation
  int op;               // Operation 1 or 0
  int md;               // Operation 2 or 0

  int alt;              // Do alternative operation
  int rev;              // Do reverse operation
  int arb;              // Do arbitrage operation

  cBigNumber a;         // Operand 1
  cBigNumber b;         // Operand 2
  cBigNumber c;         // Operand 3

  cBigNumber x;         // Result
  cBigNumber y;         // Remainder
  cBigNumber p;         // Result obtained by alternative operation
  cBigNumber q;         // Remainder obtained by alternative operation
  cBigNumber r;         // Result obtained by reverse operation
  cBigNumber s;         // Remainder after reverse operation, must be 0
  cBigNumber t;         // Operand to compare with result of reverse operation
  cBigNumber v;         // Result obtained by arbitrage operation
  cBigNumber w;         // Remainder obtained by arbitrage operation

  void Write (_CIOSTD ostream& os);
};

exvector<tpar> par;         // Cyclic data buffer
volatile size_t ipar = 0;   // Index of data to write
volatile size_t jpar = 0;   // Index of data to calculate

#ifdef  _CTHR_PROCESS

static unsigned __stdcall ProcessThread (void* pParam)
{
  for (;;)
  {
    if (ipar != jpar)
    {
      (*(exvector<tpar>*)pParam) [ipar].Write (cout);
      if (++ipar >= par.size()) ipar = 0;
    }
  }
}

#endif//_CTHR_PROCESS

const char szID[] =
    "Big integer calculator. Version 2.1c beta public of " __DATE__ "\n"
    "Copyright 2002-2017 by Dr. Raul N.Shakirov, IMach of RAS(UB)\n"
    ;
const char szHelp[] =
    "Call:   Arifexp [Options] [Infile]\n"
    "Options:\n"
    "-idle   idle priority\n"
    "-high   high priority\n"
    "-par n  run up to n concurrent threads (actually 1 or 2)\n"
    "-hex    hexadecimal input/output\n"
    "-hexi   hexadecimal input\n"
    "-hexo   hexadecimal output\n"
    "-div0   allow division by 0\n"
    "-rand   generate random numbers instead of computing\n"
    "    all including degree of power and shift\n"
    "      0 generate -8,-4,-2,-1,0,1,2,4,8 instead of 0\n"
    "-exp n  append up to n trailing zeroes to random numbers\n"
    "-dn  n  add/remove extra sign words to fit code to n words\n"
    "-size   show high word and size of numbers\n"
    "-time   show estimation of computing time in Athlon cycles\n"
    "-mhz n  CPU rate for estimation of computing time in ms\n"
    "-rep n  repeat operation n times\n"
    "-v      write expression before the result\n"
    "-check  check results of operations, if possible\n"
    "-help   show this help and exit\n"
    "\n"
    "Site:   http://www.imach.uran.ru/cbignum\n"
    ;

int main (int argc, char** argv)
{
  int  retcode = 0;         // Error code.

  cout << dec;
  cerr << szID << endl;

  const char *pszPriority = "";
  const char *pszGenerate = "";
  const char *pszOutput   = "";
  const char *pszAll      = "";

  int radix    = 10;
  int nHexi    = 0;
  int nTime    = 0;
  long nKHz    = 0;
  int nRepeat  = 1;
  int nPar     = 1;
  int nRand0   = 0;
  int maxexp   = 0;
  size_t dn    = 0;

  // Initialize random generator.

  RRand3();

  // Look for options.

  for (; argc > 1 && argv [1] [0] == '-'; --argc, ++argv)
  {
    if (argv [1] [1] == '-') ++(argv [1]);

    if (strcmp (argv [1], "-idle") == 0)
    {
      if (SetIdlePriority()) pszPriority = "Idle priority\n\n";
      continue;
    }
    if (strcmp (argv [1], "-high") == 0)
    {
      if (SetHighPriority()) pszPriority = "High priority\n\n";
      continue;
    }
    if (strcmp (argv [1], "-par") == 0 && argc > 2)
    {
      nPar = atoi (argv [2]);
      --argc, ++argv;
      continue;
    }
    if (strcmp (argv [1], "-hex") == 0)
    {
      nHexi = 1;
      cout << hex; radix = 16;
      continue;
    }
    if (strcmp (argv [1], "-hexi") == 0)
    {
      nHexi = 1;
      continue;
    }
    if (strcmp (argv [1], "-hexo") == 0)
    {
      cout << hex; radix = 16;
      continue;
    }
    if (strcmp (argv [1], "-div0") == 0)
    {
      cBigNumber::maskdiv0 (1);
      continue;
    }
    if (strcmp (argv [1], "-rand") == 0)
    {
      pszGenerate = "Generating random numbers\n\n";
      if (argc > 2 && strcmp (argv [2], "all") == 0)
      {
        pszAll = argv[2];
        --argc, ++argv;
      }
      if (argc > 2 && strcmp (argv [2], "0") == 0)
      {
        nRand0 = 1;
        --argc, ++argv;
      }
      continue;
    }
    if (strcmp (argv [1], "-exp") == 0 && argc > 2)
    {
      maxexp = atoi (argv [2]);
      --argc, ++argv;
      continue;
    }
    if (strcmp (argv [1], "-dn") == 0 && argc > 2)
    {
      dn = atoi (argv [2]);
      --argc, ++argv;
      continue;
    }
    if (strcmp (argv [1], "-size") == 0)
    {
      nSize = 1;
      continue;
    }
    if (strcmp (argv [1], "-time") == 0)
    {
      nTime = 1;
      continue;
    }
    if (strcmp (argv [1], "-mhz") == 0 && argc > 2)
    {
      nKHz = atoi (argv [2]) * 1000L;
      --argc, ++argv;
      continue;
    }
    if (strcmp (argv [1], "-rep") == 0 && argc > 2)
    {
      nRepeat = atoi (argv [2]);
      --argc, ++argv;
      continue;
    }
    if (strcmp (argv [1], "-v") == 0)
    {
      nVerbose = 1;
      continue;
    }
    if (strcmp (argv [1], "-check") == 0)
    {
      nCheck = 1;
      continue;
    }
    if (strcmp (argv [1], "-help") == 0 ||
        strcmp (argv [1], "-h")    == 0 ||
        strcmp (argv [1], "-?")    == 0)
    {
      cout << szHelp;
      return 0;
    }

    // Unrecognized option.

    cerr << "Unrecognized option " << argv [1];
    cerr << " (type -help to view list of options)" << endl;
    return 255;
  }

  // Open input file

  istream *pin = &cin;
  ifstream fin;
  if (argc > 1)
  {
    fin.open (argv [1]);
    if (!fin)
    {
       cerr << "Can't read file " << argv [1] << endl;
       return 255;
    }
    pin = &fin;
  }

  // Set input mode.

  if (nHexi) *pin >> hex; else *pin >> dec;

  // Set parallel mode.

  cBigNumberMessages = cTTY_StandardOutput;

#ifdef  _CTHR_PROCESS

  if (nPar > 1 && !*pszGenerate &&
    ::_beginthreadex (NULL, 0, ProcessThread, (void*)&par, 0, NULL))
  {
    pszOutput = "\nRun concurrent thread for output";

    par.expand (500);   // Approximate buffer size (may be greater)

    nPar = 2;
  }
  else

#endif//_CTHR_PROCESS

  {
  //ComparingMessages  = cTTY_StandardOutput;
    cBigNumberProgress = cTTY_ProgressOutput;

    par.resize (1);

    if (nPar > 1) nPar = 1;
  }

  // Report process mode.

  cerr << pszPriority;
  cerr << pszGenerate;

  if (!*pszGenerate)
  {
  #ifdef  _CBNL_ASM
    cerr << "Accelerated ";
  #else //_CBIGNUM_ASM
    cerr << "Portable ";
  #endif//_CBIGNUM_ASM
    cerr << BITS << " bit";
  #ifdef  __AVX2__
    cerr << " BMI2";
  #endif//__AVX2__
  #ifndef NDEBUG
    cerr << " debug";
  #endif//NDEBUG
    cerr << " mode";

  #ifdef  _CBIGNUM_HARDWARE_DIV
  #ifdef  _CBIGNUM_SMALL_DIV
  #define HMOD _CBIGNUM_SMALL_DIV
  #else //_CBIGNUM_SMALL_DIV
  #ifdef  _CBIGNUM_SMALL_POWMOD
  #define HMOD _CBIGNUM_SMALL_POWMOD
  #endif//_CBIGNUM_SMALL_POWMOD
  #endif//_CBIGNUM_SMALL_DIV
  #endif//_CBIGNUM_HARDWARE_DIV

  #ifdef  _CBIGNUM_HARDWARE_MUL
    cerr << " with hardware multiplication";
  #if (HMOD)
    cerr << " and division";
  #endif
  #else //_CBIGNUM_HARDWARE_MUL
  #if (HMOD)
    cerr << " with hardware division";
  #endif
  #endif//_CBIGNUM_HARDWARE_MUL

  #ifdef  _CBIGNUM_KARATSUBA_MUL
    cerr << "\nUse Karatsuba method for " << (_CBNL_KARATSUBA_MIN * BITS) << " Kbits and larger numbers";
  #endif//_CBIGNUM_KARATSUBA_MUL

  #ifdef  _CBIGNUM_SMALL_DIV
    cerr << "\nUse unfolding for divider/module smaller " << (_CBIGNUM_SMALL_DIV * BITS) << " bits";
  #else //_CBIGNUM_SMALL_DIV
  #ifdef  _CBIGNUM_SMALL_POWMOD
    cerr << "\nUse unfolding for power by module smaller " << BITS << " bits";
  #endif//_CBIGNUM_SMALL_POWMOD
  #endif//_CBIGNUM_SMALL_DIV

    cerr << pszOutput << "\n\n";
  }

  // Dividers for time estimation.

    #define DCP   ((5 * sizeof (CBNL) / 4) + 10)

  #ifdef  _CBIGNUM_HARDWARE_CBR
    #define DADD ((10 * sizeof (CBNL) / 4) + 5)
  #else //_CBIGNUM_HARDWARE_CBR
    #define DADD   (5 * sizeof (CBNL) / 4)
  #endif//_CBIGNUM_HARDWARE_CBR

  #ifdef _CBIGNUM_TERNARY_MUL
    #define DTABMUL (DADD * 3)
  #else
    #define DTABMUL (DADD * 2)
  #endif

#ifdef  _CBIGNUM_HARDWARE_MUL

  #ifdef  _CBNL_MAC

  #if defined(_CBNL_ML) && defined(_M_AMD64) && defined(__AVX2__)
    #define DMUL (133 * sizeof (CBNL) * sizeof (CBNL) / 8)
  #else
    #define DMUL (100 * sizeof (CBNL) * sizeof (CBNL) / 8)
  #endif
    #define DINP (DMUL * (4 - sizeof (CBNL) / 8) / 2)

  #else //_CBNL_MAC

    #define DMUL1 (DADD * 4 * sizeof (CBNL))
    #define DMUL2 (100 * sizeof (CBNL) * sizeof (CBNL) / 8)
    #define DMUL ((DMUL1 * DMUL2) / (DMUL1 + DMUL2))
    #define DINP (DMUL * 2)

  #endif//_CBNL_MAC

#else //_CBIGNUM_HARDWARE_MUL

  #ifdef _CBIGNUM_SHIFTTAB_MUL
    #define DMUL (DTABMUL * (10 - sizeof (CBNL) / 8) / 10)
  #else
    #define DMUL (DADD)
  #endif
    #define DINP (DTABMUL * 11 / 5)

#endif//_CBIGNUM_HARDWARE_MUL

  #ifdef _CBIGNUM_KARATSUBA_MUL
    #define DMKR  (15 * sizeof (CBNL) * _CBNL_KARATSUBA_MIN)
    #define DIKR (250 * sizeof (CBNL) * _CBNL_KARATSUBA_MIN)
  #endif

    #define DDIV (DADD * 2)
    #define DOUT (DDIV * 5 / 2)
    #define DSQR (DADD * 8)

  // Must round size of short operand if hardware multiplication is in use.

  #ifdef  _CBIGNUM_HARDWARE_MUL
    #define BRND(n) ((n | (BITS-1)) + 1)
  #else //_CBIGNUM_HARDWARE_MUL
    #define BRND(n) (n)
  #endif//_CBIGNUM_HARDWARE_MUL

  // Allocate big numbers (must be initialized within cycle before use).

  cBigNumber a;         // Operand 1
  cBigNumber b;         // Operand 2
  cBigNumber c;         // Operand 3
  cBigNumber null;      // Null number

  cBigNumber na;        // Number of bits in a
  cBigNumber nb;        // Number of bits in b
  cBigNumber nc;        // Number of bits in c
  cBigNumber nt;        // Estimated number of CPU cycles
  cBigNumber total_nt;  // Total estimated number of CPU cycles

  cBigNumber x;         // Result
  cBigNumber y;         // Remainder
  cBigNumber p;         // Result obtained by alternative operation
  cBigNumber q;         // Remainder obtained by alternative operation
  cBigNumber r;         // Result obtained by reverse operation
  cBigNumber s;         // Remainder after reverse operation, must be 0
  cBigNumber t;         // Operand to compare with result of reverse operation
  cBigNumber v;         // Result obtained by arbitrage operation
  cBigNumber w;         // Remainder obtained by arbitrage operation

  long timer = 0;       // Timer

  // Process expressions.

  int ch;
  do
  {
    // Get expression.

    if (argc > 1)
    {
      ch = fin.get();
      if (ch == EOF) break;
      fin.putback ((char)ch);
    }
    else
    {
      cerr << "Please, type expression "\
            "a+-*/%\\|&^?b= a+-b*c= a\\b%c= a<<>>b= @U~b= 2MZVb= aR=\n"\
            "and press Enter or press Enter to exit\n";

      ch = cin.get();
      if (ch == EOF || ch == '\r' || ch == '\n') break;
      cin.putback ((char)ch);
    }

    // Start timer.

    timer = GetTimer (0);

    a = 0;              // Operand 1
    b = 0;              // Operand 2
    c = 0;              // Operand 3
    null = 0;           // Null number

    int pp = ch;        // First symbol
    int op = 0, md = 0; // Operators
    unsigned error = 0; // Error mask

    unsigned alt = 1;   // Use alternative operation in check mode
    unsigned rev = 1;   // Use reverse operation in check mode
    unsigned arb = 0;   // Use simplified arbitrage algorithm in check mode

    ch = GetNumberOp (*pin, a, dn);

    switch (ch)
    {
      case '<': case '>': op = ch; *pin >> ws; ch = pin->get();
                          if (op != ch) op = ch = ' ';  // Error
    }

    if (ch != EOF && ch != '=')
    {
      op = ch; ch = GetNumberOp (*pin, b, dn);
    }

    if (ch != EOF && ch != '=' && ch != ' ')
    {
      md = ch; ch = GetNumberOp (*pin, c, dn);
    }

    // Accumulate time to read.

    total_r += SetTimer (timer);

    // Check operands to be correct and calculate number of cycles.

    na.setbits (a);     // Number of bits in a
    nb.setbits (b);     // Number of bits in b
    nc.setbits (c);     // Number of bits in c
    nt = 0;             // Estimated number of CPU cycles

    if (op != 0)
    {
      if (md == 0 || md != '*' && md != '%')
      switch (op)
      {
        case '+':   if (pp == op && a == 0) { op = +1; a = b; arb = 1;
                      if (nTime && nKHz < 0) nt = nb / DCP;
                    } else
                      if (nTime && nKHz < 0) nt = na / DCP + nb / DADD;
                    break;
        case '-':   if (pp == op && a == 0) { op = -1; a = b; arb = 1;
                      if (nTime && nKHz < 0) nt = nb / DCP;
                    } else
                      if (nTime && nKHz < 0) nt = na / DCP + nb / DADD;
                    break;
        case '*':   if (a == 0 || b == 0) rev = 0;
                    if (nTime) {
                    #ifdef _CBIGNUM_KARATSUBA_MUL
                      nt.setbits ((na < nb? na: nb) / DMKR);
                      nt = BRND(na) * BRND(nb) *
                        cBigPow (3, nt) / (cBigPow (4, nt) * DMUL);
                    #else
                      nt = BRND(na) * BRND(nb) / DMUL;
                    #endif
                    }
                    arb = 1;
                    break;
        case '/':
        case '%':   if (b.length() > _CBNL_TAB_MIN) alt = 0;
                    if (nTime) { nt = nb; if (na > nb) nt *= (na - nb);
                               #ifdef _CBIGNUM_SHIFTTAB_DIV
                                 if (b.length() <= _CBNL_TAB_MAX)
                                   nt /= DDIV;
                                 else
                               #endif
                                   nt /= DADD;
                               }
                    break;
        case '\\':  switch (b.loword()) {
                      case 32: case 16: case 8: case 4: case 2: case 1:
                             if (b.length() == 1) break;
                      default: alt = 0; rev = 0;
                    }        break;
        case '>':   rev = 0;
        case '<':   if (b <= 5) arb = 1;
                             break;
        case '|':   rev = 0; break;
        case '&':   rev = 0; break;
        case '^':            break;
        case '~':   arb = 1;
        case '@':
        case 'U':
        case 'u':   rev = 0;
                    if (pp == op && a == 0) { a = b; break; }
                    error = 0x0001; break;
        case 'M':
        case 'm':   rev = 0;
                    if ((CBNL)b.words() > (CBNL)(CBNL_MAX / BITS)) alt = 0;
                    if (a != 2) error = 0x0002;
                    break;
        case 'Z':
        case 'z':   rev = 0;
                    if ((CBNL)b.exwords() >
                        (CBNL)((CBNL_MAX - (BITS - 1)) / BITS)) alt = 0;
                    if (a != 2) error = 0x0002;
                    break;
        case 'V':
        case 'v':   alt = 0;
                    if (nTime) nt = nb * nb / DSQR;
                    if (a != 2) error = 0x0002;
                    break;
        case '?':   rev = 0; break;
        default:    error = 0x0100;
      }
      if (md == 0) ;
      else if (md == '*')
      switch (op)
      {
        case '+':
        case '-':   if (b == 0 || c == 0) rev = 0;
                    if (nTime) {
                    #ifdef _CBIGNUM_KARATSUBA_MUL
                      nt.setbits ((nb < nc? nb: nc) / DMKR);
                      nt = BRND(nb) * BRND(nc) *
                        cBigPow (3, nt) / (cBigPow (4, nt) * DMUL);
                    #else
                      nt = BRND(nb) * BRND(nc) / DMUL;
                    #endif
                    }
                    arb = 1;
                    break;
        default:    error = 0x0100;
      }
      else if (md == '%')
      switch (op)
      {
        case '\\':  null = na; null *= b; null /= 8;
                    if (null > (EXSIZE_T_MAX & (size_t)0x00FFFFFFUL) / 2 ||
                        c == 0) alt = 0;
                    rev = 0;
                    if (nTime) {
                    #ifdef _CBIGNUM_KARATSUBA_MUL
                      nt.setbits (nc / DMKR);
                      nt = nc * nc *
                           cBigPow (3, nt) / (cBigPow (4, nt) * DMUL);
                    #else
                      nt = nc * nc / DMUL;
                    #endif
                      nt += nc * nc / DDIV;
                      nt *= nb;
                      nt += (nt >> 1);
                    }
                    break;
        default:    error = 0x0100;
      }
      else error |= 0x0200;

      nt *= nRepeat;
    }
    else if (nTime)
    {
      #ifdef _CBIGNUM_KARATSUBA_MUL
        nt.setbits (na / DIKR);
        nt = (na * na * cBigPow (3, nt)) / (cBigPow (4, nt) * DINP);
      #else
        nt = na * na / DINP;
      #endif
    }

    // Put error diagnostics

    if (ch == ' ')
    {
      ch = pin->get();                          // Check for hex input.
      if (isxdigit (ch))
      {
        error &= 0x00ff; error |= 0x1000;       // Skip until =
        while (ch != EOF && ch != '=') ch = pin->get();
      }
    }

    while (ch != EOF && ch != '=')
    {
      error |= 0x4000; ch = pin->get();         // Skip until =
    }

    for (ch = pin->get(); ch != EOF && ch != '\n'; ch = pin->get())
    {
      if (!isspace (ch)) error |= 0x8000;       // Skip rest of line
    }

    if (error & 0x0001)
      cerr << "Error: excessive operand before operator " << (char)op << endl;

    if (error & 0x0002)
      cerr << "Error: incorrect operand before operator " << (char)op << endl;

    if (error & 0x0100)
      cerr << "Error: 1st operator"
           << " is either missed, unknown or not allowed here" << endl;

    if (error & 0x0200)
      cerr << "Error: 2nd operator"
           << " is either missed, unknown or not allowed here" << endl;

    if (error & 0x1000)
      cerr << "Error: hexadecimal input not allowed" << endl;

    if (error & 0x4000)
      cerr << "Error: excessive input before sign =" << endl;

    if (error & 0x8000)
      cerr << "Error: misplaced input after sign =" << endl;

    if (error)
    {
      retcode |= 255;
      cerr << endl;
      continue;
    }

    // Generate random numbers.

    if (*pszGenerate != 0)
    {
      switch (op)
      {
        case  +1:
        case  -1:   cout << (char) pp;
        case '~':
        case '@':
        case 'U': case 'u':
                    cout << (char) pp;
                    break;
        case 'M': case 'm':
        case 'Z': case 'z':
        case 'V': case 'v':
                    cout << a << endl << (char) op;
                    break;
        default:    if (a != 0 || nRand0 == 0)
                         GenerateRandom (a, radix, maxexp);
                    else GenerateRandomExp (maxexp);
                    cout << endl;
                    if (op != 0) cout << (char) op;
      }

      if (op != 0)
      {
        switch (op)
        {
          case '<':
          case '>': cout << (char) op;
          case '\\':if (*pszAll == 0)
                    {
                       cout << endl << b;
                       break;
                    }
          default:  cout << endl;
                    if (b != 0 || nRand0 == 0)
                         GenerateRandom (b, radix, maxexp);
                    else GenerateRandomExp (maxexp);
        }
        if (md != 0)
        {
          cout << endl << (char) md << endl;
          if (c != 0 || nRand0 == 0)
               GenerateRandom (c, radix, maxexp);
          else GenerateRandomExp (maxexp);
        }
        cout << endl;
      }
      cout << '=' << endl;
      continue;
    }

    // Report dividers for time estimation.

    if (nTime && nKHz < 0)
    {
//  cerr << 'D' << BITS;
      cerr << "CP="   << DCP;
      cerr << " ADD=" << DADD;
      cerr << " MUL=" << DMUL;
    #ifdef DMUL1
      cerr << '(' << DMUL1 << ',' << DMUL2 << ')';
    #endif
      cerr << " DIV=" << DDIV;
      cerr << " SQR=" << DSQR;
      cerr << " INP=" << DINP;
      cerr << " OUT=" << DOUT;
    #ifdef _CBIGNUM_KARATSUBA_MUL
      cerr << " MKR=" << DMKR;
      cerr << " IKR=" << DIKR;
    #endif
      cerr << "\n\n";
      nKHz = -nKHz;
    }

    // Report size of numbers.

    if (nSize)
    {
      switch (op)
      {
        case  +1:
        case  -1:
        case '~':
        case '@':
        case 'U': case 'u':
        case 'M': case 'm':
        case 'Z': case 'z':
        case 'V': case 'v':
                    break;
        default:    ShowStat (a, na);
      }
      if (op != 0)  ShowStat (b, nb);
      if (md != 0)  ShowStat (c, nc);
    }

    // Accumulate time.

    total += SetTimer (timer);

    // Report number of cycles for operation.

    if (nTime && nt > 0)
    {
      cerr << "Estimating " << setw (6);
      ShowCycles (nt, nKHz);
      if (op != 0) total_nt += nt;
      else
      {
        cerr << " for input " << setw (6);
        cerr << endl;
        cerr << "Estimating " << setw (6);
        ShowCycles (nt = (na * na) / DOUT, nKHz);
        cerr << " for output";
      }
      cerr << endl;
    }

    // Start timer.

    timer = GetTimer (0);

    // Main calculations.

    if (op != 0)
    {
      // Obtain the result.

      for (int n = 0;;)
      {
        if(!n)cerr << "Computing ";

        x = a;          // Result 1
        y = 0;          // Result 2

        if (md == 0)
        switch (op)
        {
          case  +1: if (!n) cerr <<"++b";       ++x;                break;
          case  -1: if (!n) cerr <<"--b";       --x;                break;
          case '+': if (!n) cerr <<"a+b";       x += b;             break;
          case '-': if (!n) cerr <<"a-b";       x -= b;             break;
          case '*': if (!n) cerr <<"a*b";       x *= b;             break;
          case '/': if (!n) cerr <<"a/b,a%b";   x.setdivmod (y = a, b);
                                                                    break;
          case '%': if (!n) cerr <<"a%b";       x %= b;             break;
          case '\\':if (!n) cerr <<"a\\b (pow)";x.pow (b);          break;
          case '<': if (!n) cerr <<"a<<b";      x <<= b;            break;
          case '>': if (!n) cerr <<"a>>b";      x >>= b;            break;
          case '|': if (!n) cerr <<"a|b (or)";  x |= b;             break;
          case '&': if (!n) cerr <<"a&b (and)"; x &= b;             break;
          case '^': if (!n) cerr <<"a^b (xor)"; x ^= b;             break;
          case '~': if (!n) cerr <<"~b";        x.setcompl (x);     break;
          case '@': if (!n) cerr <<"abs(b)";    x.setabs (b);       break;
          case 'U':
          case 'u': if (!n) cerr <<"unsign(b)"; x.setunsign (b);    break;
          case 'M':
          case 'm': if (!n) cerr <<"bits(b)";   x.setbits (b);      break;
          case 'Z':
          case 'z': if (!n) cerr <<"exbits(b)"; x.setexbits (b);    break;
          case 'V':
          case 'v': if (!n) cerr <<"sqrt(b),b-sqrt(b)*sqrt(b)";
                                                x.setsqrtrm (y = b);break;
          case '?': if (!n) cerr <<"a?b (cmp)"; x = x.comp (b);     break;
        }
        else if (md == '*')
        switch (op)
        {
          case '+': if (!n) cerr <<"a+b*c";     x.addmul (b, c);    break;
          case '-': if (!n) cerr <<"a-b*c";     x.submul (b, c);    break;
        }
        else if (md == '%')
        switch (op)
        {
          case '\\':if (!n) cerr <<"a\\b%c";    x.powmod (b, c);    break;
        }

        if (!n && nRepeat > 1) cerr <<" "<< nRepeat << " times";
        if (++n >= nRepeat) { cerr << endl; break; }
      }
    }
    else // Just copy the operand.
    {
      x = a;
    }

    // Comparing by alternative operation.

    if (nCheck != 0 && op != 0 && alt != 0)
    {
      p = 0;            // Number to compare with result
      q = 0;            // Number to compare with result

      cerr << "Computing ";

      if (md == 0)
      switch (op)
      {
        case +1:  cerr <<"b+1";         p = b + 1;              break;
        case -1:  cerr <<"b-1";         p = b - 1;              break;
        case '+': cerr <<"a+b";         p = a + b;              break;
        case '-': cerr <<"a-b";         p = a - b;              break;
        case '*': cerr <<"b*a";         csetmul (a, b, p);      break;
        case '/': cerr <<"a/b,a%b (tab)";
                            if ((a.hiword() ^ b.hiword()) >= 0 || b == 0)
                                        p.setdivmodtab (q = a, b.tab());
                            else      { p.setdivmodtab (q = a, (-b).tab());
                                        p.neg(); }              break;
        case '%': cerr <<"a%b (tab)";         p = a % b;
                            if ((a.hiword() ^ b.hiword()) >= 0 || b == 0)
                                        p.setmodtab (a, b.tab());
                            else        p.setmodtab (a, (-b).tab());
                                                                break;
        case '\\':cerr <<"a";           p = a;
                  switch (b.loword()) {
          case 32:cerr << "\\2";        p.pow2();
          case 16:cerr << "\\2";        p.pow2();
          case 8: cerr << "\\2";        p.pow2();
          case 4: cerr << "\\2";        p.pow2();
          case 2: cerr << "\\2";        p.pow2();
                  }                                             break;
        case '<': if (b >= 0)
                        { cerr <<"a*2\\b";  p = a * cBigPow (2, b); }
                  else  { cerr <<"floor(a/2\\-b)";
                                        p = cBigPow (2, -b);
                            if (a >= 0) p = a / p;
                            else        p = (a - p + 1) / p; }  break;
        case '>': if (b < 0)
                        { cerr <<"a*2\\-b"; p = a * cBigPow (2, -b); }
                  else  { cerr <<"floor(a/2\\b)";
                                        p = cBigPow (2, b);
                            if (a >= 0) p = a / p;
                            else        p = (a - p + 1) / p; }  break;
        case '|': cerr <<"~(~a&~b)";    p = ~(~a & ~b);         break;
        case '&': cerr <<"~(~a|~b)";    p = ~(~a | ~b);         break;
        case '^': cerr <<"~a&b|a&~b";   p = (~a & b) | (a & ~b);break;
        case '~': cerr <<"b^~0";        p = b^(~(CBNL)0);       break;
        case '@': cerr <<"b.abs()";     p = b.abs();            break;
        case 'U':
        case 'u': cerr <<"b.unsign()";  p = b.unsign();         break;
        case 'M':
        case 'm': cerr <<"b.bits()";    p = b.bits();           break;
        case 'Z':
        case 'z': cerr <<"b.exbits()";  p = b.exbits();         break;
        case '?': cerr <<"a-b?";        p = a; p -= b; p = p.comp0();
                                                                break;
      }
      else if (md == '*')
      switch (op)
      {
        case '+': cerr <<"a+c*b";       caddmul (b, c, p = a);  break;
        case '-': cerr <<"a-c*b";       csubmul (b, c, p = a);  break;
      }
      else if (md == '%')
      switch (op)
      {
        case '\\':cerr <<"(a\\b)%c";    p = cBigPow (a, b) % c; break;
      }
      cerr << endl;
    }

    // Do arbitrage operation.

    if (nCheck != 0 && op != 0 && arb != 0)
    {
      v = 0;            // Number to vote for result

      cerr << "Computing ";

      if (md == 0)
      switch (op)
      {
        case +1:  cerr <<"1+b";         v = 1 + b;              break;
        case -1:  cerr <<"-(1-b)";      v = - (1 - b);          break;
        case '~': cerr <<"~0^b";        v = (~(CBNL)0)^b;       break;
        case '<': cerr <<"a";           v = a;
                  switch (b.loword()) {
          case 5: cerr << "*2";         v.mul2();
          case 4: cerr << "*2";         v.mul2();
          case 3: cerr << "*2";         v.mul2();
          case 2: cerr << "*2";         v.mul2();
          case 1: cerr << "*2";         v.mul2();
                  }                                             break;
        case '>': cerr <<"a";           v = a;
                  switch (b.loword()) {
          case 5: cerr << "/2";         v.div2();
          case 4: cerr << "/2";         v.div2();
          case 3: cerr << "/2";         v.div2();
          case 2: cerr << "/2";         v.div2();
          case 1: cerr << "/2";         v.div2();
                  }                                             break;
        case '*': cerr <<"a*b (tab)";   if (b >= 0) v.addmultab (a.tab(), b);
                                               else v.submultab (a.tab(), -b);
                                        break;
      }
      else if (md == '*')
      switch (op)
      {
        case '+': cerr <<"a+b*c (tab)"; v = a;
                                        if (c >= 0) v.addmultab (b.tab(), c);
                                               else v.submultab (b.tab(), -c);
                                        break;
        case '-': cerr <<"a-b*c (tab)"; v = a;
                                        if (c >= 0) v.submultab (b.tab(), c);
                                               else v.addmultab (b.tab(), -c);
                                        break;
      }
      cerr << endl;
    }

    // Do reverse operation.

    if (nCheck != 0 && op != 0 && rev != 0)
    {
      r = x;            // Number to compare with operand
      s = 0;            // Number to compare with 0
      t = a;            // Operand to compare

      cerr << "Computing ";

      if (md == 0)
      switch (op)
      {
        case  +1: cerr << "--++b";      --r;                break;
        case  -1: cerr << "++--b";      ++r;                break;
        case '+': cerr << "a+b-b";      r -= b;             break;
        case '-': cerr << "a-b+b";      r += b;             break;
        case '*': cerr << "a*b/b";      r.setdivmod (s = r, b);
                                                            break;
        case '/': cerr << "a/b*b+a%b";  r *= b; r += y;     break;
        case '%': cerr << "a%b+a/b*b";  r += (a/b)*b;       break;
        case '\\':if ((b.loword() & 1) == 0 && t < 0) t.neg();
                  switch (b.loword()) {
          case 32:cerr << "sqrt(a\\32)";r.setsqrtrm (s = r);
                                                     if (s) break;
          case 16:cerr << "sqrt(a\\16)";r.setsqrtrm (s = r);
                                                     if (s) break;
          case 8: cerr << "sqrt(a\\8)"; r.setsqrtrm (s = r);
                                                     if (s) break;
          case 4: cerr << "sqrt(a\\4)"; r.setsqrtrm (s = r);
                                                     if (s) break;
          case 2: cerr << "sqrt(a\\2)"; r.setsqrtrm (s = r);
                  }                                         break;
        case '<': cerr << "(a<<b)>>b";  r >>= b;            break;
        case '^': cerr << "a^b^b";      r ^= b;             break;
        case '~': cerr <<"~~b";         r = ~r;             break;
        case 'V':
        case 'v': cerr << "sqrt(b)*sqrt(b)+(b-sqrt(b)*sqrt(b))";
                               t = b; r.pow2(); r += y;     break;
      }
      else if (md == '*')
      switch (op)
      {
        case '+': cerr << "(a+b*c-a)/c";r = r - a;
                                        r.setdivmod (s = r, c);
                                        t = b;              break;
        case '-': cerr << "(a-a-b*c)/c";r = a - r;
                                        r.setdivmod (s = r, c);
                                        t = b;              break;
      }
      cerr << endl;
    }

    // Accumulate time to compute.

    total_c += SetTimer (timer);

    // Get buffer item.

    tpar* ppar = &(par [jpar]);

    // Copy data.

    ppar->pp  = pp;  ppar->op  = op;  ppar->md  = md;
    ppar->alt = alt; ppar->rev = rev; ppar->arb = arb;
    ppar->a = a; ppar->b = b; ppar->c = c; ppar->x = x; ppar->y = y;
    if (alt != 0) { ppar->p = p; ppar->q = q; }
    if (rev != 0) { ppar->r = r; ppar->s = s; ppar->t = t; }
    if (arb != 0) { ppar->v = v; ppar->w = w; }

#ifdef  _CTHR_PROCESS

    // Show output thread that data is ready.

    if (*pszOutput != 0)
    {
      size_t j = jpar + 1;
      if (j >= par.size()) j = 0;
      do continue; while (ipar == j);   // Wait if buffer is full
      jpar = j;
    }
    else

#endif//_CTHR_PROCESS

    // Output data.

    {
      ppar->Write (cout);
    }

    // Accumulate total time.

    total += SetTimer (timer);
  }
  while (ch != EOF);

  // Wait for output thread to be ready

  if (*pszOutput != 0 && ipar != jpar)
  {
    // Start timer.

    timer = GetTimer (0);

    cerr << "Wait for writing of results..." << endl;
    do continue; while (ipar != jpar);

    // Accumulate total time.

    total += SetTimer (timer);
  }

  cout << dec;

  if (*pszGenerate == 0)
  {
    cout << "Reading    " << setw (6) << total_r << " ms\n"; total += total_r;
    cout << "Computing  " << setw (6) << total_c << " ms";   total += total_c;
    if (nRepeat > 1)
      cout << " (" << ((total_c * 2 + nRepeat) / (nRepeat * 2)) << " ms "
           << nRepeat << " times)";
    if (nKHz && total_nt != 0)
      cout << ", " << ((cBigMul (total_c, nKHz * 100) + total_nt/2) / total_nt)
           << "% of estimation";
    cout << "\n";
    cout << "Writing    " << setw (6) << total_w << " ms\n";
    cout << "Total      " << setw (6) << total   << " ms";
    if (nPar > 1) cout << " for main of " << nPar << " threads";
    cout << "\n";
  }

  if (retcode_w != 0) cerr << endl << "Comparing FAILED!" << endl;

  retcode |= retcode_w;
  return retcode;
}

// Output function

void tpar::Write (_CIOSTD ostream& os)
{
  {
    // Start timer.

    long timer = GetTimer (0);

    // Write operands.

    if (nVerbose != 0)
    {
      switch (op)
      {
        case  +1:
        case  -1:   os << (char) pp << (char) pp;
                    break;
        default:    retcode_w |= WriteNumber (os, a);
      }
      if (op != 0)
      {
        switch (op)
        {
          case '<':
          case '>': os << (char) op;
          default:  os << (char) op;
          case  +1:
          case  -1: os << '\n';
                    retcode_w |= WriteNumber (os, b);
        }
        if (md != 0)
        {
          os << (char) md << '\n';
          retcode_w |= WriteNumber (os, c);
        }
      }
      os << '=' << '\n';
    }

    // Write the result.

    retcode_w |= WriteNumber (os, x);
    switch (op)
    {
      case '/':
      case 'V':
      case 'v': retcode_w |= WriteNumber (os, y);
                break;
    }

    // Comparing with result of alternative operation.

    if (nCheck != 0 && op != 0 && alt != 0)
    {
      ComparingMessages << "Comparing... ";

      if (x == p && y == q)
      {
        ComparingMessages <<= "OK";
      }
      else
      {
        ComparingMessages <<= "FAIL!";
        os << "NO MATCH OF RESULTS 1 AND 2" << '\n';
        retcode_w |= WriteNumber (os, p);
        switch (op)
        {
          case '/':
          case 'V':
          case 'v': retcode_w |= WriteNumber (os, q);
                    break;
        }
        retcode_w |= 1;
      }
    }

    // Comparing with result of arbitrage operation.

    if (nCheck != 0 && op != 0 && arb != 0)
    {
      ComparingMessages << "Comparing... ";

      int f = 0;
      if (x == v) f |= 1;
      if (p == v) f |= 2;
      if (f == 2) x = p;    // Store confirmed result.

      if (f)
      {
        ComparingMessages << "OK";
        if (f != 3)
        {
          ComparingMessages << f;
          os << "NO MATCH OF RESULTS " << (3 - f) << " AND 3 " << '\n';
          os << "RESULT " << f << " IS CONFIRMED BY RESULT 3" << '\n';
          retcode_w |= WriteNumber (os, v);
          retcode_w |= 1;
        }
        ComparingMessages <<= "";
      }
      else
      {
        ComparingMessages <<= "FAIL!";
        os << "NO MATCH OF RESULTS 1 AND 3" << '\n';
        os << "NO MATCH OF RESULTS 2 AND 3" << '\n';
        retcode_w |= WriteNumber (os, v);
        retcode_w |= 1;
      }
    }

    // Comparing operand with result of reverse operation.

    if (nCheck != 0 && op != 0 && rev != 0)
    {
      ComparingMessages << "Comparing... ";

      if (r == t && s == 0)
      {
        ComparingMessages <<= "OK";
      }
      else
      {
        ComparingMessages <<= "FAIL!";
        os << "NO MATCH AFTER REVERSE OPERATION" << '\n';
        retcode_w |= WriteNumber (os, r);
        if (md == 0)
        switch (op)
        {
          case '*':
          case '\\':retcode_w |= WriteNumber (os, s);
        }
        else if (md == '*')
        switch (op)
        {
          case '+':
          case '-': retcode_w |= WriteNumber (os, s);
        }
        retcode_w |= 1;
      }
    }

    // Completing.

    os << '\n';

    // Accumulate time to write.

    total_w += SetTimer (timer);
  }
}

static void GenerateRandom (cBigNumber& n, size_t radix, size_t maxexp)
{
  GenerateRandom (n.tolong(), radix, maxexp);
}

static void GenerateRandom (long n, size_t radix, size_t maxexp)
{
  assert (radix > 1);

  if (n < 0) { cout << '-'; n = -n; }

  if (maxexp != 0) maxexp = (int) (dRand3() * maxexp);
  if (nSize)
  {
    cerr << "Generating random number of length "<< n;
    if (maxexp != 0) cerr << " and exponent " << (long)maxexp;
    cerr << endl;
  }

  if (n > 0)
  {
    cout << (int) (1 + (dRand3() * (radix - 1)));
    while (--n != 0) cout << (int) (dRand3() * radix);
    for (; maxexp > 0; --maxexp) cout << 0;
  }
  else
  {
    cout << n;
  }
}

static int ExpSet[]={-8,-4,-2,-1,0,1,2,4,8};
static void GenerateRandomExp (size_t maxexp)
{
  if (maxexp != 0) maxexp = (int) (dRand3() * maxexp);

  if (nSize)
  {
    cerr << "Generating random number of set -8,-4,-2,-1,0,1,2,4,8";
    if (maxexp != 0) cerr << " and exponent " << (long)maxexp;
    cerr << endl;
  }

  int n = ExpSet [(int) (dRand3() * 9)];
  cout << n;
  if (n != 0) for (; maxexp > 0; --maxexp) cout << 0;
}

static void ShowStat (cBigNumber& a, cBigNumber& na)
{
  cerr << (const char*) cHexDump (a.hiword());
  if (a.length() > 1)
  {
    cHexDump a2 (a.code() [a.length() - 2]);
    ((char*) (const char*) a2) [2] = 0;
    cerr <<" "<< (const char*) a2 <<"..";
  }
  else
    cerr <<"     ";

  cerr <<": "<< setw (7) << na <<" bit" << (na != 1?"s":" ");
  size_t la = a.length();
  cerr <<" in "<< setw (5) << (long)la <<" word" << ("s" + (la == 1));
  cerr << endl;
  //a.dump();
  //a.info();
}

static void ShowCycles (cBigNumber& nt, long nKHz)
{
  if (nt < 1000000L)
    cerr << ((nt + 499) / 1000) << 'K';
  else
    cerr << ((nt + 499999L) / 1000000L) << 'M';
  cerr << " Athlon cycles";
  if (nKHz)
    cerr << " or " << setw (5) << ((nt + nKHz / 2) / nKHz) << " ms for "
                   << setw (0) << (nKHz / 1000) << " MHz CPU";
}
