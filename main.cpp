#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int rows, cols;
	double **mat;
} Matrix;

void printMatrix(Matrix matrix) {
	for (int x = 0; x < matrix.rows; x++) {
		for (int y = 0; y < matrix.cols; y++) {
			printf("%5.1lf ", matrix.mat[x][y]);
		}
		printf("\n");
	}
	printf("\n");
}

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

double **newMat(int rows, int cols) {
	double** mat = new double *[rows];
	//For each row, create an array of size cols
	for (int i = 0; i < rows; i++) {
		mat[i] = new double[cols];
	}
	return mat;
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

Matrix reduceMatrix(Matrix matrix) {
	Matrix ret;
	ret.rows = matrix.rows;
	ret.cols = matrix.cols;

	ret.mat = newMat(ret.rows, ret.cols);

	for (int y = 0; y < matrix.rows; y++) {
		for (int x = 0; x < matrix.cols; x++) {
			ret.mat[y][x] = matrix.mat[y][x];
		}
	}

	echelonForm(ret);
	reducedEchelonForm(ret);

	return ret;
}

bool getInverse(Matrix matrix, Matrix* out) {
	bool square = matrix.rows == matrix.cols, gotIdentityMatrix = true;
	if(square) {
		Matrix temp;
		temp.rows = matrix.rows;
		temp.cols = matrix.cols * 2;

		temp.mat = newMat(temp.rows, temp.cols);

		for (int y = 0; y < matrix.rows; y++) {
			for (int x = 0; x < matrix.cols; x++) {
				temp.mat[y][x] = matrix.mat[y][x];
			}
			for (int x = matrix.cols; x < temp.cols; x++) {
				if (x - matrix.cols == y) {
					temp.mat[y][x] = 1;
				} else {
					temp.mat[y][x] = 0;
				}
			}
		}

		echelonForm(temp);
		reducedEchelonForm(temp);

		//Check that the left side is the identity matrix
		for (int y = 0; y < matrix.rows; y++) {
			for (int x = 0; x < matrix.cols; x++) {
				if ((x == y && temp.mat[y][x] != 1) || (x != y && temp.mat[y][x] != 0)) {
					gotIdentityMatrix = false;
				}
			}
		}

		if (gotIdentityMatrix) {
			(*out).rows = matrix.rows;
			(*out).cols = matrix.cols;

			(*out).mat = newMat((*out).rows, (*out).cols);

			for (int y = 0; y < matrix.rows; y++) {
				for (int x = 0; x < matrix.cols; x++) {
					(*out).mat[y][x] = temp.mat[y][x + matrix.cols];
				}
			}
		}
	}
	return square && gotIdentityMatrix;
}

bool mult(Matrix one, Matrix two, Matrix* out) {
	//Condition required for multiplication
	bool ret = one.cols == two.rows;
	
	if (ret) {
		(*out).rows = one.rows;
		(*out).cols = two.cols;

		(*out).mat = newMat((*out).rows, (*out).cols);

		//Fill with zeros
		for (int y = 0; y < (*out).rows; y++) {
			for(int x = 0; x < (*out).cols; x++) {
				(*out).mat[y][x] = 0; 
			}
		}

		//For each vector making up the second matrix
		for (int z = 0; z < two.cols; z++) {
			//For each element in each vector making up the second matrix
			for (int y = 0; y < two.rows; y++) {
				//Multiply the item by the elements in the corresponding row of the first matrix
				//Place them in the new matrix with row=row for the first matrix, col=column of second matrix
				for(int x = 0; x < one.rows; x++) {
					(*out).mat[x][z] += two.mat[y][z] * one.mat[x][y];
				}
			}
		}
	}

	return ret;
}

int main() {
	char generate;
	printf("Generate random matrix? (y/n): ");
	scanf("%c", &generate);

	Matrix matrix;
	printf("Enter # rows: ");
	scanf("%d", &(matrix.rows));
	printf("Enter # columns: ");
	scanf("%d", &(matrix.cols));

	matrix.mat = newMat(matrix.rows, matrix.cols);

	if (generate == 'y') {
		for (int x = 0; x < matrix.rows; x++) {
			for (int y = 0; y < matrix.cols; y++) {
				matrix.mat[x][y] = rand() % 10;
			}
		}
	} else {
		printf("Enter your matrix: \n");
		for (int x = 0; x < matrix.rows; x++) {
			for (int y = 0; y < matrix.cols; y++) {
				printf("Enter (row, column) (%d, %d): ", x, y);
				scanf("%lf", &(matrix.mat[x][y]));
			}
		}
	}
	
	printf("Matrix:\n");
	printMatrix(matrix);

	Matrix reduced = reduceMatrix(matrix);

	printf("Reduced Row Echelon Form:\n");
	printMatrix(reduced);

	Matrix inv;
	if (getInverse(matrix, &inv)) {
		printf("Inverse:\n");
		printMatrix(inv);
	} else {
		printf("Not invertible.\n");
	}

	Matrix sqr;
	if (mult(matrix, matrix, &sqr)) {
		printf("Square of matrix:\n");
		printMatrix(sqr);
	} else {
		printf("Could not square matrix because it is not square.\n");
	}
	return 0;
}
