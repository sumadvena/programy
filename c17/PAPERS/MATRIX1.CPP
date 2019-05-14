/*	matrix1.cpp

	Перемножение матриц с использованием статических массивов.

	(C) Р.Н.Шакиров, ИМАШ УрО PAH, 1998 - 2000
	All Rights Reserved.
*/
#include <fstream.h>
#define  DIM 500

int	main	(int argc, char **argv)
{
  if (argc < 3) return 1;
  static int m1 [DIM] [DIM];
  static int m2 [DIM] [DIM];
  static int m3 [DIM] [DIM];
  int i,j,k,dim = 0;

  // Ввод исходных данных

  ifstream fin  (argv [1]);
  if (!fin) return 1;
  fin >> dim;

  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      fin >> m1 [i] [j];

  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      fin >> m2 [i] [j];

  // Перемножение матриц
  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
    {
      int sum = 0;
      for (k = 0; k < dim; k++)
        sum += m1 [i] [k] * m2 [k] [j];
      m3 [i] [j] = sum;
    }

  // Вывод результата 

  ofstream fout (argv [2]);
  if (!fout) return 1;
  for (i = 0; i < dim; i++)
  {
    for (j = 0; j < dim; j++)
      { fout.width (6); fout << m3 [i] [j]; }
    fout << '\n';
  }

  return 0;
}
