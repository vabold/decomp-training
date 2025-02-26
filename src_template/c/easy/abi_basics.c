/* ================================================================ *
 *
 **** ABI BASICS 
 *
 * In order for a piece of software to be able to run on
 * a given piece of hardware and/or interoperate with other pieces of
 * software on a system, there has to be a general agreement
 * as to how the software will interact with various aspects of 
 * the hardware. This is known as an *application binary interface*, or
 * ABI. For example, every library and function on the Gamecube/Wii
 * "agrees" that it will use a specific register to return values if
 * the function returns something, which you'll see in the first training
 * function. 
 *
 * An ABI close to the what the GC/Wii uses that is useful to 
 * reference is the System V PowerPC ABI, which can be found here:
 *
 * http://refspecs.linux-foundation.org/elf/elfspec_ppc.pdf
 *
 * In addition, since the GC/Wii is an embedded system, its CPU
 * also obeys a superset of the ABI known as an *embedded* application
 * binary interface*, or EABI, which can be viewed here:
 *
 * https://files.decomp.dev/E500ABIUG.pdf
 *
 * References to the general ABI will use the notation (ABI page #-#), 
 * and the EABI will use (EABI page #-#), 
 *
 * ================================================================ */

/* ================================================================ *
 *
 * Registers and parameters (Function Calling Sequence, ABI page 3-14)
 *
 * As you may be familiar with, the CPU has to move any data it wants
 * to operate on from main memory (RAM) into its own internal registers 
 * to be able to access it. Thus, an optimization most ABIs utilize
 * to reduce the amount of times a given function has to access RAM is
 * to allow registers themselves to be used as both function arguments
 * and return values. 
 *
 * In the case of non-float values, up to eight general-purpose registers 
 * can used to pass arguments to a function, starting at GPR3 and ending
 * at GPR10. Additionally, GPR3 acts a return register that a callsite of
 * the function can read from. 
 *
 * For example, to write a function add() that adds two inputted integers
 * together and returns the result, you simply have to add r3 and r4 
 * and store the result in r3.
 * 
 * ================================================================ */

int abi_function_1(int a, int b) {
}

/* ================================================================ *
 * 
 * For floats, up to eight floating-point registers can be used,
 * starting at FPR1 and ending at FPR8. FPR1 also acts as the return
 * register. 
 *
 * ================================================================ */

int abi_function_2(int a, int b) {
}

/* ================================================================ *
 *
 * Volatile and non-volatile registers (Registers, ABI page 3-14)
 * 
 * The sets of registers used for function passing (r3-r8, f1-f8) are
 * also known as *volatile* registers, because whenever you branch into
 * a new function, the ABI allows the values in those registers to get
 * overwritten. In other words, any time you step over a "b some_func", 
 * you must assume that all volatile registers have effectively been
 * destroyed (or modified in the case of return registers). Thus 
 * the concept of "non-volatile" registers become useful, which allow
 * us to preserve data between registers that cross callsites without
 * having to read/write from main memory.
 *
 * The simplest example of this behavior can be seen below, where
 * r3 is moved to the non-volatile register r31, since r3 can get
 * overwritten by some_func, which is then passed back to r3 to 
 * be used as the return register. The other instructions, which are
 * related to the stack, will be explained next.
 *
 * ================================================================ */

int abi_function_3(int a) {
}

/* ================================================================ *
 *
 * Function prologues/epilogues, the stack, and the link register
 * (Function Prologue and Epilogue, ABI page 3-34)
 * (The Stack Frame, ABI page 3-17)
 * 
 * You may have noticed a pattern among the functions listed so far in
 * that they all end in the "blr" opcode, and many of them contain a
 * "mflr" towards the top and a "mtlr" towards the bottom. These
 * opcodes all concern a special register called the *link register*,
 * which holds the memory address of the previous function that called
 * the function you're currently in. To explain it briefly:

 * TODO(fox): the final file could have these be real addresses
 *
 * | // some_func
 * | 0x80103F14 | add r3, r3, 0x5
 * | 0x80103F18 | blr
 *
 * | // a bunch of code
 * | 
 * | // abi_function_4
 * | 0x802D8910 | mflr r0
 * | 0x802D8914 | stw r0, 0x4(r1)
 * | 0x802D8918 | stwu r1, -0x8(r1)
 * | 0x802D891C | bl 0x80103F14 // address of some_func
 * | 0x802D8920 | lwz r0, 0xc(r1)
 * | 0x802D8924 | addi r1, r1, 0x8
 * | 0x802D8918 | mtlr r0
 * | 0x802D891C | blr
 *
 * When the "bl 0x80103F14" instruction gets executed, the link
 * register (LR) is automatically set with the address of the next
 * instruction at 0x802D8920, which is "lwz r0, 0xc(r1)". Then once it
 * hits the blr in "some_func" at 0x80103F18, it branches to the value
 * at the LR (0x802D8920) and resumes where it left off in
 * "abi_function_4."
 *
 * However you may be wondering, if the LR gets overwritten by the "bl
 * 0x80103F14" in "abi_function_4," then what will happen to the LR
 * that "abi_function_4" is currently holding? It won't be able to
 * return to the function that's calling itself when it executes its
 * "blr" if its LR gets overwritten! Luckily, that problem is exactly
 * what all of the other instructions are addressing, which are a part
 * of what is known as the "prologue" and "epilogue" that are executed
 * at the beginning and end of a function respectively. 

 * In the prologue, "mflr" moves the address in the LR to a register
 * r0, which is then stored in what is known as the "stack" in the
 * "stw". Note that r1 is a special register which holds the stack
 * pointer, and its current value is required to be decremented and
 * placed on the stack (in the "stwu"), which you can read more about
 * on 3-34 of the ABI doc. Then in the epilogue, that address is
 * loaded out from the stack back into r0 and moved back into the LR
 * in the "lwz" and "mtlr" instructions, which allows the "blr" to
 * successfully return to "abi_function_4"'s caller. 
 *
 * The reason "mflr" and "mtlr" don't show up in every function, as
 * you may be able to guess, is that a function doesn't need to save
 * and restore the LR unless it actually needs to (i.e. it calls a
 * function), which is why some_func doesn't have them. 
 *
 * ================================================================ */

void abi_function_4(float a) {
}

// TODO(fox): This should probably be combined with the branching
// section. This is a more complicated example than using a single
// conditional, but I think the multiple branches illustrates the
// point better.

/* ================================================================ *
 *
 * Typically when a function has early returns, such as the cases in
 * this switch statement in abi_function_6, they will branch to the
 * epilogue. However if a function doesn't use the stack, the epilogue
 * effectively turns into a single blr instruction. In this case, the
 * compiler can perform an optimization where it replaces those
 * epilogue branches with the epilogue itself and create multiple blrs
 * in a single function.
 *
 * ================================================================ */

void abi_function_6(int a) {
}

void abi_function_7(int a) {
}

/* ================================================================ *
 *
 * Besides saving/restoring the LR in the prologue/epilogue, the stack
 * will also get used by normal code for various reasons when
 * registers aren't enough to represent the data. Normally when you
 * declare variables on "the stack" in C, the compiler will try to
 * avoid actually implementing a stack and simply use registers.
 * However one case where the compiler won't do that is if you declare
 * a struct on the stack and you pass its address to a function, in
 * which case it always implements a real stack. Typically you'll see
 * this with math-related structs like Vec3 or Vec4 or Mtx.
 *
 * ================================================================ */

void abi_function_5(float a) {
}

