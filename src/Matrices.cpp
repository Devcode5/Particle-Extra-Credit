
// src/Matrices.cpp
#include "Matrices.h"
#include <stdexcept>
#include <cmath>

using namespace Matrices;
using namespace std;

Matrix::Matrix(int _rows, int _cols) : rows(_rows), cols(_cols) {
    a.assign(rows, vector<double>(cols, 0.0));
}

// operator+ (elementwise)
Matrix Matrices::operator+(const Matrix& A, const Matrix& B) {
    if (A.getRows() != B.getRows() || A.getCols() != B.getCols())
        throw runtime_error("Matrix dimensions must match for +");
    Matrix C(A.getRows(), A.getCols());
    for (int i = 0; i < A.getRows(); ++i)
        for (int j = 0; j < A.getCols(); ++j)
            C(i,j) = A(i,j) + B(i,j);
    return C;
}

// operator* (matrix multiply)
Matrix Matrices::operator*(const Matrix& A, const Matrix& B) {
    if (A.getCols() != B.getRows())
        throw runtime_error("Matrix multiply dimension mismatch");
    Matrix C(A.getRows(), B.getCols());
    for (int i = 0; i < A.getRows(); ++i) {
        for (int j = 0; j < B.getCols(); ++j) {
            double sum = 0.0;
            for (int k = 0; k < A.getCols(); ++k)
                sum += A(i,k) * B(k,j);
            C(i,j) = sum;
        }
    }
    return C;
}


bool Matrices::operator==(const Matrix& A, const Matrix& B) {
    if (A.getRows() != B.getRows() || A.getCols() != B.getCols()) return false;
    const double eps = 1e-6;
    for (int i=0;i<A.getRows();++i)
        for (int j=0;j<A.getCols();++j)
            if (fabs(A(i,j)-B(i,j))>eps) return false;
    return true;
}
bool Matrices::operator!=(const Matrix& A, const Matrix& B) { return !(A==B); }

ostream& Matrices::operator<<(ostream& os, const Matrix& A) {
    for (int i=0;i<A.getRows();++i) {
        for (int j=0;j<A.getCols();++j) {
            os << A(i,j);
            if (j+1 < A.getCols()) os << ' ';
        }
        if (i+1 < A.getRows()) os << '\n';
    }
    return os;
}

// RotationMatrix constructor
RotationMatrix::RotationMatrix(double theta) : Matrix(2,2) {
    (*this)(0,0) = cos(theta);
    (*this)(0,1) = -sin(theta);
    (*this)(1,0) = sin(theta);
    (*this)(1,1) = cos(theta);
}

// ScalingMatrix constructor
ScalingMatrix::ScalingMatrix(double scale) : Matrix(2,2) {
    (*this)(0,0) = scale; (*this)(0,1)=0;
    (*this)(1,0) = 0;     (*this)(1,1)=scale;
}

// TranslationMatrix constructor: 2 x nCols, each column is (xShift, yShift)
TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2, nCols) {
    for (int j=0;j<nCols;++j) {
        (*this)(0,j) = xShift;
        (*this)(1,j) = yShift;
    }
}
