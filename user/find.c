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

	while(read(fd, &de, sizeof(de)) == sizeof(de)){
		strcpy(buf, path);
		p = buf + strlen(path);
		*p++ = '/';
		if(de.inum == 0){
			continue;
		}
		memmove(p, de.name, DIRSIZ);
		p[DIRSIZ] = 0;

		if(stat(buf, &st) < 0){
			fprintf(2, "ERROR: can't stat %s\n", buf);
			exit(1);
		}

		switch(st.type){
			case T_FILE:
				if(strcmp(de.name, filename) == 0){
					printf("%s\n", buf);
				}
				break;
			case T_DIR:
				if(strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0){
					find(buf, filename);
				}
		}
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