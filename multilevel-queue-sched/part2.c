#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <math.h>
#include <string.h>
#include "part2.h"
#define INP "inp_file"

void create_queue(char buf[], queue *main) {
	const char ct[2] = " ";
	char *t;
	char *cpu_algo = malloc(5*sizeof(char)); //algo
	//check if queue is ready
	if(buf[0] != 'q')
		return;
	//allocate memory for proc array
	int *proc = malloc(10*sizeof(int)); 
	t = strtok(buf, " "); //tokenize the buffer
	if(!strcmp(t, "q")) {
		t = strtok(NULL, ct); //all these are in a std position
		main ->q = atoi(t);
		t=strtok(NULL, ct);
		main -> pr = atoi(t=strtok(NULL,ct));
		t=strtok(NULL, ct);
		strcpy(cpu_algo, t);
		main -> cpu_algo = cpu_algo;
		if(strcmp(t, "rr") == 0) { //check if rr to set tq
			t=strtok(NULL, ct);
			main -> tq = atoi(t=strtok(NULL,ct));
		}
	}
	t=strtok(NULL, ct); //loop through the p1/cpu bursts
	for(int i = 0; i<10; i++) {
		t=strtok(NULL,ct); //get cpu burst
		proc[i] = atoi(t); //conver to integer
		t=strtok(NULL, ct);//get px so we can skip them
	}
	main -> qu = proc; //set up pointers
	return;
}
queue *ini_env() {
	FILE *fp; //declare ptr for inp file
	char buff[255];//set up local vars and queue array
	int i=0;
	queue tmp;
	queue *main = (queue *)malloc(3 * sizeof(queue));	
	fp = fopen(INP, "r");
	while(fgets(buff, 255, fp)) { //read line by line and organize queues appropriately
		i = buff[7]-'1';
		create_queue(buff, &(main[i])); //-1 since we want pr 1 in spot 0
		if(!main[i].q)
			printf("Error creating queue....\n");
		
	}
	fclose(fp); //close file
	return main;
}
void execute(queue * work) {
	int i = 0; //since we have a constant numb of procs
	//fakly execute the process's
	printf("Currently executing priority [%d], algo [%s]\n", work -> pr, work -> cpu_algo);
	return; //No need to change any info for fcfs
}

int main() {
	queue *test = ini_env();
	for(int i=0; i<3; i++) {
		execute(&test[i]);
	} //Returned from execution time to output the results
	int total;
	for(int t = 2; t >= 0; t--)
		print_results(test[t], &total);
	printf("Total time taken: %d\n", total);
	return 0; 
}

void print_results(queue out, int *t) {
	int avg =0; //this functoin could be seen as the actual "executing" because
	//i implemented all the algorithms here
	printf("Printing results for Queue #%d and algo [%s]\n", out.q, out.cpu_algo);
	if(!strcmp(out.cpu_algo, "fcfs")) { //vary styles of output based on type of algo
		int i = 0; 
		for(int t=0; t<10; t++) {
			printf("Proc %d executed with time %d\n", t, i);
			avg +=i;
			i+= out.qu[t];
		}
	}
	else if(!strcmp(out.cpu_algo, "sjf")) {
		int ord[10];
		int i=0, pos = 0;
		while(i<10) {
			pos = 0;
			for(int t=0; t<10; t++) { //find the position of the process execution
				if(out.qu[i] > out.qu[t])
					pos +=1;
			}
			ord[pos] = i+1; //set the position to the proc #
			i+=1;
		}
		int t =0;
		for(int k=0; k<10; k++) { //print order and wait times
			printf("Proc %d executed with wait time %d\n", ord[k], t);
			avg += t;
			t += out.qu[ord[k]-1];
		}
		
	} else { //round robin
		int i=0, max = 0;
		for(int k=0; k<10; k++) { //find how many times the max proc will execute
			if(out.qu[k] > out.qu[max])
				max = k;
			avg += out.qu[k];
		}
		int numb_exec = ceil(out.qu[max]/out.tq); 
		printf("Max executes: %d, found at proc: %d\nOrder: ", numb_exec, max);
		for(int r =0; r < numb_exec; r++) { //loop through all the procs
			for(int t=0; t<10; t++)
				if( ceil(out.qu[t] - r*out.tq) -1 >=0)
					printf("p%d, ", t+1);
		}
		printf("\n");
		for(int k=0; k<10; k++)
			printf("Proc %d had turn around time: %d\n", k+1, out.qu[k]);

	}
	*t += avg; //avg is just total time, add it to the pointer
	printf("Average wait time: %f\n", (float) avg/10); //divide avg and print
}


