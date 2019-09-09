#include<stdio.h>
#include<stdlib.h>


int main(){
    FILE* fp=fopen("data.csv","w");
    for(long i=0;i<1000000;i++){
      fprintf(fp,"%d,%d\n",rand()%2000-1000,rand()%2000-1000);
    }
}
