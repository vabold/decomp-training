#include <runtime/runtime_core.h>
#include <runtime/runtime_exception.h>

int main( int argc, char **argv );

__DECL_SECTION( ".init" ) void *memset( void *s, int c, size_t n )
{
    u8 *p = (u8 *)s;
    u8 v = (u8)c;

    size_t i = 0;
    for( ; i < n; ++i )
    {
        p[ i ] = v;
    }

    return s;
}

__DECL_SECTION( ".init" ) void *memcpy( void *dst, const void *src, size_t n )
{
    u8 *d = (u8 *)dst;
    const u8 *s = (const u8 *)src;

    size_t i = 0;
    for( ; i < n; ++i )
    {
        d[ i ] = s[ i ];
    }

    return dst;
}

__DECL_SECTION( ".init" ) asm static void __init_registers( void )
{
    // clang-format off
    nofralloc

    li r0, 0x0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    li r11, 0x0
    li r12, 0x0
    li r14, 0x0
    li r15, 0x0
    li r16, 0x0
    li r17, 0x0
    li r18, 0x0
    li r19, 0x0
    li r20, 0x0
    li r21, 0x0
    li r22, 0x0
    li r23, 0x0
    li r24, 0x0
    li r25, 0x0
    li r26, 0x0
    li r27, 0x0
    li r28, 0x0
    li r29, 0x0
    li r30, 0x0
    li r31, 0x0

    lis r1,_stack_addr@h
    ori r1, r1, _stack_addr@l
    lis r2, _SDA2_BASE_@h
    ori r2, r2, _SDA2_BASE_@l
    lis r13, _SDA_BASE_@h
    ori r13, r13, _SDA_BASE_@l

    blr
    // clang-format on
}

__DECL_SECTION( ".init" ) static void __init_bss_section( void *dst, size_t n )
{
    if( n != 0 )
    {
        memset( dst, 0, n );
    }
}

__DECL_SECTION( ".init" ) static void __copy_rom_section( void *dst, const void *src, size_t n )
{
    if( n != 0 && dst != src )
    {
        memcpy( dst, src, n );
    }
}

__DECL_SECTION( ".init" ) static void __init_data( void )
{
    const __RomSection *rs;
    const __BssSection *bs;

    rs = _rom_copy_info;
    while( TRUE )
    {
        if( rs->size == 0 )
        {
            break;
        }

        __copy_rom_section( rs->virt, rs->phys, rs->size );
        ++rs;
    }

    bs = _bss_init_info;
    while( TRUE )
    {
        if( bs->size == 0 )
        {
            break;
        }

        __init_bss_section( bs->virt, bs->size );
        ++bs;
    }
}

#pragma section ".ctors$00"
__DECL_SECTION( ".ctors$00" ) extern funcptr_t _ctors[];
#pragma section ".dtors$00"
__DECL_SECTION( ".dtors$00" ) extern funcptr_t _dtors[];

__DECL_SECTION( ".init" ) void __init_cpp( void )
{
    funcptr_t *ctor;
    for( ctor = _ctors; *ctor; ++ctor )
    {
        ( *ctor )( );
    }
}

__DECL_SECTION( ".init" ) void __fini_cpp( void )
{
    funcptr_t *dtor;

    for( dtor = _dtors; *dtor; ++dtor )
    {
        ( *dtor )( );
    }
}

__DECL_SECTION( ".init" )
asm void PPCHalt( void ){
    // clang-format off
    nofralloc
    sync

loop:
    nop
    li r3, 0x0
    nop
    b loop
    // clang-format on
}

__DECL_SECTION( ".init" ) void exit( void )
{
    __fini_cpp( );
    PPCHalt( );
}

__DECL_SECTION( ".init" )
asm void __start( void )
{
    // clang-format off
    nofralloc

    bl __init_registers
    bl __init_data
    bl __init_cpp
    bl main
    b exit
    // clang-format on
}
