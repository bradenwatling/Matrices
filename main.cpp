#include <stdio.h>
#include <stdlib.h>

struct Matrix {
	int rows, cols;
	double **mat;
};

void addRows(Matrix matrix, int srcRow, int dstRow) {
	if (srcRow >= 0 && dstRow >= 0 && srcRow < matrix.rows && dstRow < matrix.rows)
	{
		for (int i = 0; i < matrix.cols; i++) {
			matrix.mat[dstRow][i] += matrix.mat[srcRow][i];
		}
	}
}

void subtractRows(Matrix matrix, int srcRow, int dstRow) {
	if (srcRow >= 0 && dstRow >= 0 && srcRow < matrix.rows && dstRow < matrix.rows)
	{
		for (int i = 0; i < matrix.cols; i++) {
			matrix.mat[dstRow][i] -= matrix.mat[srcRow][i];
		}
	}
}

void multRow(Matrix matrix, int dstRow, double factor) {
	if (dstRow >= 0 && dstRow < matrix.rows)
	{
		for (int i = 0; i < matrix.cols; i++) {
			matrix.mat[dstRow][i] *= factor;
		}
	}
}

void divideRow(Matrix matrix, int dstRow, double factor) {
	if (dstRow >= 0 && dstRow < matrix.rows)
	{
		for (int i = 0; i < matrix.cols; i++) {
			matrix.mat[dstRow][i] /= factor;
		}
	}
}

int indexOfLeadingNonZero(int row, Matrix matrix) {
	int ret = -1;
	for (int i = 0; i < matrix.cols && ret < 0; i++) {
		if (matrix.mat[row][i] != 0) {
			ret = i;
		}
	}

	return ret;
}

void echelonForm(Matrix matrix) {
	//For each row
	for (int i = 0; i < matrix.rows; i++) {
		int leadingNonZero = indexOfLeadingNonZero(i, matrix);

		if (leadingNonZero >= 0) {
			//Give all rows a 1 in column with leading non-zero
			for (int j = i; j < matrix.rows; j++) {
				double factor = matrix.mat[j][leadingNonZero];
				if (factor != 0) {
					divideRow(matrix, j, factor);
				}
			}
		}
		//For each row
		for (int k = 0; k < matrix.rows; k++) {
			//If the row we're subtracting from is not our working row
			//And if it's leading coefficient is non-zero
			if (k != i && matrix.mat[k][leadingNonZero] != 0) {
				subtractRows(matrix, i, k);
			}
		}
	}
}

void reducedEchelonForm(Matrix matrix) {
	//Iterate over rows from last to first
	for (int i = matrix.rows - 1; i >= 0; i--) {
		//Find the leading non zero in the current row
		int leadingNonZero = indexOfLeadingNonZero(i, matrix);
		//If the row isn't all zeros
		if (leadingNonZero >= 0) {
			//Iterate over all rows above this one
			for (int j = i - 1; j >= 0; j--) {
				//Determine the factor to cancel out the leadingNonZero column
				double factor = matrix.mat[j][leadingNonZero];
				if (factor != 0) {
					//Cancel that row and leave the current row in tact
					multRow(matrix, i, factor);
					subtractRows(matrix, i, j);
					divideRow(matrix, i, factor);
				}
			}
		}
	}
}

void printMatrix(Matrix matrix) {
	for (int x = 0; x < matrix.rows; x++) {
		for (int y = 0; y < matrix.cols; y++) {
			printf("%4.1lf ", matrix.mat[x][y]);
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

	if (generate == 'y') {
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
	
	printf("Matrix:\n");
	printMatrix(matrix);

	echelonForm(matrix);
	reducedEchelonForm(matrix);

	printf("Reduced Row Echelon Form:\n");
	printMatrix(matrix);

	return 0;
}
