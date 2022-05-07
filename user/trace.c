#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

/* 
  ex1: trace 32 grep hello README
  ex2: trace 2147483647 grep hello README
  ex3: trace 2 usertests forkforkfork
*/
int
main(int argc, char *argv[])
{
  int i;
  char *nargv[MAXARG];

  // 最少 3 个参数，trace 本身、待跟踪的 SYSCALL、待执行的程序
  if(argc < 3 || (argv[1][0] < '0' || argv[1][0] > '9')){
    fprintf(2, "Usage: %s mask command\n", argv[0]);
    exit(1);
  }

  // 调用 trace 系统调用，传入待跟踪的系统调用号
  if (trace(atoi(argv[1])) < 0) {
    fprintf(2, "%s: trace failed\n", argv[0]);
    exit(1);
  }
  
  // 执行待跟踪的程序, nargv 存放的是待执行的程序以及其参数
  for(i = 2; i < argc && i < MAXARG; i++){
    nargv[i-2] = argv[i];
  }
  exec(nargv[0], nargv);
  exit(0);
}
