#ifndef COMMON_H
#define COMMON_H

/* ================================ *
 *     C DEFINITIONS
 * ================================ */

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;

typedef float f32;
typedef float f64;

typedef u32 size_t;
typedef u32 uintptr_t;
typedef void ( *funcptr_t )( void );

typedef int BOOL;
enum
{
    FALSE,
    TRUE
};

#define NULL 0

#define __DECL_SECTION( x ) __declspec( section x )
#define __DECL_WEAK __declspec( weak )

/* ================================ *
 *     C++ DEFINITIONS
 * ================================ */

#ifdef __cplusplus
extern "C"
{
#endif

#define static_assert( cond ) __static_assert( cond, #cond )

#ifdef __cplusplus
}
#endif

#endif
