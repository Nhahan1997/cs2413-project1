/*
 * CSR.cpp
 *
 *  Created on: Feb 2, 2021
 *      Author: Nhan Le
 */

# include<iostream>
using namespace std;

class CSR{

	protected:
			int n;	//row
			int m;	// column
			int indexOfValue, indexOfCol; //keep track the index for values[] and colPos[]
			int nonZeros;	//number of nonZeros in matrix
			int* values;	//array contain all nonZeros value
			int* rowPtr;	//array contain the first position of nonZeros in each row
			int* colPos;
	public:
			CSR();	//default constructor
			CSR(CSR& matrixB);	//copy constructor
			CSR(int rows, int cols, int numNonZeros);	//constructor
			int getNumRows();	// return number of row n
			void addValue(int value);	//add a new value in the values array
			void addColumn(int col);	//add a column in the colPos array
			void addRow(int row);	//add a row in the rowPtr array
			void display();	//print the content of three arrays, its matrix.Each array must be on a different line
			int* getRowVec(int row);
			int* getColVec(int col);
			int* matrixVectorMultiply(int* inputVector);	//matrix-vector multiplication
			CSR* matrixMultiply(CSR& matrixB);	//matrix-matrix multipication
			~CSR();	//destructure
			CSR& operator = (CSR& matrixB);

};
//Initial all static and dynamic variable
CSR::CSR(){
	n =0;
	m =0;
	indexOfValue =0;
	indexOfCol =0;
	nonZeros = 0;
	values = NULL;
	rowPtr = NULL;
	colPos = NULL;
}

CSR::CSR(int rows, int cols, int numNonZeros){
	n = rows;
	m = cols;
	nonZeros = numNonZeros;
	indexOfValue =0;
	indexOfCol =0;
	values = new int[nonZeros];
	colPos = new int[nonZeros];
	rowPtr = new int[n];
}
CSR::CSR(CSR & matrixB){
	n = matrixB.n;
	m = matrixB.m;
	nonZeros = matrixB.nonZeros;
	indexOfValue =0;
	indexOfCol =0;
	values = matrixB.values;
	colPos = matrixB.colPos;
	rowPtr = matrixB.rowPtr;
}

int CSR::getNumRows(){
	return n;
}

void CSR::addValue(int value){
	values[indexOfValue++] = value;
}

void CSR::addColumn(int col){
	colPos[indexOfCol++] = col;
}

void CSR::addRow(int row){
	if(rowPtr[row+1] == 0){
		rowPtr[row+1] = rowPtr[row]+1;
	}
	else
		rowPtr[row+1]++;
}

void CSR::display(){

	for(int i=0; i< n; i++){
		int* vector = new int[n];
		vector = getRowVec(i);
		for(int j=0; j<m; j++){
			cout << vector[j] << " ";
		}
		cout << endl;
	}

	cout << "rowPtr:" ;
	for(int i=0; i<n; i++){
		cout << " " << rowPtr[i];
	}
	cout << endl;

	cout << "colPos:";
	for (int i =0; i< nonZeros; i++){
		cout << " " << colPos[i];
	}
	cout << endl;

	cout << "values:";
	for (int i =0; i< nonZeros; i++){
		cout << " " <<values[i];
	}
	cout << endl;

}

int* CSR::getRowVec(int row){
	int *vector = new int[n];
	for(int i=0; i<n; i++)
		vector[i] = 0;

	if(row < n-1){
		for(int i= rowPtr[row]; i< rowPtr[row +1]; i++){
			for(int j=0; j< m; j++){
				if(colPos[i] == j)
					vector[j] = values[i];
			}
		}
	}
	else{
		for(int i= rowPtr[row]; i<nonZeros; i++){
			for (int j=0; j< m; j++){
				if (colPos[i] == j)
					vector[j] = values[i];
			}
		}

	}
	return vector;
}

int* CSR::getColVec(int col){
	int* colVector = new int[n];
	int r;
	for(int i=0; i<n; i++)
		colVector[i] = 0;
	bool found;
	int k, j;

	k = 0;
	for(int i=0; i< n-1; i++){
		r = rowPtr[i+1] - rowPtr[i];
		k = rowPtr[i];
		found = false;
		j=0;
		while((j<r) && !found){
			if(colPos[k] == col){
				found = true;
				colVector[i] = values[k];
			}
			k++;
			j++;
		}
	}

	int p = rowPtr[n-1];
	found = false;
	while((p < nonZeros) && !found){
		if(colPos[p] == col){
			colVector[n-1] = values[p];
			found = true;
		}
		else
			p++;
	}
	return colVector;
}

