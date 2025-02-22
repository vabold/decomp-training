/* ================================================================ *
 *
 **** 64-BIT INTEGERS
 *
 * Integers of type "long long," aka an integer that takes up 8 bytes,
 * or 64 bits, have to be implemented a bit specially since GPRs on 
 * the GC/Wii are only 32 bits long:
 *
 * ================================================================ */

/* ================================================================ *
 *
 * Register usage
 *
 * Each 64-bit value occupies two general-purpose registers. When
 * passed into a function, it simply occupies two continuous registers.
 * Additionally, r4 can be used in conjunction with r3 as a return value
 * to return a 64-bit value without needing to use the stack. 
 * 
 * ================================================================ */

u64 add_64(u64 a, u64 b) {
}

/* ================================================================ *
 *
 * The ABI requires the compiler to "align" the two GPRs belonging to
 * a 64-bit integer to an odd-numbered register (PPC 3-19), meaning
 * that in the example below, the register order looks like this:
 *
 * r3 - a
 * r4 - empty
 * r5, r6 - b
 *
 * Presumably this is to decrease register chain dependencies; if the
 * 64-bit argument was allowed to be placed in r4 and r5 instead, then
 * this example would compile to this and require an extra register
 * move to free up r4:
 * 
 * addi r6, r4, 0x0
 * li r0, 0x5
 * addc r4, r5, r0
 * li r0, 0x0
 * adde r3, r6, r0
 * blr
 * 
 * ================================================================ */

u64 aligned_64(u32 a, u64 b) {
}

/* ================================================================ *
 *
 **** DETERMINING 64-BIT USAGE
 *
 * The best way to confirm whether a given variable or struct/class
 * member is 64-bit or not is to inspect its operations, as most of
 * them are distinct from their 32-bit or less counterparts. 
 * 
 * An important optimization the compiler can do to watch out for is
 * in the case where the rvalue is 64-bit but the lvalue is 32-bit,
 * which I'll refer to as a "downcast." Look at the examples below to 
 * see what I mean:
 *
 * ================================================================ */

/* ================================================================ *
 *
 * Addition and subtraction
 *
 * Addition and subtraction use a pair of carry and extend instructions
 * to pass the carry bit to the higher-value register. Interstingly on
 * all versions of MWCC, the compiler will still emit the carry part of
 * the instruction instead of the normal variant when downcasting to 32-bit
 * even with optimizations, so this could theoretically be used to
 * identify a 64-bit value. ProDG emits the normal variant which causes
 * it to be indistinguishable like multiplication.
 * 
 * ================================================================ */

u64 add_64(u64 a, u64 b) {
}

u32 add_64_downcast(u64 a, u64 b) {
}

u64 sub_64(u64 a, u64 b) {
}

u32 sub_64_downcast(u64 a, u64 b) {
}

/* ================================================================ *
 *
 * Multiplication
 *
 * Multiplication has a long and recognizable 64-bit pattern, but a 
 * 32-bit downcast is indistinguishable from multiplying two 32-bit numbers.
 * 
 * ================================================================ */

u64 mul_64(u64 a, u64 b) {
}

u32 mul_64_downcast(u64 a, u64 b) {
}

/* ================================================================ *
 *
 * Compiler intrinsics
 *
 * Division, modulo, and bit shifts call compiler intrinsics on both 
 * MWCC and ProDG, which are unaffected by downcasts.
 * 
 * ================================================================ */

u64 div_64(u64 a, u64 b) {
}

u64 mod_64(u64 a, u64 b) {
}

u64 shl_64(u64 a, u64 b) {
}

u64 shr_64(u64 a, u64 b) {
}

/* ================================================================ *
 *
 * Boolean operations
 *
 * 64-bit AND, OR, and XOR are indistinguishable from two back-to-back 
 * 32-bit operations, so you can only know for sure that 64-bit is being
 * used if the inputs/results are passed into other known 64-bit
 * operations, like a comparison.
 * 
 * ================================================================ */

u64 and_64(u64 a, u64 b) {
}

u64 or_64(u64 a, u64 b) {
}

u64 xor_64(u64 a, u64 b) {
}

/* ================================================================ *
 *
 * Comparisons
 *
 * Both branched and branchless 64-bit comparisons are unambiguous
 * and are unaffected by downcasting, since the result of a comparison
 * is already a single bit (true/false).
 * 
 * ================================================================ */

// TODO(fox): implement
