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
 * Function prologues/epilogues and the Link Register
 * (Function Prologue and Epilogue, ABI page 3-34)
 * 
 * TODO(fox): write this
 *
 * ================================================================ */

void abi_function_4(float a) {
}

/* ================================================================ *
 *
 * The stack (The Stack Frame, ABI page 3-17)
 * 
 * TODO(fox): write this
 *
 * ================================================================ */

void abi_function_5(float a) {
}
