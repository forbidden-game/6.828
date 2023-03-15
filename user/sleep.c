//
// Created by ForbiddenGame on 2023/3/15.
//

#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
	if(argc <= 1){
		fprintf(2, "usage sleep [time]\n");
		exit(1);
	}

	if(argv[1][0] == '-'){
		fprintf(2, "time must be positive\n");
		exit(1);
	}

	int time = atoi(argv[1]);
	sleep(time);
	exit(0);
}
