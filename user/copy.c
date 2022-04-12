// Copyright: 	LeiChao 
// Author: 		Leichao
// Github:		https://github.com/GiganticRay
// Date:		2022/04/11

// copy.c: copy input to output
// adding function into xv6: https://www.geeksforgeeks.org/xv6-operating-system-add-a-user-program/

#include "kernel/types.h"
#include "user/user.h"

int main(){
	char buf[64];

	while(1){
		int n = read(0, buf, sizeof(buf));
		if(n <= 0)
			break;
		write(1, buf, n);
	}

	exit(0);
}