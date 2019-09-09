#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>


// The sum computed by the background thread
struct sum_runner_struct {
	long long limit;
        long long ans1;
	long long count;
        long long n;

};

int arr1[1000000];
int arr2[1000000];

double computemean(int arr[], int n){
    double sum=0.0;
    for(int i=0;i<n;i++){
      sum += arr[i];
    }
    return sum/n;
}

double sdev(int arr[], int n, double mean){
    double sum = 0.0;
    for(int i=0;i<n;i++)
    {
       sum += (((double)arr[i]-mean)*((double)arr[i]-mean));
    }
    return sqrt(sum/n);
}

double computetstats(int arr1[], int arr2[], int n){
    double x1 = computemean(arr1, n);
    double x2 = computemean(arr2, n);
    double s1 = sdev(arr1, n, x1);
    double s2 = sdev(arr2, n, x2);
    double tstats = (x2-x1)/sqrt((s2*s2+s1*s1)/n);
    return tstats;
}


int solve(int n, int k, int ans1) 
{ 
    int i;  
    int reservoir1[k], reservoir2[k];
    unsigned int rr = rand();
    srand(time(NULL)); 

    for (i = 0; i < k; i++) {
        reservoir1[i] = arr1[rand_r(&rr) % (1000000)]; 
        reservoir2[i] = arr2[rand_r(&rr) % (1000000)];
    } 
  
  
    double ans2 = computetstats(reservoir1, reservoir2, k);
    if(ans2>=ans1) return 1;
    return 0;
} 

void* sum_runner(void* arg){
        struct sum_runner_struct *arg_struct = (struct sum_runner_struct*) arg;
        long long limit = arg_struct->limit;
        long long ans1 = arg_struct->ans1;
        long long hits = 0;
     

	for (long long i = 0; i <= limit; i++) {
		if(solve(arg_struct->n, 20, ans1) == 1){
                  hits += 1;
                }
                
	}

        arg_struct->count = hits;
	pthread_exit(0);
}

int main(int argc, char **argv)
{
      


      int ar[6] = {2, 4, 8, 16, 32, 40};
      for(int tt=0; tt<6; tt++){
      
        time_t ta;
	srand((unsigned) time(&ta));    
	struct timespec begin, end;
	double elapsed;	
	clock_gettime(CLOCK_MONOTONIC, &begin);

      FILE *fp=fopen("data.csv","r");
      for(int i=0;i<1000000;i++)
      {
        fscanf(fp,"%d,%d",&arr1[i], &arr2[i]);
      }
      //fclose(fp);

      double ans1 = computetstats(arr1, arr2, 1000000);
      printf("t-statistics for the whole file is %f \n", ans1);
	long long ntimes = 10000000;
        long long t = ar[tt]; //atoll(argv[1]);

	struct sum_runner_struct args[t];

        pthread_t tids[t];

	for (long long i = 0; i < t; i++) {
		args[i].limit = ntimes/t;
                args[i].count = 0;
               
                args[i].ans1 = ans1;
                args[i].n = 1000000;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tids[i], &attr, sum_runner, &args[i]);
	}

         long long sum = 0;
         for (long long i = 0; i < t; i++) {
		pthread_join(tids[i], NULL);
                sum += args[i].count;
		
	}
       
        printf("# times the overall t-statistic of random samples is greater is %lld\n", sum);

	clock_gettime(CLOCK_MONOTONIC, &end);

    	elapsed = end.tv_sec - begin.tv_sec;
    	elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
    	printf("Time taken to execute is:%lf seconds\n",elapsed);


       FILE *fptr;
       fptr = fopen("data2.txt", "a");
       fprintf(fptr,"%Ld %f \r\n", t, elapsed);
       fclose(fptr); 
   }

}
