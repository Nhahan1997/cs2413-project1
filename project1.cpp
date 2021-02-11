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
			int* colPos;	//array contain the position of nonZeros in each column
	public:
			CSR();	//default constructor
			CSR(CSR& matrixB);	//copy constructor
			CSR(int rows, int cols, int numNonZeros);	//non-defalut constructor
			int getNumRows();	// return number of row n
			int getNumCols();	//accessor of n
			int getNonZeros();	//accessor of m
			int* getValues();	//accessor of value
			int* getRowPtr();	//accessor of rowPtr
			int* getColPos();	//accessor of colPos
			void addValue(int value);	//add a new value in the values array
			void addColumn(int col);	//add a column in the colPos array
			void addRow(int row);		//add a row in the rowPtr array
			void display();				//print the content of three arrays, its matrix.Each array must be on a different line
			int* getRowVec(int row);	//fetch the vector at specific row of matrix
			int* getColVec(int col);	//fetch the vector at specific column of matrix
			int* matrixVectorMultiply(int* inputVector);	//matrix-vector multiplication
			CSR* matrixMultiply(CSR& matrixB);	//matrix-matrix multipication
			~CSR();		//destructure
			CSR& operator = (CSR& matrixB); 	// overload operation = for deep copy

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

//Constructor
CSR::CSR(int rows, int cols, int numNonZeros){
	n = rows;	//assign parameter rows to n
	m = cols;	//assign parameter cols to m
	nonZeros = numNonZeros;	//assign parameter numNonZeros = nonZeros
	indexOfValue =0;
	indexOfCol =0;
	values = new int[nonZeros];	//create new array values with size equal nonZeros
	colPos = new int[nonZeros];	//create new array colPos with size equal nonZeros
	rowPtr = new int[n];		//create new array rowPtr with size equal n (number of row)
}

//copy constructor
CSR::CSR(CSR & matrixB){

	n = matrixB.getNumRows(); 	//assign n equal to number of row of matrixB
	m = matrixB.getNumCols();	//assign m equal to number of column of matrixB
	nonZeros = matrixB.getNonZeros(); //assign nonZeros equal to number of nonZeros of matrixB
	indexOfValue =0;
	indexOfCol =0;
	values = matrixB.getValues();	//assign array value now is equal to array values of matrixB
	colPos = matrixB.getColPos();	//assign array colPos now is equal to array colPos of matrixB
	rowPtr = matrixB.getRowPtr();	//assign array rowPtr now is equal to array rowPtr of matrixB
}

int CSR::getNumRows(){	//accessor
	return n;
}
int CSR::getNumCols(){	//accessor
	return m;
}
int CSR::getNonZeros(){	//accessor
	return nonZeros;
}
int* CSR::getRowPtr(){	//accessor
	return rowPtr;
}
int* CSR::getValues(){	//accessor
	return values;
}
int* CSR::getColPos(){	//accessor
	return colPos;
}

// add new value to array values. "indexOfValue" is variable to
// keep track index of values. It will increment whenever this method is called
void CSR::addValue(int value){
	values[indexOfValue++] = value;
}

// add new col to array colPos. "indexOfCol" is variable to
// keep track index of values. It will increment whenever this method is called
void CSR::addColumn(int col){
	colPos[indexOfCol++] = col;
}

// add new row to array rowPtr. In this method, value of rowPtr is where the first
// non-zeros in that row start. Its value is also the index of colPos and values
// However, the value of rowPtr is also total number of non-zeros before it. If
// rowPtr[1] = 3 that mean total non-zeros in row 0 is 3.
void CSR::addRow(int row){
	if(rowPtr[row+1] == 0){
		rowPtr[row+1] = rowPtr[row]+1;
	}
	else
		rowPtr[row+1]++;
}

// print the matrix and the three arrays of that metrix which is values, rowPtr, colPos
void CSR::display(){
	//print the matrix by using getRowVecc()
	for(int i=0; i< n; i++){		//loop in range number of rows.
		int* vector = new int[n];	//creat new array with size is number of row.
		vector = getRowVec(i);		//assign the vector returned and store in array vector.
		for(int j=0; j<m; j++){		//loop each column to print each element in array vector.
			cout << vector[j] << " ";
		}
		cout << endl;
	}
	//print the array rowPtr
	cout << "rowPtr:" ;
	for(int i=0; i<n; i++){
		cout << " " << rowPtr[i];
	}
	cout << endl;
	//print array colPos
	cout << "colPos:";
	for (int i =0; i< nonZeros; i++){
		cout << " " << colPos[i];
	}
	cout << endl;
	//print array values
	cout << "values:";
	for (int i =0; i< nonZeros; i++){
		cout << " " <<values[i];
	}
	cout << endl;

}

