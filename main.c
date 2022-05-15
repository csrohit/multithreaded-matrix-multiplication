#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>

int n_rows, n_cols, n_threads;
float *a, *b, *c;

float multiply(int row_num, int col_num);
void print_matrix(float *mat, int n_rows, int n_cols);
void gen_matrix(float *mat, int n_rows, int n_cols);

void * mat_mul(void *params){
	long thread_num = (long) params;
	// printf("Thread %ld\n", thread_num);
	int i=(int) thread_num,j;
	// *c = multiply(0,0);
	for(; i< n_rows; i+=n_threads){
		for(j=0; j < n_cols; j++)
			*(c + i*n_cols + j) = multiply(i, j);
	}
}

int main(){
	int i=0, ret;
	pthread_t *threads;
	double time_spent = 0.0;
	time_t begin, end;
	
	printf("Enter number of rows: ");
	scanf("%d", &n_rows);
	printf("Enter number of columns: ");
	scanf("%d", &n_cols);
	printf("Enter number of threads: ");
	scanf("%d", &n_threads);
	
	if(n_threads > n_rows)
		n_threads = n_rows;

	a = (float *) malloc(sizeof(float)*n_rows*n_cols);
	b = (float *) malloc(sizeof(float)*n_rows*n_cols);
	c= (float *) malloc(sizeof(float)*n_rows*n_cols);
	
	// printf("Matrix A: \n");
	gen_matrix(a, n_rows, n_cols);
	// print_matrix(a, n_rows, n_cols);
	
	// printf("Matrix B: \n");
	gen_matrix(b, n_rows, n_cols);
	// print_matrix(b, n_rows, n_cols);
	
	begin = time(NULL);
	threads = (pthread_t *) malloc(sizeof(pthread_t)*n_threads);
	for(i=0; i<n_threads; i++){
		ret = pthread_create(threads+i, NULL, mat_mul, (void *)i);
	}

//	pause();
	for(i-=1; i>=0; i--){
		pthread_join(*(threads+i), NULL);
	}
	end = time(NULL);
	// printf("Matrix C: \n");
	// print_matrix(c, n_rows, n_cols);

	printf("Time elapsed: %ld\n", end-begin);

	free(threads);
	free(a);
}

float multiply(int row_num, int col_num){
	float sum = 0.00;
	int i;
	for(i=0; i<n_cols; i++){
		sum += *(a + row_num * n_cols + i) * *(b + col_num + i* n_cols );
		// printf("%8.2f * %8.2f = %10.2f\n", *(a + row_num * n_cols + i) , *(b + col_num + i* n_cols ), *(a + row_num * n_cols + i) * *(b + col_num + i* n_cols ));
	}
	return sum;
}

void print_matrix(float *mat, int n_row, int n_col){
	
	int i,j;
	for(i=0; i< n_rows;i++){
		for(j=0; j<n_cols; j++){
			printf("%8.2f", *(mat + i*n_cols + j));
		}
		printf("\n");
	}

}


void gen_matrix(float *mat, int n_row, int n_col){
	
	int i,j;
	for(i=0; i< n_rows;i++){
		for(j=0; j<n_cols; j++){
			float x = ((float)(rand()%100)/10);
//		 printf("generated a[%d][%d]=%8.2f\n",i,j,x);
			*(mat + i*n_cols + j) = x;
		}
	}

}



