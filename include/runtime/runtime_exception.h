#ifndef RUNTIME_EXCEPTION_H
#define RUNTIME_EXCEPTION_H

#include <Common.h>

/* ================================ *
 *     global_destructor_chain.h
 * ================================ */

#ifdef __cplusplus
extern "C"
{
#endif

void __destroy_global_chain( void );

#ifdef __cplusplus
}
#endif

/* ================================ *
 *     Gecko_ExceptionPPC.h
 * ================================ */

#ifdef __cplusplus
extern "C"
{
#endif

// Forward declarations
typedef struct ExtabIndexInfo
{
    void *section;
    struct ExtabIndexInfo *extab;
    void *codeStart;
    size_t codeSize;
} ExtabIndexInfo;

int __register_fragment( const ExtabIndexInfo *extab, void *toc );
void __unregister_fragment( int i );

#ifdef __cplusplus
}
#endif

__DECL_SECTION( ".init" ) extern const ExtabIndexInfo _eti_init_info[];

#endif
