
// TODO(fox): Should the typedefs be used for long longs?

u64 add_64(u64 a, u64 b) {
    return a + b;
}

u64 aligned_64(u32 a, u64 b) {
    return b + 5;
}

u64 add_64(u64 a, u64 b) {
    return a + b;
}

u32 add_64_downcast(u64 a, u64 b) {
    return a + b;
}

u64 sub_64(u64 a, u64 b) {
    return a - b;
}

u32 sub_64_downcast(u64 a, u64 b) {
    return a - b;
}

u64 mul_64(u64 a, u64 b) {
    return a * b;
}

u32 mul_64_downcast(u64 a, u64 b) {
    return a * b;
}

u64 div_64(u64 a, u64 b) {
    return a / b;
}

u64 mod_64(u64 a, u64 b) {
    return a % b;
}

u64 shl_64(u64 a, u64 b) {
    return a << b;
}

u64 shr_64(u64 a, u64 b) {
    return a >> b;
}

u64 and_64(u64 a, u64 b) {
    return a & b;
}

u64 or_64(u64 a, u64 b) {
    return a | b;
}

u64 xor_64(u64 a, u64 b) {
    return a ^ b;
}