// matrix-vector multiplication
int* CSR::matrixVectorMultiply(int* inputVector){
	int* outputVector = new int[n];

	for(int i=0; i<n; i++)
		outputVector[i] = 0;

	int start, end;
	for(int i=0; i<n; i++){
		start = rowPtr[i];
		if((i+1) == n)
			end = nonZeros;
		else
			end = rowPtr[i+1];

		for(int j = start; j<end; j++)
			outputVector[i] = outputVector[i] + (values[j]*inputVector[colPos[j]]);
	}
	return outputVector;

}

// matrix-matrix multiplication method
CSR* CSR::matrixMultiply(CSR& matrixB){
	int maxNonZeros = n*m;

	int* vecRow = new int[n];
	int* vecCol = new int[matrixB.m];
	for(int i=0; i<n; i++)
		vecRow[i] = 0;
	for(int i=0; i<matrixB.m; i++)
		vecCol[i] = 0;

	CSR* resultMatrix = new CSR(n, matrixB.m, maxNonZeros);
	for(int i=0; i<n; i++){
		vecRow = getRowVec(i);
		for(int j=0; j<matrixB.m; j++){
			int value = 0;
			vecCol = getColVec(j);
			for(int k=0; k< m; k++){
				value = value + (vecRow[k]*vecCol[k]);
			}

			if(value != 0){
				(*resultMatrix).addValue(value);
				(*resultMatrix).addRow(i);
				(*resultMatrix).addColumn(j);
			}
		}
	}

	return resultMatrix;
}

//destructors method
CSR::~CSR(){
	if (values != NULL) delete [] values;
	if (rowPtr != NULL) delete [] rowPtr;
	if (colPos != NULL) delete [] colPos;
	cout << "CSR Object Destroyed!!" << endl;
	n = 0;
	m = 0;
	nonZeros = 0;
	indexOfValue = 0;
	indexOfCol = 0;
}

CSR& CSR::operator =(CSR& matrixB){
	n = matrixB.getNumRows();
	return *this;
}

int main(){
	CSR* A;
	CSR* B;
	int* aVector;
	int numRows, numColumns, numNonZeros;
	int row, col, value;

	// read in the first matrix
	cin >> numRows >> numColumns;
	cin >> numNonZeros;

	// call the constructor
	A = new CSR(numRows, numColumns, numNonZeros);
	for(int i=0; i<numNonZeros; i++){
		//read in first sparse matrix representation
		cin >> row >> col >> value;
		(*A).addValue(value);
		(*A).addRow(row);
		(*A).addColumn(col);
	}
	cout << "Matrix A:" << endl;
	(*A).display();

	//copy constructor
	CSR*C = new CSR(*A);
	cout << "Copied Matrix C:"<< endl;
	(*C).display();

	//read in the second matrix
	cin >> numRows >> numColumns;
	cin >> numNonZeros;

	//call constructor
	B = new CSR(numRows, numColumns, numNonZeros);
	for(int i=0; i<numNonZeros; i++){
		//read in second sparse matrix representation
		cin >> row >> col >> value;
		(*B).addValue(value);
		(*B).addRow(row);
		(*B).addColumn(col);
	}
	cout << "Matrix B:" << endl;
	(*B).display();

	// read in vector
	cin >> numColumns;
	aVector = new int[numColumns];
	for(int i=0; i<numColumns; i++)
		cin >> aVector[i];

	cout << "Vector:" << endl;
	for(int i=0; i<numColumns; i++)
		cout << aVector[i] << " ";
	cout << endl;
	cout << "A*vector:" << endl;

	// matrix-vector multiplication
	int* resultVector =(*A).matrixVectorMultiply(aVector);
	for(int i=0; i< (*A).getNumRows(); i++)
		cout << resultVector[i] << " ";
	cout << endl;

	// matrix-matrix multiplication
	cout << "A*B:" << endl;
	CSR*resultMatrix = (*C).matrixMultiply(*B);
	(*resultMatrix).display();

	// call the destructors
	delete [] aVector;
	delete [] resultVector;
	delete A;
	delete B;
	delete C;
	delete resultMatrix;

	return 0;

}



