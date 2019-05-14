/* --------------------------------------------------------------
    Multiplication of unlimited integer matrixes.

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
#include <math.h>
#include <stdlib.h>
#include "Cbignum.h"
#include "Cbignums.h"
#include "Gettimer.h"

#ifdef  _CIOS_STDSTREAM
#include <fstream>
#else //_CIOS_STDSTREAM
#include <fstream.h>
#endif//_CIOS_STDSTREAM

#ifdef  _CIOS_STDSTREAM
using namespace std;
#endif//_CIOS_STDSTREAM

/* ==============================================================
    Compilation-time parameters:
    num     Type of numbers.
    DIM     Dimension for static allocation of memory.
            If not specified, dynamic arrays are used.
============================================================== */

typedef cBigNumber num;
//typedef int num;
//#define DIM 100

int main (int argc, char **argv)
{
  if (argc < 3) return 0;
#ifdef DIM
  num m1 [DIM] [DIM];
  num m2 [DIM] [DIM];
  num m3 [DIM] [DIM];
#else
  typedef exarray<num> vector;
  exarray <vector> m1, m2, m3;
#endif
  int i, j, k, dim = 0;

  // Input.

  ifstream fin  (argv [1]);
  if (!fin)
  {
     cerr << "Can't read file " << argv [1] << endl;
     return 255;
  }
  fin >> dec >> dim;

#ifdef DIM
  if (dim > DIM)
  {
     cerr << "Error: dimension > " << DIM << endl;
     return 255;
  }
#endif

  SetIdlePriority();

  cout << "Reading...\n";

  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      fin >> m1 [i] [j];

  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      fin >> m2 [i] [j];

  cout << "Computing...\n";

  long timer = GetTimer (0);
  for (i = 0; i < dim; i++)
  {
    for (j = 0; j < dim; j++)
    {
      num sum = 0;
      for (k = 0; k < dim; k++)
      {
        sum = sum + m1 [i] [k] * m2 [k] [j];
      }
      m3 [i] [j] = sum;
    }
  }
  timer = GetTimer (timer);

  cout << "Writing...\n";

  ofstream fout (argv [2]);

  if (!fout)
  {
     cerr << "Can't write file " << argv [2] << endl;
     return 255;
  }

  for (i = 0; i < dim; i++)
  {
    for (j = 0; j < dim; j++)
      { fout.width (30); fout << m3 [i] [j] <<" "; }
    fout << '\n';
  }

  if (!fout)
  {
     cerr << "Can't write file " << argv [2] << endl;
     return 255;
  }

  // Reporting.

  cout << "Dim=" << setw (4);
  cout << dim;
  cout << ", Size=" << setw (8);
#ifdef DIM
  cout << DIM * DIM * (sizeof(**m1) + sizeof(**m2) + sizeof(**m3));
#else
  cout << (size_t)((exalloc_status.pMaxAlloc - exalloc_status.pMinAlloc));
#endif
  cout << " for " << setw (8);
  cout << dim * dim * (sizeof(**m1) + sizeof(**m2) + sizeof(**m3));
  cout << ", Time=" << setw (6);
  cout << timer << " mls";
  cout << " for " << setw (10);
  unsigned long c = dim * dim * dim;
  cout << c; c /= 1000;
  cout << " of " << setw (4);
  if (c > 0) c = (timer * 1000 + (c / 2)) / c;
  cout << c <<" ns"<< endl;

  return 0;
}
