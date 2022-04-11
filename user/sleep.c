// Copyright: 	LeiChao 
// Author: 		Leichao
// Github:		https://github.com/GiganticRay
// Date:		2022/04/11

#include "kernel/types.h"
#include "user/user.h"

// C command line argument
// argc: argument count, 
// By default argc is 1 and argv[0] = filename

// edit: using fprintf instead of printf to put output into specified output stream.
// exit(1) insetead of exit(-1) since exit(1) indicate abnormal termination of the program.
int
main(int argc, char *argv[]){
	if(argc == 1){
		fprintf(2, "one time parameter is expected!\n");
		exit(-1);
	}
	if(argc > 2){
		fprintf(2, "too many argument!\n");
		exit(-1);
	}
	sleep(atoi(argv[1]));
	exit(0);
}
