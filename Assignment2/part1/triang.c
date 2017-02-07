#include <stdio.h>
#include <stdlib.h>

int** allocate_matrix(int n);
double binomial(int n, int k);
void fill_pascal(int** matrix, int n);
void print_triang(int** matrix, int n);

int main(int argc, char const *argv[]) {
	// Set length of triangle and allocate
	int n = atoi(argv[1]);
	int ** tri = allocate_matrix(n);
	
	// Compute and print
	fill_pascal(tri, n);
	print_triang(tri, n);
		
	// Free Pascal's triangle
	int i;
	for(i = 0; i < n; i++) {
		free(tri[i]);
	}
	free(tri);
	
	return 0;
}

/*
* Allocates a matrix of size n*n.
*/

int** allocate_matrix(int n) {
	int** matrix = (int **)malloc(n*sizeof(int *));
	int i;
	for(i = 0; i < n; i++) {
		matrix[i] = (int *)malloc(n*sizeof(int));
	}
	
	return matrix;
}

/*
* Recursively computes binomial coefficient (n k).
*/
double binomial(int n, int k) {
	if(k == 1) {
		return (double)n;
	} else if(k == 0) {
		return 1;
	} else {
		return (double)(n+1-k)/k * binomial(n, k-1);
	}	
}
			
void fill_pascal(int **matrix, int n) {
	int i, j;
	for(i = 0; i < n; i++) {
		for(j = 0; j < n; j++) {
			matrix[i][j] = binomial(i, j);
		}
	}
}

void print_triang(int **matrix, int n) {
	int i, j;
	for(i = 0; i < n; i++) {
		for(j = 0; j <= i; j++) {
			printf("%d\t", matrix[i][j]);
		}
		printf("\n");
	}
}
			
