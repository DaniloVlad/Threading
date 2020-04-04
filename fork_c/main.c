#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	pid_t c1, c2;		//Declare vars for use
	if((c1 = fork()) < 0) {	//Create child 1, make sure theres no error
		printf("Error creating child_1!\n");
		exit(c1);
	}
	if(c1 > 0) {		//Parent
		printf("Parent (PID %d) created child_1 (PID %d)\n", getpid(), c1);
		wait(NULL); 	//wait for child_1 to finish
		if((c2 = fork()) < 0) {
			printf("Error creating child_2!\n");
			exit(c2);
		}
		if(c2 > 0) {	//parent to child_2	
			printf("Parent (PID %d) created child_2 (PID %d)\n", getpid(), c2);
			wait(NULL);
			
		}
		if(c2 == 0) {
			printf("Child_2 (PID %d) is calling external program!\n", getpid());
			execl("b.out", NULL); //execute b.out, and exit child_2
		}
	}
	if(c1 == 0) {	//Child_1 
		printf("Parent (PID %d) is waiting for child_1 (PID %d) to complete before creating child_2\n", getppid(), getpid());
		c1 = fork(); //Create child_1.1
		if (c1>0) 	//Make child_1 wait for child_1.1
			wait(NULL);
		if(c1 == 0)  //child_1.1
			printf("Child_1 (PID %d) created Child_1.1 (PID %d)\n", getppid(), getpid());
		return 0;
	}

}
