/* --------------------------------------------------------------
    Macro for check of bounds in the debug mode (version 1.2c).

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
#ifndef EXDEBUG_H
#define EXDEBUG_H

#ifndef EXARRAY_H
#include "Exarray.h"
#endif

#ifndef NDEBUG
#define EXDEBUG(a)  ((void)(a))
#else
#define EXDEBUG(a)  ((void)0)
#endif

#ifdef  NCHECKPTR

#define EXARR(T,m,n)    T m[(n)+1]
#define EXPTR(T)        T*
#define EXPTRTO(T,m,n)  (m)
#define EXPTRTYPE(m)    ((m).base())
#define EXPTRBASE(p)    (p)
#define EXPTRCHECK(p,n) (p)
#define EXPTRINDEX(p,i) (p)
#define EXPTRNEW(T,n)   (EXNEW(T,(n)))
#define EXPTRDELETE(p)  (EXDELETE(p))
#define EXPTRALLOC(T,n) (EXALLOC(T,(n)))
#define EXPTRFREE(p)    (EXFREE(p))
#define EXPTRALIGNED_ALLOC(T,n,a)   (EXALIGNED_ALLOC(T,(n),(a)))
#define EXPTRALIGNED_FREE(p)        (EXALIGNED_FREE(p))

#else

#define EXARR(T,m,n)    T m##___ [(n)+1];\
                        exptr<T>m = exptr<T>(m##___,\
                                      sizeof(m##___)/sizeof(T)-1,0)
#define EXPTR(T)        exptr<T>
#define EXPTRTO(T,m,n)  (exptr<T>((m),(n)))
#define EXPTRTYPE(m)    (m)
#define EXPTRBASE(p)    ((p).base())
#define EXPTRCHECK(p,n) ((p).check(n))
#define EXPTRINDEX(p,i) ((p).index(i))
#define EXPTRNEW(T,n)   (exptr<T>().allocate(n))
#define EXPTRDELETE(p)  ((p).deallocate())
#define EXPTRALLOC(T,n) (exptr<T>().alloc(n))
#define EXPTRFREE(p)    ((p).free())
#define EXPTRALIGNED_ALLOC(T,n,a)   (exptr<T>().aligned_alloc(n,a))
#define EXPTRALIGNED_FREE(p)        ((p).aligned_free())

#endif

#endif
