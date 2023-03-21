//
// Created by ForbiddenGame on 2023/3/16.
//

#include "kernel/types.h"
#include "user/user.h"
#define PRIME_NUM 35
#define READEND 0
#define WRITEEND 1


void
new_proc(int p[2])
{
	int prime;
	int q;
	int newp[2];

	close(p[WRITEEND]); // 先关闭写端，避免打开太多管道而没有关闭

	int read_first = read(p[READEND], &prime, sizeof(int));
	if(read_first == 0){
		exit(0);
	}
	pipe(newp);

	if(fork() == 0){
		new_proc(newp);
	} else{
		close(newp[READEND]);
		printf("prime %d\n", prime);
		while(read(p[READEND], &q, sizeof(int))){
			if(q % prime){
				write(newp[WRITEEND], &q, sizeof(int));
			}
		}
		close(p[READEND]);
		close(newp[WRITEEND]);
		wait(0);
	}

	exit(0);
}

int
main(int argc, char* argv[])
{
	int p[2];

	if(pipe(p) == -1){
		fprintf(2, "parent proc pipe failed");
		exit(1);
	}

	if(fork() == 0){
		new_proc(p); // 利用 new_proc 函数将此管道传给子进程
	} else{
		close(p[READEND]);
		for (int i = 2; i <= PRIME_NUM + 1; ++i) {
			if(write(p[WRITEEND], &i, sizeof(int)) != sizeof(int)){
				fprintf(2, "parent proc write failed");
			}
		}
		close(p[WRITEEND]);
		wait(0);
		exit(0);
	}
	return 0;
}