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

# Experienment

## ex1.1: sleep
1. command line parameter, **argc** is argument count. By default **argc** is 1 and *argv[0]** is filename.
2. using fprintf instead of printf to put output into specified output stream.
3. exit(1) insetead of exit(-1) since exit(1) indicate abnormal termination of the program.
4. self-implemented file is located in `/user/`, system call is implemented in `/user/usys.S` using **assembly language**. user call is implemented in `/user/*.c`. `/user/user.h` provide the interface of both of them.


## ex1.2: pingpong
### prerequisite
1. pipe() to initilize a pipe.
### STEPS
1. 仅用 1 个 pipe，主进程先往 `pipe[1]`(写端)写入 1 字节
2. `fork` 后，子进程判断 `read from pipe[0]` 是否成功，如果成功的话，管道中的值已经被读出了，为空（这里主进程**保证**仅向管道内写入 `1` 个字节的数据, 子进程再向 `pipe[1]` 内写入一个字节
3. 主进程 `wait for the end of sub process`. 如果成功，则说明子进程成功写入 `1` 字节元素进入管道，主进程读取该字节（同上需要子进程做出承诺).


## ex1.3: primes 
### data flow
> $main[2,35] \Rightarrow p[0] \rightarrow p[1] \Rightarrow subP1[3,5,7,\cdots, 35] \Rightarrow p1[0] \rightarrow p1[1] \Rightarrow subP2[5,7,11,\cdots,35]\cdots$ 

### Thought
1. 可不要小瞧了递归的重要性，虽然 leetcode 的树遍历都是要求将递归展开，不过这种天然递归的时候，用递归更容易理清代码逻辑
```c++
	subprimes(pare_pipe){
		pipe(sub_pipe)
		if(get an int(base) from pipe[READEND]){
			fork a subprocess to provoke subprimes(sub_pipe) recursively.
			transfer i which i%base != 0 into sub_pipe.

		}
	}
```
2. doubting about hint of homework description.
> Hint: read `returns zero` when the `write-side` of a pipe is `closed`.

> Q: `pipe` 的 `write-side` 一关闭，return 就立马返回 0 吗？

> Q: `pipe` 是在 `fork()` 之前建立的，那么 子进程保留的是 pipe 的拷贝(虽然同一个 `fd` 共享 `offset`)。那么父进程里面的 `写端` 关闭了，对子进程的 `读` 有什么影响呢？

> A: 不是的，关闭 `pipe` 的一端，相当于给这个 `pipe` 加上了一个文件末尾标识符, 就算是拷贝的，但是最终指向的，还是同一个 `pipe`，所以子进程在 读取 `pipe` 的时候，最终会返回 `0`，表示读取到 `pipe` 的末尾了。
