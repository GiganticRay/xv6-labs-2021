// Copyright: 	LeiChao 
// Author: 		Leichao
// Github:		https://github.com/GiganticRay
// Date:		2022/04/12

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

/* pseudo code find(path, name)
	get stat of path
	use buf to concatenate path/
	while(read each elem(stored in de, since we need name) in path directory file):
		use buf to concatenate path/elem.name
		get stat of buf
		switch(buf.stat.type)
			case: file
				compare
			case: dir
				find(buf, name)
*/

void 
find(char* path, char* name){
	char buf[512], *p;

	int fd;
	struct stat st;
	struct dirent de;
	if((fd = open(path, O_RDONLY)) < 0){
		fprintf(2, "find: cannot open %s\n", path);
		return;
	}

	if(fstat(fd, &st) < 0){
		fprintf(2, "find: connot stat %s\n", path);
		close(fd);
		return;
	}

	switch(st.type){
		case T_FILE:
			fprintf(2, "parameters given must be directory.\n");
			exit(-1);
		case T_DIR:
			// traverse all files inside directory
			// dir:  recursion
			// file: compare
			if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
				printf("find: path too long");
				break;
			}
			strcpy(buf, path);
			p = buf + strlen(path);
			*p++ = '/';
			while(read(fd, &de, sizeof(de)) == sizeof(de)){
				// if file, then compare name
				// if dir, then recursion
				if(de.inum == 0)
					continue;
				struct stat tmp_st;
				memmove(p, de.name, DIRSIZ);
				p[DIRSIZ] = 0;
				if(stat(buf, &tmp_st) < 0){
					printf("find: cannot stat %s\n", buf);
				}
				else{
					switch (tmp_st.type)
					{
						case T_FILE:
							if(strcmp(name, de.name) == 0){
								fprintf(1, "%s\n", buf);
							}
							break;
						case T_DIR:
							if(strcmp(".", de.name) && strcmp("..", de.name) != 0){
								find(buf, name);
								break;
							}
						
						default:
							break;
					}
				}
			}
			break;
	}
	close(fd);
	return;
}

int
main(int argc, char* argv[]){
	// user have to provide 1 arg at least(file name)
	if(argc < 2){
		fprintf(2, "more arguments are expected.");
		exit(-1);
	}
	else if(argc == 2){
		find(".", argv[1]);
		exit(0);
	}
	else{
		int i;
		for(i = 1; i < argc-1; i++)
			find(argv[i], argv[argc-1]);
		exit(0);
	}
}