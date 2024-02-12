#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>


int main(){

	int shmid, *pointer;
	int max_iter = 5000000;

	shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT|S_IRUSR|S_IWUSR);
	pointer = (int *)shmat(shmid, 0, 0);
	*pointer = 0;

	if(!fork()){
		for (int i = 0; i < max_iter; ++i)
		{
			*pointer = *pointer + 1;
		}
		shmdt(pointer);
	}
	else{
		
		
		for (int i = 0; i < max_iter; ++i)
		{
			*pointer = *pointer + 1;
		}

		wait(NULL);
		printf("the value of pointer is = %d\n", *pointer);

		shmdt(pointer);
		shmctl(shmid, IPC_RMID, NULL);
	}

	return EXIT_SUCCESS;
}