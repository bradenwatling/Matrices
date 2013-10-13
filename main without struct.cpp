#include <stdio.h>
#include <stdlib.h>

struct Matrix {
	int rows, cols;
	double **mat;
};

void addRows(int rows, int cols, double **mat, int srcRow, int dstRow) {
	if (srcRow >= 0 && dstRow >= 0 && srcRow < rows && dstRow < rows)
	{
		for (int i = 0; i < cols; i++) {
			mat[dstRow][i] += mat[srcRow][i];
		}
	}
}

void subtractRows(int rows, int cols, double **mat, int srcRow, int dstRow) {
	if (srcRow >= 0 && dstRow >= 0 && srcRow < rows && dstRow < rows)
	{
		for (int i = 0; i < cols; i++) {
			mat[dstRow][i] -= mat[srcRow][i];
		}
	}
}

void multRow(int rows, int cols, double **mat, int dstRow, double factor) {
	if (dstRow >= 0 && dstRow < rows)
	{
		for (int i = 0; i < cols; i++) {
			mat[dstRow][i] *= factor;
		}
	}
}

void divideRow(int rows, int cols, double **mat, int dstRow, double factor) {
	if (dstRow >= 0 && dstRow < rows)
	{
		for (int i = 0; i < cols; i++) {
			mat[dstRow][i] /= factor;
		}
	}
}

int indexOfLeadingNonZero(int row, int cols, double **mat) {
	int ret = -1;
	for (int i = 0; i < cols && ret < 0; i++) {
		if (mat[row][i] != 0) {
			ret = i;
		}
	}

	return ret;
}

void echelonForm(int rows, int cols, double **mat) {
	//For each row
	for (int i = 0; i < rows; i++) {
		int leadingNonZero = indexOfLeadingNonZero(i, cols, mat);

		if (leadingNonZero >= 0) {
			//Give all rows a 1 in column with leading non-zero
			for (int j = i; j < rows; j++) {
				double factor = mat[j][leadingNonZero];
				if (factor != 0) {
					divideRow(rows, cols, mat, j, factor);
				}
			}
		}
		//For each row
		for (int k = 0; k < rows; k++) {
			//If the row we're subtracting from is not our working row
			//And if it's leading coefficient is non-zero
			if (k != i && mat[k][leadingNonZero] != 0) {
				subtractRows(rows, cols, mat, i, k);
			}
		}
	}
}

void reducedEchelonForm(int rows, int cols, double **mat) {
	//Iterate over rows from last to first
	for (int i = rows - 1; i >= 0; i--) {
		//Find the leading non zero in the current row
		int leadingNonZero = indexOfLeadingNonZero(i, cols, mat);
		//If the row isn't all zeros
		if (leadingNonZero >= 0) {
			//Iterate over all rows above this one
			for (int j = i - 1; j >= 0; j--) {
				//Determine the factor to cancel out the leadingNonZero column
				double factor = mat[j][leadingNonZero];
				if (factor != 0) {
					//Cancel that row and leave the current row in tact
					multRow(rows, cols, mat, i, factor);
					subtractRows(rows, cols, mat, i, j);
					divideRow(rows, cols, mat, i, factor);
				}
			}
		}
	}
}

void printMatrix(int rows, int cols, double **mat) {
	for (int x = 0; x < rows; x++) {
		for (int y = 0; y < cols; y++) {
			printf("%10.2lf ", mat[x][y]);
		}
		printf("\n");
	}
	printf("\n");
}

int main() {
	char generate;
	printf("Generate random matrix? (y/n): ");
	scanf("%c", &generate);

	int rows, cols;
	printf("Enter # rows: ");
	scanf("%d", &rows);
	printf("Enter # columns: ");
	scanf("%d", &cols);

	double *mat[rows];
	//For each row, create an array of size cols
	for (int i = 0; i < rows; i++) {
		mat[i] = new double[cols];
	}

	if(generate == 'y') {
		for (int x = 0; x < rows; x++) {
			for (int y = 0; y < cols; y++) {
				mat[x][y] = rand() % 10;
			}
		}
	} else {
		printf("Enter your matrix: \n");
		for (int x = 0; x < rows; x++) {
			for (int y = 0; y < cols; y++) {
				printf("Enter (row, column) (%d, %d): ", x, y);
				scanf("%lf", &mat[x][y]);
			}
		}
	}

	Matrix matrix;
	matrix.rows = rows;
	matrix.cols = cols;
	matrix.mat = mat;
	
	printMatrix(rows, cols, mat);
	echelonForm(rows, cols, mat);
	printMatrix(rows, cols, mat);
	reducedEchelonForm(rows, cols, mat);
	printMatrix(rows, cols, mat);
	return 0;
}