// method help to fetch a row of values from the matrix. Row number is parameter
// a vector of expected row would store in a new array and return
int* CSR::getRowVec(int row){
	int *vector = new int[n];
	for(int i=0; i<n; i++)	//fill the array with all zeros
		vector[i] = 0;

	if(row < n-1){		//check if row is last index or not
		// variable i start at value of rowPtr at index is row
		// Loop unitl value of rowPtr at index row+1
		for(int i= rowPtr[row]; i< rowPtr[row +1]; i++){
			for(int j=0; j< m; j++){ 		//loop the column
				if(colPos[i] == j)			//check if colPos at index i equal j,
					vector[j] = values[i];	//if yes, add values at i to vector
			}								//this step is add non-zeros to expected position
		}
	}
	//else mean row is the last row
	else{
		//now it loop utill last non-zeros element. And same idea as above
		for(int i= rowPtr[row]; i<nonZeros; i++){
			for (int j=0; j< m; j++){
				if (colPos[i] == j)
					vector[j] = values[i];
			}
		}

	}
	return vector;
}

// method help to fetch a column of values from the matrix. col number is parameter
// a vector of expected col would store in a new array and return
int* CSR::getColVec(int col){
	//create new array colVector size equal n
	int* colVector = new int[n];
	int r;

	for(int i=0; i<n; i++)		// fill array with all zeross
		colVector[i] = 0;
	bool found;					// variable to keep track if non-zero is found
	int k, j;

	k = 0;
	// looping until the index before last index of rowPtr
	for(int i=0; i< n-1; i++){
		r = rowPtr[i+1] - rowPtr[i]; 		//the different between next row and current row
											//the different tell number of non-zeros in current row
		k = rowPtr[i];
		found = false;
		j=0;
		while((j<r) && !found){
			if(colPos[k] == col){			//if the non-zeros is found at index k
				found = true;
				colVector[i] = values[k]; 	//add values at k to colVector
			}
			k++;							//else increment k and j
			j++;
		}
	}
	//checking last index of rowPtr
	//assign it to p
	int p = rowPtr[n-1];
	found = false;
	while((p < nonZeros) && !found){
		if(colPos[p] == col){
			colVector[n-1] = values[p]; 	//if non-zeros is found add it value to colVector
			found = true;
		}
		else
			p++;							//else increment p
	}
	return colVector;
}

// matrix-vector multiplication. Return a new vector
int* CSR::matrixVectorMultiply(int* inputVector){
	int* outputVector = new int[n];	//create new array
	//fill array with zeros
	for(int i = 0; i < n; i++)
		outputVector[i] = 0;

	int start, end;
	for(int i = 0; i < n; i++){
		start = rowPtr[i];		//start at first row
		if((i+1) == n)
			end = nonZeros;
		else
			end = rowPtr[i+1];

		for(int j = start; j < end; j++)
			outputVector[i] = outputVector[i] + (values[j]*inputVector[colPos[j]]);
	}
	return outputVector;

}

// matrix-matrix multiplication method. Return a new matrix in type
// of sparse representation which is values, rowPtr, colPos
CSR* CSR::matrixMultiply(CSR& matrixB){
	int maxNonZeros = n*m;					//max non-zeros element in new matrix

	int* vecRow = new int[getNumRows()];	//create two array
	int* vecCol = new int[matrixB.getNumCols()];

	//copy constructor with parameter are number of row, column of matrixB
	// and maxNonZeros
	CSR* resultMatrix = new CSR(n, matrixB.getNumCols(), maxNonZeros);
	for(int i = 0; i < n; i++){
		vecRow = getRowVec(i);				//get the row vector
		for(int j = 0; j < matrixB.getNumCols(); j++){
			int value = 0;
			vecCol = matrixB.getColVec(j);	//get the column vector
			for(int k = 0; k < m; k++){
				value = value + (vecRow[k]*vecCol[k]); //multiply the each element of row with column an add them up
			}
			if(value != 0){					//if value is non-zeros, add them to values, rowPtr, colPos
				(*resultMatrix).addValue(value);
				(*resultMatrix).addRow(i);
				(*resultMatrix).addColumn(j);
			}
		}
	}

	return resultMatrix;
}

//destructors method. Delete the pointer data
// and set all static data to default
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

//operator = for deep copy for copy constructor
CSR& CSR::operator =(CSR& matrixB){
	n = matrixB.getNumRows();
	m = matrixB.getNumCols();
	nonZeros = matrixB.getNonZeros();
	values = new int(*(matrixB.getValues()));
	rowPtr = new int(*(matrixB.getRowPtr()));
	colPos = new int(*(matrixB.getColPos()));
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
	//CSR*C = new CSR(*A);
	//CSR*C=(*A);
	CSR*C = A;
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
	//print the vector
	cout << "Vector:" << endl;
	for(int i=0; i<numColumns; i++)
		cout << aVector[i] << " ";
	cout << endl;
	cout << "A*vector:" << endl;

	// matrix-vector multiplication. Call the matrixVectorMultiply and print result
	int* resultVector =(*A).matrixVectorMultiply(aVector);
	for(int i=0; i< (*A).getNumRows(); i++)
		cout << resultVector[i] << " ";
	cout << endl;

	//call matrix-matrix multiplication and display
	cout << "A*B:" << endl;
	CSR*resultMatrix = (*C).matrixMultiply(*B);
	(*resultMatrix).display();

	// delete all the pointer data and object
	delete [] aVector;
	delete [] resultVector;
	delete A;
	delete B;
	delete C;
	delete resultMatrix;

	return 0;

}



