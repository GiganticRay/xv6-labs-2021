# Initial process that execs /init.
# This code runs in user space.

#include "syscall.h"

# exec(init, argv)
# SYS_exec is matched into entries defiend in syscalls array, a table of function pointers (kernel/syscall.c)
# the ecall traps into the kernel and causes uservec, usertrap, it is this instruction invoke (syscall, syscall.c)?
# the ecall switch the CPU from user-mode to supervisor-mode and enters the kernel at an entry point specified by the kernel which is located in syscall.c
.globl start
start:
        la a0, init
        la a1, argv
        li a7, SYS_exec
        ecall

# for(;;) exit();
exit:
        li a7, SYS_exit
        ecall
        jal exit

# char init[] = "/init\0";
init:
  .string "/init\0"

# char *argv[] = { init, 0 };
.p2align 2
argv:
  .long init
  .long 0
