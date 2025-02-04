#ifndef RUNTIME_CORE_H
#define RUNTIME_CORE_H

#include <Common.h>

void *memset( void *s, int c, size_t n );
void *memcpy( void *dst, const void *src, size_t n );

__DECL_SECTION( ".init" ) extern u8 _stack_addr[];
__DECL_SECTION( ".init" ) extern u8 _SDA_BASE_[];
__DECL_SECTION( ".init" ) extern u8 _SDA2_BASE_[];

typedef struct __RomSection
{
    void *phys;
    void *virt;
    size_t size;
} __RomSection;

typedef struct __BssSection
{
    void *virt;
    size_t size;
} __BssSection;

__DECL_SECTION( ".init" ) extern __RomSection _rom_copy_info[];
__DECL_SECTION( ".init" ) extern __BssSection _bss_init_info[];

#endif
