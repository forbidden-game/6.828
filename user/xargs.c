//
// Created by ForbiddenGame on 2023/3/20.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

void copy(char **p1, char *p2){
	*p1 = malloc(strlen(p2) + 1);
	strcpy(*p1, p2);
}


int
read_line(char *new_argv[], int new_argc)
{
	int m = 1024;
	char buf[m];
	int n = 0;
	while(read(0, buf+n, 1)){
		if(buf[n] == '\n'){
			buf[n] = 0;
			break;
		}
		n++;
		if(n == m){
			fprintf(2, "too long arg\n");
			exit(1);
		}
	}
	if(n == 0){
		return -1;
	}
	int k = 0;
	while(k < n){
		if(new_argc > MAXARG){
			fprintf(2, "too many args\n");
		}
		while(k < n && buf[k] == ' ')
			k++;
		int l = k;
		while(k < n && buf[k] != ' ')
			k++;
		buf[k++] = 0;
		copy(&new_argv[new_argc++], buf + l);
	}
	return new_argc;
}


int
main(int argc, char *argv[])
{
	if(argc < 2){
		fprintf(2, "usage: xargs command\n");
		exit(1);
	}

	char *new_argv[MAXARG];
	for(int i = 1; i < argc; i++){
		new_argv[i - 1] = malloc(strlen(argv[i] + 1));
		strcpy(new_argv[i - 1], argv[i]);
	}

	int new_argc;
	while((new_argc = read_line(new_argv, argc - 1)) != -1){
		new_argv[new_argc] = 0;
		if(fork() == 0){
			exec(new_argv[0], new_argv);
			exit(1);
		} else{
			wait(0);
		}
	}
	exit(0);
}