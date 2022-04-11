// Copyright: 	LeiChao 
// Author: 		Leichao
// Github:		https://github.com/GiganticRay
// Date:		2022/04/11

/*
	The parent should send a byte to the child; 
	the child should print "<pid>: received ping", where <pid> is its process ID, write the byte on the pipe to the parent, and exit; 
	the parent should read the byte from the child, print "<pid>: received pong", and exit.
*/

#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char* argv[]){
	char buf[1];
	int pipe_fds[2];
	// pipefd[0] refers to the read end of the pipe. pipefd[1] refers to the write end of the pipe
	if(pipe(pipe_fds) == -1){
		fprintf(2, "pipe error");
		exit(1);
	}
	// parent process write a byte into pipe.
	write(pipe_fds[1], "1", 1);
	if(fork() == 0){
		if(read(pipe_fds[0], buf, 1) != 1){
			fprintf(2, "read error from child process.");
			exit(1);
		}
		else{
			// sub process has already got the msg from the parent p. 
			fprintf(1, "%d: received ping\n", getpid());
			write(pipe_fds[1], "1", 1);
			exit(0);
		}
	}
	else{
		// wait(0);	// if the parent doesn't care about the exit status of a child, pass a 0 address to wait.
		int son_status;
		wait(&son_status);
		if(son_status == 1){
			fprintf(2, "sub process exit in exception.");
			exit(1);
		}

		if(read(pipe_fds[0], buf, 1) != 1){
			fprintf(2, "read error from parent process.");
			exit(1);
		}
		else{
			fprintf(1, "%d: received pong\n", getpid());
			exit(0);
		}
	}
}

