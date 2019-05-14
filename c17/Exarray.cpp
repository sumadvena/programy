/* --------------------------------------------------------------
    Dynamic arrays with check of bounds (version 1.2c).
    Memory allocation functions.

    http://www.imach.uran.ru/exarray

    Copyright 1998-2010 by Dr. Raul N.Shakirov, IMach of RAS(UB).
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
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#define EXALLOC_OUT_OF_LINE
#include "Exarray.h"

#ifdef  _MSC_VER
#pragma auto_inline (off)
#endif

#ifndef _mm_malloc
#define _aligned_malloc(p,b,a)  malloc(b)
#define _aligned_realloc(p,b,a) realloc(p,b)
#define _aligned_free(p)        free(p)
#endif

#ifdef  __cplusplus
extern "C" {
#endif

struct  exblock_t VDECL exnull_c;

void (* set_exalloc_handler (void (*pvf) (void))) (void)
{
    static void (* pvfFailHandler)(void) = NULL;
    void (*pvfPrev) (void) = pvfFailHandler;
    pvfFailHandler = pvf;
    return pvfPrev;
}

void exalloc_size_fail (void)
{
    for (;;)
    {
        void (*pvf) (void) = set_exalloc_handler (NULL);
        set_exalloc_handler (pvf);
        if (pvf == NULL) break;
        (*pvf)();
    }
    abort();
}

void exalloc_stub (void) {}

static void exalloc_null_fail (void)
{
    #define NULL_POINTER_INDIRECTION 0
    assert (NULL_POINTER_INDIRECTION);
    abort();
}

static void exalloc_range_fail (void)
{
    #define INDEX_RANGE_ERROR 0
    assert (INDEX_RANGE_ERROR);
    abort();
}

struct exalloc_status_t VDECL exalloc_status =
    { exalloc_size_fail, exalloc_stub,
      exalloc_null_fail, exalloc_range_fail,
      0, 0, 0, 0 };

void*   FCDECL  exmalloc (size_t blocksize)
{
    void* p = NULL;
    exaligned_mrealloc (&p, blocksize, 1, 0, blocksize);
    return (p);
}

void*   FCDECL  exaligned_malloc (size_t blocksize, size_t blockalign)
{
    void* p = NULL;
    exaligned_mrealloc (&p, blocksize, blockalign, 0, blocksize);
    return (p);
}

void    FCDECL  exmrealloc (void** p, size_t blocksize,
                 size_t memset_start, size_t memset_stop)
{
    exaligned_mrealloc (p, blocksize, 1, memset_start, memset_stop);
}

void    FCDECL  exaligned_mrealloc (void** p,
                 size_t blocksize, size_t blockalign,
                 size_t memset_start, size_t memset_stop)
{
    void* pp = *p;

    if (blocksize)
    {
        while (blocksize > EXCALCBLOCKSIZE_MAX - blockalign ||
              (pp = _aligned_realloc (*p, blocksize, blockalign))
              == NULL)
        {
            void (*pvf) (void) = set_exalloc_handler (NULL);
            set_exalloc_handler (pvf);
            if (pvf != NULL) { (*pvf)(); continue; }

            (*exalloc_status.size_handler)();
            exalloc_status.nBlocksFailed++;
            return;
        }

        if (((size_t)pp & (blockalign - 1)) != 0) {}

        if (pp != NULL)
        {
            exalloc_status.nBlocksAllocated++;
        }

        if (memset_stop > memset_start)
        {
             memset ((char*)pp + memset_start, 0,
                   memset_stop - memset_start);
        }

        if (exalloc_status.pMinAlloc - 1 >= (char*) pp)
        {
            exalloc_status.pMinAlloc = (char*)pp;
        }
        if (exalloc_status.pMaxAlloc < (char*)pp + blocksize)
        {
            exalloc_status.pMaxAlloc = (char*)pp + blocksize;
        }
    }

    else if (pp)
    {
        _aligned_free (pp); pp = NULL;
        exalloc_status.nBlocksAllocated--;
    }

    *p = pp;
}

void    FCDECL  exfree (void* p)
{
    exaligned_free (p);
}

void    FCDECL  exaligned_free (void* p)
{
    if (p)
    {
        _aligned_free (p);
        exalloc_status.nBlocksAllocated--;
    }
}

void    FCDECL  exrange (void* p)
{
    if (p == NULL)
            (*exalloc_status.null_handler)();
    else    (*exalloc_status.range_handler)();
}

size_t  FCDECL  excalcblocksize (size_t blocksize)
{
    size_t n = EXCALCBLOCKSIZE_MOD, k = 0;

    for (;; k = ~k, (k? (n <<= 1): (n += (n >> 1))))
    {
        n -= EXCALCBLOCKSIZE_SYS; if (n >= blocksize) break;
        n += EXCALCBLOCKSIZE_SYS; if ((ptrdiff_t) n < 0)
                               { n = EXSIZE_T_MAX; break; }
    }

    return (n);
}

size_t  FCDECL  exmuladd (size_t s, size_t n, size_t k)
{
    return ((n <= (EXSIZE_T_MAX - k) / s)? (n * s + k): EXSIZE_T_MAX);
}

size_t  FCDECL  exmul (size_t s, size_t n)
{
    return ((s == 1)? n: (n <= EXSIZE_T_MAX / s)? n * s: EXSIZE_T_MAX);
}

size_t  FCDECL  exadd (size_t n, size_t k)
{
    return ((n <= EXSIZE_T_MAX - k)? (n + k): EXSIZE_T_MAX);
}

size_t  FCDECL  exchkadd (size_t n, size_t k, size_t max)
{
    if (n > max - k) (*exalloc_status.size_handler)();
    return (n + k);
}

#ifdef  __cplusplus
}
#endif
