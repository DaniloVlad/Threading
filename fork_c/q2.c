#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	int fds[2], x, y, temp; //Declare vars
	pid_t pid;
	if(pipe(fds) < 0) {	//set up pipe and catch errors
		printf("Error creating pipe!\n");
		exit(0);
	}
	printf("A pipe is creatd for communication between parent (PID %d) and child!\n", getpid());
	printf("Enter a value for x: ");  //Get input from user in parent proccess for value x
	scanf(" %d", &x);

	if((pid = fork()) < 0) {	//Create child and catch error
		printf("Error creating child!\n");
		exit(0);
	}
	if(pid > 0) {	//parent
		printf("Parent (PID %d) created a child (PID %d)\n", getpid(), pid);
		wait(NULL);	//wait till child is done executing 
		temp = read(fds[0], &y, sizeof(int));	//I used sizeof(int) rather than just setting 4 bytes, reads info from pipe
		printf("Parent (PID %d) reading Y from pipe (Y = %d)\n", getpid() , y); //Final prints and sums
		printf("Parent (PID %d) adding x+y = %d\n", getpid(), x+y);
	} else {	//child
		printf("From child, enter a value for y: ");
		scanf(" %d", &y);
		printf("Writing Y = %d into pipe!\n", y);
		write(fds[1], &y, sizeof(int));  //send the user-entered value of y to parent.

	}

	return 0;
}
