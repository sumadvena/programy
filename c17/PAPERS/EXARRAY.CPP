/*	exarray.cpp

	Динамические массивы и указатели с
	автоматической проверкой индекса
	(сокращенный вариант).
	Функции распределения памяти.

	(C) Р.Н.Шакиров, ИМАШ УрО PAH, 1998 - 2000
	All Rights Reserved.
*/
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <new.h>

//	Функция exmrealloc выделяет, удлиняет,
//	укорачивает и освобождает блоки памяти,
//	заполненные нулями.
//	При переполнении разрядной сетки индекса
//	вызывается функция abort().
//	При нехватке памяти вызывается функция,
//	установленная по set_new_handler из
//	new.h, а если ее нет - то abort().

void	exmrealloc (void **p, unsigned size,
	 		      unsigned oldsize)
{
  void *pp = *p;
  if (size)		// Выделение памяти.
  {
    if (size > (~0u)-9)	// Обход ошибки realloc().
      abort();
    while ((pp = realloc (*p, size)) == NULL)
    {			// Вызов new_handler().
      typedef void (*pvfunc) (void);
      pvfunc pvf = set_new_handler (NULL);
      set_new_handler (pvf);
      if (pvf) (*pvf)(); else abort();
    }
    if (size > oldsize)
      memset           	// Обнуление.
       ((char*)pp + oldsize, 0, size - oldsize);
  }
  else			// Оcвобождение памяти.
  { 
    if (pp) { free (pp); pp = NULL; }
  }
  *p = pp;
}

//	Функция excalcsize вычисляет размер блока
//	памяти в байтах, который должен быть не
//	меньше требуемого, для чего начальный
//	размер SIZE_MOD умножается на 2 и 1.5.
//	При переполнении выдается ~0u.
//	Для уменьшения фрагментации динамической
//	памяти учитывается размер системных данных
//	SIZE_SYS, добавляемых функцией realloc в
//	Borland C++ 3.1, 4.5.
//	Для оптимизации работы кэша L1 процесcоров
//	Pentuim SIZE_MOD задается как 64**n +/- 16.

#define SIZE_MOD (112)
#define SIZE_SYS (sizeof(int) * 2)

unsigned excalcsize (unsigned size)
{
  unsigned n = SIZE_MOD, k = 0;
  for (;; k = ~k, (k? (n <<= 1): (n += (n >> 1))))
  {
    n -= SIZE_SYS; if (n  >= size) break;
    n += SIZE_SYS; if ((int)n < 0) {n =~0u; break;}
  }
  return (n);
}

//	Объект - константа, содержащий
//	нулевой указатель и нулевое значение.

class	{ void* e; unsigned len; } __EXNULL;
