xv6 is a re-implementation of Dennis Ritchie's and Ken Thompson's Unix
Version 6 (v6).  xv6 loosely follows the structure and style of v6,
but is implemented for a modern RISC-V multiprocessor using ANSI C.

ACKNOWLEDGMENTS

xv6 is inspired by John Lions's Commentary on UNIX 6th Edition (Peer
to Peer Communications; ISBN: 1-57398-013-7; 1st edition (June 14,
2000)). See also https://pdos.csail.mit.edu/6.828/, which
provides pointers to on-line resources for v6.

The following people have made contributions: Russ Cox (context switching,
locking), Cliff Frey (MP), Xiao Yu (MP), Nickolai Zeldovich, and Austin
Clements.

We are also grateful for the bug reports and patches contributed by
Takahiro Aoyagi, Silas Boyd-Wickizer, Anton Burtsev, Ian Chen, Dan
Cross, Cody Cutler, Mike CAT, Tej Chajed, Asami Doi, eyalz800, Nelson
Elhage, Saar Ettinger, Alice Ferrazzi, Nathaniel Filardo, flespark,
Peter Froehlich, Yakir Goaron,Shivam Handa, Matt Harvey, Bryan Henry,
jaichenhengjie, Jim Huang, Matúš Jókay, Alexander Kapshuk, Anders
Kaseorg, kehao95, Wolfgang Keller, Jungwoo Kim, Jonathan Kimmitt,
Eddie Kohler, Vadim Kolontsov , Austin Liew, l0stman, Pavan
Maddamsetti, Imbar Marinescu, Yandong Mao, , Matan Shabtay, Hitoshi
Mitake, Carmi Merimovich, Mark Morrissey, mtasm, Joel Nider,
OptimisticSide, Greg Price, Jude Rich, Ayan Shafqat, Eldar Sehayek,
Yongming Shen, Fumiya Shigemitsu, Cam Tenny, tyfkda, Warren Toomey,
Stephen Tu, Rafael Ubal, Amane Uehara, Pablo Ventura, Xi Wang, Keiichi
Watanabe, Nicolas Wolovick, wxdao, Grant Wu, Jindong Zhang, Icenowy
Zheng, ZhUyU1997, and Zou Chang Wei.

The code in the files that constitute xv6 is
Copyright 2006-2020 Frans Kaashoek, Robert Morris, and Russ Cox.

ERROR REPORTS

Please send errors and suggestions to Frans Kaashoek and Robert Morris
(kaashoek,rtm@mit.edu). The main purpose of xv6 is as a teaching
operating system for MIT's 6.S081, so we are more interested in
simplifications and clarifications than new features.

BUILDING AND RUNNING XV6

You will need a RISC-V "newlib" tool chain from
https://github.com/riscv/riscv-gnu-toolchain, and qemu compiled for
riscv64-softmmu. Once they are installed, and in your shell
search path, you can run "make qemu".

# Experiment 2
## ex 2.1 System call tracing
> requirement: trace [tracing_mask] [command] ，要求当 command 调用了给定的 `tracing_mask`对应的 `sys_call` 的时候，打印输出调用该 `sys_call`的进程`PID`, system call的`名称`、system call的`返回值`

### file desctiption
1. `user/user.h`: 声明了 `sys call func`, `user lib func` 的函数接口
2. `user/usys.pl`: perl script，被 makefile 调用生成 `usys.S`, 里面存放的内容是 stub of function
3. `kernel/syscall.h`: store system call number
4. `kernel/proc.h`: 定义了进程的属性，包括 trapframe.
5. `kernel.syscall.c`: 系统调用的入口，需要申明 系统调用其他函数的函数指针，里面有个 `syscall` 函数，由该函数调用 `syscalls` 函数，来执行系统调用。
6. `kernel/sysproc.c`: 此处实现的 `sys_call`,对于常规的系统调用而言，就是 sys_call 就是读取参数，放入寄存器，然后调用在 `kernel/proc.c` 中实现的系统调用函数。该实验中，`sys_trace`并不需要再往下调用什么函数，所以在这一层赋值 `mask_num`就可以了。在这一层，做的事情是，将用户进程调用系统调用的参数，写入寄存器里面。
7. `kernel/proc.c`: 更底层的系统调用, 直接与寄存器进行交互。

### 用户进程的系统调用过程
1. 用户态下，（汇编代码）在`寄存器`中设置`系统调用的参数`、`调用代码`
2. 用户态下，(汇编代码) 调用 `_entry` instr, 抵达系统调用的`入口`(`kernel/syscal.c`)。（ 从高级`c`语言来看，以 trace 举例），其直接调用 `trace` 系统调用，传入参数。实际上来说，这段高级语言还是要转换成汇编，做的事情符合上面的描述。汇编调用`_entry`之后，程序跑到 `syscall.c`, 找到 `func num` 与 `sys_trace` 函数指针的对应，然后调用 `sys_trace` 将该系统调用的参数写入 寄存器之中。
3. `syscall.c` 的 `syscall` 从 `a7` 寄存器获取要调用系统调用的 num, 然后执行对应的 `sys_call`。
4. `sys_call` 将该系统调用的参数，写入寄存器，一般来说再次调用对应的 `call`，位于`kernel/proc.c`
5. `call` 直接与寄存器进行交互，实现目的.
6. `sys_call` 的返回值，在 `syscall.c` 中，写入 `a0` 寄存器。
7. 如何返回用户态？这一点暂时还没看到