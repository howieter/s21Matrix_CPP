#include "s21_matrix_oop.hpp"

namespace S21 {
Matrix::Matrix() noexcept : rows_{0}, cols_{0}, matrix_{nullptr} {};

Matrix::Matrix(const int &newRow, const int &newCol)
    : rows_{newRow}, cols_{newCol} {
  InitializeMatrix();
}

Matrix::Matrix(const Matrix &other) noexcept
    : rows_{0}, cols_{0}, matrix_{nullptr} {
  CopyMatrix(other);
}

Matrix::Matrix(Matrix &&other) noexcept : rows_{0}, cols_{0}, matrix_{nullptr} {
  CopyMatrix(other);
  other.DeleteMatrix();
}

Matrix::~Matrix() { DeleteMatrix(); }

int Matrix::GetRows() const { return rows_; }

int Matrix::GetCols() const { return cols_; }

double **Matrix::GetMatrix() const { return matrix_; }

void Matrix::SetRows(const int &newRows) {
  if (newRows < 0) {
    throw std::invalid_argument("Rows is less than zero");
  }
  Matrix newMatrix(newRows, cols_);
  for (int i = 0; i < newRows && i < rows_; ++i) {
    for (int k = 0; k < cols_; ++k) {
      newMatrix(i, k) = matrix_[i][k];
    }
  }
  DeleteMatrix();
  rows_ = newRows;
  *this = newMatrix;
}

void Matrix::SetCols(const int &newCols) {
  if (newCols < 0) {
    throw std::invalid_argument("Columns is less than zero");
  }
  Matrix newMatrix(rows_, newCols);
  for (int i = 0; i < rows_; ++i) {
    for (int k = 0; k < cols_ && k < newCols; ++k) {
      newMatrix(i, k) = matrix_[i][k];
    }
  }
  DeleteMatrix();
  rows_ = newCols;
  *this = newMatrix;
}

void Matrix::SetMatrix(double **newMatrix, const int &row, const int &col) {
  if (rows_ != row) {
    SetRows(row);
  }
  if (cols_ != col) {
    SetCols(col);
  }
  for (int i = 0; i < row; ++i) {
    for (int k = 0; k < col; ++k) {
      matrix_[i][k] = newMatrix[i][k];
    }
  }
}

Matrix Matrix::operator+(const Matrix &other) const {
  Matrix sum(*this);
  sum.SumMatrix(other);
  return sum;
}

Matrix Matrix::operator-(const Matrix &other) const {
  Matrix sub(*this);
  sub.SubMatrix(other);
  return sub;
}

Matrix Matrix::operator*(const Matrix &other) const {
  Matrix mulM(*this);
  mulM.MulMatrix(other);
  return mulM;
}

Matrix operator*(const Matrix &other, const double &num) {
  Matrix mulN(other);
  mulN.MulNumber(num);
  return mulN;
}

Matrix operator*(const double &num, const Matrix &other) {
  Matrix mulN(other);
  mulN.MulNumber(num);
  return mulN;
}

Matrix &Matrix::operator+=(const Matrix &other) {
  SumMatrix(other);
  return *this;
}

Matrix &Matrix::operator-=(const Matrix &other) {
  SubMatrix(other);
  return *this;
}

Matrix &Matrix::operator*=(const Matrix &other) {
  MulMatrix(other);
  return *this;
}

Matrix &Matrix::operator*=(const double &num) {
  MulNumber(num);
  return *this;
}

Matrix &Matrix::operator=(const Matrix &other) {
  CopyMatrix(other);
  return *this;
}

bool Matrix::operator==(const Matrix &other) const { return EqMatrix(other); }

double &Matrix::operator()(const int &i, const int &j) const {
  if (rows_ == 0 && cols_ == 0) {
    throw std::out_of_range("Matrix is empty");
  }
  if (i < 0 || j < 0 || i >= rows_ || j >= cols_) {
    throw std::out_of_range("Index less or grater than matrix size");
  }
  return matrix_[i][j];
}

bool Matrix::EqMatrix(const Matrix &other) const {
  if (SizeCompare(other)) {
    for (int i = 0; i < rows_; ++i) {
      for (int k = 0; k < cols_; ++k) {
        if (fabsl(matrix_[i][k] - other.matrix_[i][k]) >= 1e-7) {
          return false;
        }
      }
    }
  } else {
    return false;
  }
  return true;
}

void Matrix::SumMatrix(const Matrix &other) { PlusMinus(other, 1); }

void Matrix::SubMatrix(const Matrix &other) { PlusMinus(other, -1); }

void Matrix::MulNumber(const double &num) {
  for (int i = 0; i < rows_; ++i) {
    for (int k = 0; k < cols_; ++k) {
      matrix_[i][k] = matrix_[i][k] * num;
    }
  }
}

void Matrix::MulMatrix(const Matrix &other) {
  if (cols_ != other.rows_) {
    std::out_of_range("Columns of matrix_1 not equal to Rows of matrix_2");
  }
  if (cols_ <= 0 || other.cols_ <= 0 || rows_ <= 0 || other.rows_ <= 0) {
    throw std::invalid_argument(
        "Some columns or some rows equal or less to zero");
  }
  Matrix newMatrix(rows_, other.cols_);
  for (int m = 0; m < rows_; ++m) {
    for (int n = 0; n < other.cols_; ++n) {
      for (int k = 0; k < other.rows_; ++k) {
        newMatrix.matrix_[m][n] += matrix_[m][k] * other.matrix_[k][n];
      }
    }
  }
  DeleteMatrix();
  *this = newMatrix;
}

Matrix Matrix::Transpose() {
  Matrix newMatrix(cols_, rows_);
  for (int i = 0; i < cols_; ++i) {
    for (int k = 0; k < rows_; ++k) {
      newMatrix.matrix_[i][k] = matrix_[k][i];
    }
  }
  return newMatrix;
}

double Matrix::Determinant() {
  if (rows_ != cols_ || rows_ == 0) {
    throw std::out_of_range("Matrix is not square");
  }
  double det = 0;
  if (rows_ == 1) {
    return matrix_[0][0];
  } else if (rows_ == 2) {
    return matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
  } else {
    Matrix newMatrix(rows_ - 1, cols_ - 1);
    for (int i = 0; i < rows_; ++i) {
      CutMatrix(*this, i, 0, newMatrix);
      det += newMatrix.Determinant() * matrix_[i][0] * ((i % 2 == 1) ? -1 : 1);
    }
    newMatrix.DeleteMatrix();
  }
  return det;
}

Matrix Matrix::CalcComplements() {
  Matrix minor(rows_, cols_);
  if (rows_ == cols_) {
    Matrix cut(rows_ - 1, cols_ - 1);
    for (int i = 0; i < rows_; ++i) {
      for (int k = 0; k < cols_; ++k) {
        CutMatrix(*this, i, k, cut);
        minor.matrix_[i][k] = cut.Determinant() * pow(-1, i + k);
      }
      cout << endl;
    }
    cut.DeleteMatrix();
  } else {
    throw std::out_of_range("Matrix is not square");
  }
  return minor;
}

Matrix Matrix::InverseMatrix() {
  Matrix res(rows_, cols_);
  double determinant = Determinant();
  if (determinant == 0 || fabs(determinant) <= 1e-7) {
    throw std::invalid_argument("Calculation error");
  } else {
    res = CalcComplements().Transpose();
    res.MulNumber(1 / determinant);
  }
  return res;
}
}  // namespace S21