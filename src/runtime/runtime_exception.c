#include <runtime/runtime_exception.h>

/* ================================ *
 *     global_destructor_chain.c
 * ================================ */

typedef void ( *DtorFunc )( void *obj, s16 method );

typedef struct DtorLink
{
    struct DtorLink *next;
    DtorFunc dtor;
    void *obj;
} DtorLink;

DtorLink *__global_destructor_chain = NULL;

void __register_global_object( void *obj, DtorFunc dtor, DtorLink *link )
{
    link->next = __global_destructor_chain;
    link->dtor = dtor;
    link->obj = obj;
    __global_destructor_chain = link;
}

void __destroy_global_chain( void )
{
    DtorLink *link;

    while( link = __global_destructor_chain )
    {
        __global_destructor_chain = link->next;
        link->dtor( link->obj, -1 );
    }
}

#pragma section ".dtors$10"
__DECL_SECTION( ".dtors$10" )
__DECL_WEAK const funcptr_t __destroy_global_chain_reference = __destroy_global_chain;

/* ================================ *
 *     Gecko_ExceptionPPC.c
 * ================================ */

#define NUM_FRAGMENT 1

typedef struct FragmentInfo
{
    const ExtabIndexInfo *eti;
    void *toc;
    BOOL regist;
} FragmentInfo;

static FragmentInfo fragmentInfo[ NUM_FRAGMENT ];

int __register_fragment( const ExtabIndexInfo *eti, void *toc )
{
    int i;
    FragmentInfo *frag;

    for( i = 0; i < NUM_FRAGMENT; ++i )
    {
        frag = &fragmentInfo[ i ];
        if( !frag->regist )
        {
            frag->eti = eti;
            frag->toc = toc;
            frag->regist = TRUE;
            return 0;
        }
    }

    return -1;
}

void __unregister_fragment( int i )
{
    FragmentInfo *frag;
    if( i < 0 || i >= NUM_FRAGMENT )
    {
        return;
    }

    frag = &fragmentInfo[ i ];
    frag->eti = NULL;
    frag->toc = NULL;
    frag->regist = FALSE;
}

/* ================================ *
 *     __init_cpp_exceptions.cpp
 * ================================ */

static int fragmentID = -2;

#ifdef __cplusplus
extern "C"
{
#endif
void __init_cpp_exceptions( void );
void __fini_cpp_exceptions( void );
#ifdef __cplusplus
}
#endif

static void *GetTOC( void )
{
    register void *toc;

    // clang-format off
    asm
    {
        mr toc, r2
    };
    // clang-format on

    return toc;
}

void __init_cpp_exceptions( void )
{
    if( fragmentID == -2 )
    {
        fragmentID = __register_fragment( _eti_init_info, GetTOC( ) );
    }
}

void __fini_cpp_exceptions( void )
{
    if( fragmentID != -2 )
    {
        __unregister_fragment( fragmentID );
        fragmentID = -2;
    }
}

#pragma section ".ctors$10"
__DECL_SECTION( ".ctors$10" )
const funcptr_t __init_cpp_exceptions_reference = __init_cpp_exceptions;

#pragma section ".dtors$15"
__DECL_SECTION( ".dtors$15" )
const funcptr_t __fini_cpp_exceptions_reference = __fini_cpp_exceptions;
