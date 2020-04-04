#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void sum(void *);



int main() {
	int fd[2], x=20, y=10, z=0, temp; //var declaration
	pid_t pid; //for fork
	pthread_t thread1; //for thread
	if(pipe(fd) < 0) { //set up pipe & error catch
		printf("Error creating pipe!");
		exit(1);
	}
	if((pid=fork()) < 0) { //create fork & error catch
		printf("Fork error!");
		exit(1);
	}
	else if(pid > 0) {  //parent process
		printf("Parent waiting for child...\n");
		wait(NULL); //wait for child
		
		read(fd[0], &z, sizeof(int)); //read from pipe
		printf("Child1 completed!\n\tValue of z = %d\n", z);
		
		x=17;//x for test
		int *arg[3] = {&x,&y,&z}; //set up pointers to
		//to local variables to pass as arguments
		int *pass = *arg; //pass is pointing to the 
		//value of arg[0]
		//create thread and pass "pass"
		if(pthread_create(&thread1, NULL, (void *) &sum, (void *)pass)) {
			printf("Error creating thread!\n");
		}
		pthread_join(thread1, NULL); //wait for thread
		//print the new value of z only using z.
		printf("Thread exited\n\t new value of z = %d\n",z);
	
	}
	else if(pid==0) { //child process
		printf("Entering Child1...\n");
		z = x+y; //add x&y 
		write(fd[1], &z, sizeof(int)); //send through pipe
		printf("Wrote z to pipe, exiting\n");
	}
	return 0;
}

void sum(void * x) {
	//more fun with pointers!
	*((int *)x+2) = *((int *)x+1) + *((int *)x);
	//we passed over the pointer to "x" or arg[0]. First cast x from
	//void * to int *. if we derefer. (*) we get the
	//value of x from the parent. If we add to it then dereference
	//it the pointer moves sizeof(int *) letting us access z&y.
	printf("Calculated sum exiting thread...\n");
}
