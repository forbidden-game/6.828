//
// Created by ForbiddenGame on 2023/3/16.
//

#include "kernel/types.h"
#include "user/user.h"

void
new_proc(int p[2])
{
	int prime;
	int flag;
	int q;
	int newp[2];

	close(p[1]);

	if(read(p[0], &prime, 4) != 4){
		fprintf(2, "child proc read failed");
		exit(1);
	}

	printf("prime %d\n", prime);
	flag = read(p[0], &q, 4);

	if(flag){
		if(pipe(newp) == -1){
			fprintf(2, "child proc pipe failed");
			exit(1);
		}
		if(fork() == 0){
			new_proc(newp);
		} else{
			close(newp[0]);
			if(q % prime)
				write(newp[1], &q, 4);
			while(read(p[0], &q, 4)){
				if(q % prime)
					write(newp[1], &q, 4);
			}
			close(p[0]);
			close(newp[1]);
			wait(0);
		}
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
		new_proc(p);
	} else{
		close(p[0]);
		for (int i = 2; i <= 35; ++i) {
			if(write(p[1], &i, 4) != 4){
				fprintf(2, "parent proc write failed");
			}
		}
		close(p[1]);
		wait(0);
		exit(0);
	}
	return 0;
}