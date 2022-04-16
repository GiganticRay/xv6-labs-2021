// Copyright: 	LeiChao 
// Author: 		Leichao
// Github:		https://github.com/GiganticRay
// Date:		2022/04/12

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXPARALEN 32

int
main(int argc, char* argv[]){
	// read from standard input and put arguments into c_argv[].
	// expand memory dynamically.
	// argv[1] is command, argv[2:*] are arguments of command

	char* c_argv[32];
	int c_argc = 0;
	char buf[MAXPARALEN];

	char* command 	= argv[1];
	char* p_buf = buf;

	// get other arguments fro the standard input
	for(c_argc = 0; c_argc < argc-1; c_argc++){
		c_argv[c_argc] = argv[c_argc+1];
	}

	char tmp;
	while(read(0, &tmp, 1) > 0){
		if(tmp == '\n'){
			if(p_buf == buf)
				continue;
			if(c_argc >= MAXARG){
				fprintf(2, "xargs::main: too many arguments!");
				exit(-1);
			}
			// buf is a completed argument
			c_argv[c_argc] = buf;
			p_buf = buf;

			if(fork() == 0){
				exec(command, c_argv);
				exit(0);
			}
			else{
				wait((int*) 0);
			}
			// do not invoke wait here instead we invoke wait batchly in the end of process.
		}
		else{
			// put curr char into buf
			if(p_buf - buf >= MAXPARALEN){
				fprintf(2, "xargs::main: argument is too long!");
				exit(-1);
			}
			*p_buf = tmp;
			*++p_buf = '\0';
		}
	}

	// the last one. but the last character is \n implicitly so we don't need to add this
	/*
	if(p_buf != buf){
		if(c_argc >= MAXARG){
			fprintf(2, "xargs::main: too many arguments!");
			exit(-1);
		}
		// buf is a completed argument
		strcpy(c_argv[c_argc++], buf);
		p_buf = buf;
	}
	*/
	exit(0);
	
}