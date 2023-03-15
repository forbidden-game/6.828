//
// Created by ForbiddenGame on 2023/3/15.
//

#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
	uint64 pid;
	int p[2];
	char buf[60];

	if(pipe(p) == -1){
		fprintf(2, "pipe failed");
		exit(1);
	}
	pid = fork();
	if(pid == -1){
		fprintf(2, "fork failed");
		exit(1);
	}

	if(pid == 0){

		int n_read = read(p[0], buf, sizeof(buf));
		if(n_read == -1){
			fprintf(2, "child process read failed");
			exit(1);
		}
		close(p[0]);

		printf("%d: received ping\n", pid);

		int n_write = write(p[1], "hello", 5);
		if(n_write == -1){
			fprintf(2, "child process write failed");
			exit(1);
		}
		close(p[1]);

		exit(0);
	} else{

		int n_write = write(p[1], "p", 1);
		if(n_write == -1){
			fprintf(2, "parent process write failed");
			exit(1);
		}
		close(p[1]);

		sleep(10);

		int n_read = read(p[0], buf, sizeof(buf));
		if(n_read == -1){
			fprintf(2, "parent process read failed");
			exit(1);
		}
		close(p[0]);

		printf("%d: received pong\n", pid);
		exit(0);
	}
}