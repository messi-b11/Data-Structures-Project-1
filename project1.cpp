#include <iostream>
using namespace std;

class SparseRow {

        protected:
            int row; //Row#
            int col; //Column#
            int value; //We will assume that all our values will be integers
        
        public:
            SparseRow (); //default constructor; row=-1;col=-1;value=0

            void display(); // print Row#, Column#, value
            friend std::ostream& operator<< (std::ostream& s, const SparseRow& sr); //added the & sr on the const SparseRow& sr to make it consise with the method definition

            //other methods that are necessary such as get and set
            int getRow() const { return row; }
            int getCol() const { return col; }
            int getValue() const { return value; }
            void setValue(int v) {value = v; }
            void setPosition(int r, int c) { row = r; col = c; }
};


class SparseMatrix {
        
        protected:
            int noRows; //Number of rows of the original matrix
            int noCols; //Number of columns of the original matrix
            int commonValue; //read from input
            int noNonSparseValues;
            SparseRow* myMatrix; //Array
        
        public:
            SparseMatrix ();
            SparseMatrix (int n, int m, int cv, int noNSV);
            ~SparseMatrix();

            SparseMatrix* Transpose (); //Matrix Transpose
            SparseMatrix* Multiply(SparseMatrix& M);
            SparseMatrix* Add(SparseMatrix& M);
            friend std::ostream& operator<< (std::ostream& s, const SparseMatrix& sm);
            void displayMatrix (); //Display the matrix in its original format
            void setElement(int r, int c, int val); //other methods that are necessary such as get and set
            SparseRow getElement (int r, int c) const; // gets an element from the matrix


};

//define all your classes here (as given above)

SparseRow::SparseRow(){
    row = -1;
    col = -1;
    value = 0;

}

void SparseRow::display() {
    std::cout << "Row#: " << row << ", col#: " <<  col << ", value: " << value << std::endl;
}

ostream& operator<<(ostream& s, const SparseRow& sr) {
    s << "Row: " << sr.getRow() << ", Column: " << sr.getCol() << ", Value: " << sr.getValue();
    return s;
}

SparseMatrix::~SparseMatrix() {
    delete[] myMatrix;
}

SparseMatrix::SparseMatrix() {
    noRows = 0;
    noCols = 0;
    commonValue = 0;
    noNonSparseValues = 0;
    myMatrix = nullptr;
}

SparseMatrix::SparseMatrix(int  n, int m, int cv, int noNSV) {
    noRows = n;
    noCols = m;
    commonValue = cv; //this should read form the input file and therefore not cv
    noNonSparseValues = noNSV; 
    myMatrix = new SparseRow[noNonSparseValues]; //switched noRows with noNonSparseValues
    for  (int i = 0; i < noNonSparseValues; i++) {
        //myMatrix[i] = SparseRow();
        myMatrix[i].setPosition(-1,-1); // Initialize row and column to -1
        myMatrix[i].setValue(0); //initialize value to 0
    }

 }

SparseRow SparseMatrix::getElement(int r, int c) const {
    for  (int i = 0; i < noNonSparseValues; i++) { //switched the noRows with noNonSparseValues to loop only through non-sparse values 
        if (myMatrix[i].getRow() == r && myMatrix[i].getCol() == c) {
            return myMatrix[i];
        }
    }

    SparseRow defaultElement;
    defaultElement.setValue(commonValue);
    return defaultElement;

}


//new set element function
void SparseMatrix::setElement(int r, int c, int val) {
    for (int i = 0; i < noNonSparseValues; ++i) {
        if (myMatrix[i].getRow() == -1 && myMatrix[i].getCol() == -1) { // Find the next available spot
            myMatrix[i].setPosition(r, c);
            myMatrix[i].setValue(val);
            return;
        }
    }
    /*for (int i = 0; i < noNonSparseValues; i++) {
        if (myMatrix[i].getRow() == -1 && myMatrix[i].getCol() == -1) {
            myMatrix[i].setPosition(r, c);
            myMatrix[i].setValue(val);
            return;
        }
    }*/
}

SparseMatrix* SparseMatrix::Transpose() {
    SparseMatrix* transpose = new SparseMatrix(noCols, noRows, commonValue, noNonSparseValues);
    for (int i = 0; i < noNonSparseValues; i++) {
        transpose->setElement(myMatrix[i].getCol(), myMatrix[i].getRow(), myMatrix[i].getValue());
    }
    return transpose;

}


 SparseMatrix* SparseMatrix::Multiply(SparseMatrix& M) {
    if (noCols != M.noRows) {
        throw std::invalid_argument("Matrix dimensions are incompatible for multiplication");
    }

    SparseMatrix* result = new SparseMatrix(noRows, M.noCols, commonValue, noNonSparseValues);
    for (int i = 0; i < noRows; i++) {
        for (int j = 0; j < M.noCols; j++) {
            int sum = 0;
            for (int k = 0; k < noCols; k++) {
                sum += getElement(i,k).getValue() * M.getElement(k,j).getValue();
            }
            if (sum != commonValue) {
                result->setElement(i,j,sum);
            }
        }
    }
    return result;
}

