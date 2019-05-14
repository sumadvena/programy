/* --------------------------------------------------------------
    Signed integers with unlimited range (version 2.1b).
    Macro parameters for base functions.

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
#ifndef _CBIGNUMF_H
#define _CBIGNUMF_H
#include "Cbnl.h"

#ifndef _CBIGNUMF_INL
#if 1
#define _CBIGNUM_ASM
#endif
#endif

#ifdef  _CBIGNUM_ASM

#ifdef  _CBNL_CBR
#if 1
#define _CBIGNUM_HARDWARE_CBR
#endif
#endif

#endif

#ifdef  _CBNL_MUL
#if 1
#define _CBIGNUM_HARDWARE_MUL
#endif
#endif

#if 0
#define _CBIGNUM_HARDWARE_DIV
#endif

#if 1
#define _CBIGNUM_KARATSUBA_MUL
#endif

#if 1
#define _CBIGNUM_BLOCK_MUL
#endif

#if 1
#define _CBIGNUM_TERNARY_MUL
#endif

#if 1
#define _CBIGNUM_SHIFTTAB_MUL
#endif

#if 1
#define _CBIGNUM_SHIFTTAB_DIV
#endif

#if 1
#define _CBIGNUM_SMALL_DIV 2
#endif

#if 1
#define _CBIGNUM_SMALL_POWMOD
#endif

#if 0
#define _CBIGNUM_REVERSE_MOD
#endif

#if 0
#define _CBIGNUM_REDUCE_JUMPS
#endif

#define _CBNL_TAB_MIN   (3)
#define _CBNL_TAB_OPT   (((sizeof(size_t)<=2)? 120: 7680)/\
                         (sizeof (CBNL) * sizeof (CBNL)) - 3)
#define _CBNL_TAB_HIGH  (_CBNL_TAB_OPT + _CBNL_TAB_OPT / 2)
#define _CBNL_TAB_MAX   ((EXSIZE_T_MAX & (size_t)0x7FFFFFFFUL)/\
                         (1024 * sizeof (CBNL)))

#ifndef _CBIGNUM_HARDWARE_MUL
#define _CBNL_KARATSUBA_MIN     (100)
#else
#define _CBNL_KARATSUBA_MIN     (50)
#define _CBNL_MUL_OPT   (7680 / sizeof (CBNL) - 3)
#define _CBNL_MUL_HIGH  (_CBNL_MUL_OPT + _CBNL_MUL_OPT / 2)
#endif

#define _CBNL_HARDDIV_BITS 10

#endif
