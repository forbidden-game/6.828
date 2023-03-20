//
// Created by ForbiddenGame on 2023/3/19.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(const char *path, const char *filename)
{
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;


	if((fd = open(path, 0)) < 0){
		fprintf(2, "find: no such a directory or file %s\n", path);
		exit(1);
	}

	if(fstat(fd, &st) < 0){
		fprintf(2, "find: cannot stat %s\n", path);
		close(fd);
		exit(1);
	}

	switch(st.type){
		case T_FILE:
			fprintf(2, "usage: find path filename\n");
			exit(1);

		case T_DIR:
			if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
				fprintf(2, "path too long\n");
				break;
			}
			strcpy(buf, path);
			p = buf + strlen(path);
			*p++ = '/';
			while(read(fd, &de, sizeof(de)) == sizeof(de)){
				if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
					continue;

				strcpy(p, de.name);

				if(stat(buf, &st) < 0){
					printf("cannot stat %s\n", buf);
					continue;
				}

				if(st.type == T_DIR){
					find(buf, filename);
				} else{
					if(strcmp(de.name, filename) == 0){
						printf("%s\n", buf);
					}
				}
			}
			break;
	}
	close(fd);
	return;
}

int main(int argc, char const *argv[])
{
	if(argc != 3){
		fprintf(2, "usage: find path filename\n");
		exit(1);
	}

	const char *path = argv[1];
	const char *filename = argv[2];

	find(path, filename);
	exit(0);
}