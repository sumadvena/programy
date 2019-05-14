/* --------------------------------------------------------------
    Miller strong probable primality test.

    http://www.imach.uran.ru/cbignum

    Copyright 1999-2017 by Dr. Raul N.Shakirov, IMach of RAS(UB).
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
#include <stdlib.h>
#include <string.h>
#include "Cbignum.h"
#include "Cbignumf.h"
#include "Cbignums.h"
#include "Gettimer.h"
#define BITS    (CHAR_BIT * sizeof (CBNL))

#ifdef  _CIOS_STDSTREAM
#include <fstream>
#else //_CIOS_STDSTREAM
#include <fstream.h>
#endif//_CIOS_STDSTREAM

#ifdef  _CIOS_STDSTREAM
using namespace std;
#endif//_CIOS_STDSTREAM

const char szID[] =
    "Primality test. Version 2.1c beta public of " __DATE__ "\n"
    "Copyright 1999-2017 by Dr. Raul N.Shakirov, IMach of RAS(UB)\n"
    ;
const char szHelp[] =
    "Call:   Miller [Options] [Infile]\n"
    "Options:\n"
    "-idle   idle priority\n"
    "-high   high priority\n"
    "-hex    hexadecimal input/output\n"
    "-hexi   hexadecimal input\n"
    "-hexo   hexadecimal output\n"
    "-factor factor primality test (slowest)\n"
    "-proved strong & factor primality test\n"
    "-miller strong & miller primality test (faster)\n"
    "-strong strong probable primality test (fastest)\n"
    "-scan n number of increments by 2\n"
    "-help   show this help and exit\n"
    "\n"
    "Site:   http://www.imach.uran.ru/cbignum\n"
    ;

int     main    (int argc, char **argv)
{
  cin >> dec;
  cout << dec;
  cerr << szID << endl;

  long count = 1;
  long mode = 0;
  const char *pszPriority = "";

  // Initialize

  cBigNumberMessages = cTTY_StandardOutput;
  cBigNumberProgress = cTTY_ProgressOutput;

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
    if (strcmp (argv [1], "-hex") == 0)
    {
      cin >> hex;
      cout << hex;
      continue;
    }
    if (strcmp (argv [1], "-hexi") == 0)
    {
      cin >> hex;
      continue;
    }
    if (strcmp (argv [1], "-hexo") == 0)
    {
      cout << hex;
      continue;
    }
    if (strcmp (argv [1], "-factor") == 0)
    {
      mode = 2; continue;
    }
    if (strcmp (argv [1], "-proved") == 0)
    {
      mode = 3; continue;
    }
    if (strcmp (argv [1], "-miller") == 0)
    {
      mode = 4; continue;
    }
    if (strcmp (argv [1], "-strong") == 0)
    {
      mode = 10; continue;
    }
    if (strcmp (argv [1], "-scan") == 0 && argc > 2)
    {
      count = atol (argv [2]);
      if (count < 0) count = 1;
      --argc, ++argv;
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

  cerr << pszPriority;

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

  #ifdef  _CBIGNUM_SMALL_DIV
    cerr << "\nUse unfolding for divider/module smaller " << (_CBIGNUM_SMALL_DIV * BITS) << " bits";
  #else //_CBIGNUM_SMALL_DIV
  #ifdef  _CBIGNUM_SMALL_POWMOD
    cerr << "\nUse unfolding for power by module smaller " << BITS << " bits";
  #endif//_CBIGNUM_SMALL_POWMOD
  #endif//_CBIGNUM_SMALL_DIV

  cerr << "\n\n";

  // Input numbers.

  cBigNumber num;
  if (argc > 1)
  {
    ifstream fin (argv [1]);
    if (!fin)
    {
       cerr << "Can't read file " << argv [1] << endl;
       return 255;
    }
    fin >> num;
  }
  else
  {
    cerr << "Please, type number to test for primality and press Enter\n";
    cin >> num;
    cerr << "Thank you" << endl;
  }

  // Compute.

  int retcode = 0;
  long timer = GetTimer (0);
  for (; count > 0; count--, num += 2)
  {
    cout << "Proving primality of number " << num << endl;
    cout << dec;
    int ret = 0;
    switch (mode)
    {
      case  0: ret = IsPrime       (num); break;
      case  2: ret = TestFactor    (num); break;
      case  3: ret = IsProvedPrime (num); break;
      case  4: ret = IsMillerPrime (num); break;
      case 10: ret = IsStrongPrime (num); break;
    }
    if (ret > 10)
    {
      cout << endl << "NO MATCH IN RESULTS" << endl;
      cerr << "FAIL!" << endl;
    }
    if (retcode < ret) retcode = ret;
    cout << endl;
  }
  timer = GetTimer (timer);
  cout << "Time=" << setw (6) << timer << " ms" << endl;

  cerr << "Return code " << retcode << endl;

  return retcode;
}
