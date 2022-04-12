// Copyright: 	LeiChao 
// Author: 		Leichao
// Github:		https://github.com/GiganticRay
// Date:		2022/04/11

// copy.c: copy input to output
// adding function into xv6: https://www.geeksforgeeks.org/xv6-operating-system-add-a-user-program/
#include "kernel/types.h"
#include "user/user.h"

#define PRIMEBOUND 35
#define WE 1	// write end also the output end
#define RE 0	// read end also the input end

void subPrimes(int* pipe_fds);

int
main(int argc, char* argv[]){
	// only cover the range[2, 25]
	int pipe_fds[2];
	pipe(pipe_fds);

	if(fork() == 0){
		// 此处是有个递归，可以考虑下展开递归
		subPrimes(pipe_fds);
	}
	else{
		close(pipe_fds[RE]);
		int i;
		// feed [2, PRIMEBOUND] into the pipe
		for(i = 2; i <= PRIMEBOUND; i++){
			write(pipe_fds[WE], &i, sizeof(int));
		}
		close(pipe_fds[WE]);	// guess: READ in subprogram should not return 0 supposedly since fork will create a duplicate copy bak of this pipe. So this line is safe.
		wait((int*) 0);
	}
	exit(0);
}

void subPrimes(int* pipe_fds){
	int curr_pipe_fds[2];
	pipe(curr_pipe_fds);

	close(pipe_fds[WE]);

	int base;
	if(read(pipe_fds[RE], &base, sizeof(int)) > 0){
		fprintf(WE, "prime %d\n", base);
		// fork 一个新的进程出来以本线程的输入作为输出
		if(fork() == 0){
			subPrimes(curr_pipe_fds);
		}
		else{
			close(curr_pipe_fds[RE]);
			int decisive_num;
			while(read(pipe_fds[RE], &decisive_num, sizeof(int)) > 0){
				if(decisive_num % base != 0)
					write(curr_pipe_fds[WE], &decisive_num, sizeof(int));
			}
			close(curr_pipe_fds[WE]);
		}
		wait((int*) 0);
	}
	exit(0);
}