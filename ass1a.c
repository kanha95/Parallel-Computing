#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>


// The sum computed by the background thread
struct sum_runner_struct {
	long long limit;
        long long radius;
	long long answer;
};

// Thread function to generate sum of 0 to N
void* sum_runner(void* arg){
        struct sum_runner_struct *arg_struct = (struct sum_runner_struct*) arg;
        long long limit = arg_struct->limit;
        long long r = arg_struct->radius;
        long long rsquare = r*r;
        long long hits = 0;
        unsigned int rr = rand();
        srand(time(NULL));

	for (long long i = 0; i <= limit; i++) {
		int x = (rand_r(&rr) % (2*r + 1)) - r;
                int y = (rand_r(&rr) % (2*r + 1)) - r;
                if(x*x+y*y <= rsquare){
                   hits += 1;
                }
                
	}
        arg_struct->answer = hits;
	// sum is a global variable, so other threads can access.

	pthread_exit(0);
}

int main(int argc, char **argv)
{
      int ar[7] = {1, 2, 4, 8, 16, 32, 40};
      for(int tt=0; tt<7; tt++){
	long long n = 10000000;
        long long r = 2000;
        long long t = ar[tt]; //atoll(argv[1]);

	struct sum_runner_struct args[t];
	time_t ta;
	srand((unsigned) time(&ta));    
	struct timespec begin, end;
	double elapsed;	
	clock_gettime(CLOCK_MONOTONIC, &begin);
        pthread_t tids[t];

	for (long long i = 0; i < t; i++) {
		args[i].limit = n/t;
                args[i].radius = r;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tids[i], &attr, sum_runner, &args[i]);
	}

         long long sum = 0;
         for (long long i = 0; i < t; i++) {
		pthread_join(tids[i], NULL);
                sum += args[i].answer;
		
	 }

        long double pi= (4*sum*1.0)/(n); 
        printf("Value pf pi is %Lf\n", pi);
	clock_gettime(CLOCK_MONOTONIC, &end);

    	elapsed = end.tv_sec - begin.tv_sec;
    	elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
    	printf("Time taken to execute is:%lf seconds\n",elapsed);


       FILE *fptr;
       fptr = fopen("data.txt", "a");
       fprintf(fptr,"%Ld %f \r\n", t, elapsed);
       fclose(fptr);
     
 }   
}