SparseMatrix* SparseMatrix::Add(SparseMatrix& M) {
    if (noRows != M.noRows || noCols != M.noCols) {
        throw std::invalid_argument("The matrices must be of the same size");

    }

    SparseMatrix* result = new SparseMatrix(noRows, noCols, commonValue, noNonSparseValues+ M.noNonSparseValues);

    for (int i = 0; i < noRows; i++) {
        for  (int j = 0; j < noCols; j++) {
        //int sum = getElement(myMatrix[i].getRow(), myMatrix[i].getCol()).getValue() + M.getElement(myMatrix[i].getRow(), myMatrix[i].getCol()).getValue();
        int sum = getElement(i,j).getValue() + M.getElement(i,j).getValue();
        if (sum != commonValue) {
             result->setElement(i,j,sum);//result->setElement(myMatrix[i].getRow(), myMatrix[i].getCol(), sum);
            }
        }
    }
    return result;
}

/*void SparseMatrix::displayMatrix() {
    for (int i = 0; i < noRows; i++) {
        for (int j = 0; j < noCols; j++) {
           int value = getElement(i,j).getValue();
            std::cout << ( value != commonValue? value: 0 )<< " ";
        }
        
        std::cout<< endl;
    }
}
*/

void SparseMatrix::displayMatrix() {
    for (int i = 0; i < noRows; i++) {
        for (int j = 0; j < noCols; j++) {
            int found = 0;
            for (int k = 0; k < noNonSparseValues; k++) {
                if (myMatrix[k].getRow() == i && myMatrix[k].getCol() == j) {
                    std::cout << myMatrix[k].getValue() << " ";
                    found = 1;
                    break;
                }
            }
            if (!found) {
                std::cout << commonValue << " ";
            }
        }
        std::cout << std::endl;
    }
}

/*
std::ostream& operator<<(std::ostream& s, const SparseMatrix& sm) {
    // We iterate over each row and column in the matrix.
    for (int i = 0; i < sm.noRows; i++) {
        for (int j = 0; j < sm.noCols; j++) {
            // If the value at the current position is not the common value, we print it.
            int value = sm.getElement(i, j).getValue();
            s << (value != sm.commonValue ? value : 0) << " ";
        }
        s << endl;
    }
    return s;
}
*/

/*std::ostream& operator<<(std::ostream& s, const SparseMatrix& sm) {
    for (int i = 0; i < sm.noNonSparseValues; i++) {
        s << "Row: " << sm.myMatrix[i].getRow() << ", Column: " << sm.myMatrix[i].getCol() << ", Value: " << sm.myMatrix[i].getValue() << std::endl;
    }
    return s;
}*/

std::ostream& operator<<(std::ostream& s, const SparseMatrix& sm) {
    for (int i = 0; i < sm.noRows; i++) {
        for (int j = 0; j < sm.noCols; j++) {
            int found = 0;
            for (int k = 0; k < sm.noNonSparseValues; k++) {
                if (sm.myMatrix[k].getRow() == i && sm.myMatrix[k].getCol() == j) {
                    s << sm.myMatrix[k].getValue() << " ";
                    found = 1;
                    break;
                }
            }
            if (!found) {
                s << sm.commonValue << " ";
            }
        }
        s << std::endl;
    }
    return s;
}

//write the methods after the class definition
int main () {

     int n, m, cv, noNSV;
    SparseMatrix* temp;
    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV);

    // Read in the non-sparse values for the first matrix
    for (int i = 0; i < noNSV; i++) {
        int row, col, value;
        cin >> row >> col >> value;
        firstOne->setElement(row, col, value);
    }

    cin >> n >> m >> cv >> noNSV; // read in the sizes for the second matrix
    SparseMatrix* secondOne = new SparseMatrix(n, m, cv, noNSV); // create the second matrix with the correct sizes

    // Read in the non-sparse values for the second matrix
    for (int i = 0; i < noNSV; i++) {
        int row, col, value;
        cin >> row >> col >> value;
        secondOne->setElement(row, col, value);
    }

    //Write the Statements to read in the second matrix
    cout << "First one in sparse matrix format" << endl;
    cout << (*firstOne);

    cout <<"After transpose" << endl;
    cout << (*(*firstOne).Transpose());

    std::cout << "First one in matrix format" << endl;
    (*firstOne).displayMatrix();

    cout << "Second one in sparse matrix format" << endl;
    cout << (*secondOne);

    //cout << "Transpose of the first one in matrix" << endl;
    //cout << (*(*firstOne).Transpose());

    cout <<"After transpose" << endl;
    cout << (*(*secondOne).Transpose());

    cout << "Second one in matrix format" << endl;
    (*secondOne).displayMatrix();
    
    
    cout << "Matrix addition result" << endl;
    temp = firstOne->Add(*secondOne);
    cout << temp;
    (*temp).displayMatrix();

    cout << "Matrix Multiplication Result" << endl;
    temp = firstOne->Multiply( *secondOne);
    cout << temp;
    (*temp).displayMatrix();

    delete firstOne;
    delete secondOne;
    delete temp;
    //delete transpose;

    return 0;
}
