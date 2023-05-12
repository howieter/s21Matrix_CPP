#include "s21_matrix_oop.hpp"

namespace S21 {
S21Matrix::S21Matrix() noexcept : rows_{0}, cols_{0}, matrix_{nullptr} {};

S21Matrix::S21Matrix(const int &newRow, const int &newCol)
    : rows_{newRow}, cols_{newCol} {
  InitializeMatrix();
}

S21Matrix::S21Matrix(const S21Matrix &other) noexcept
    : rows_{0}, cols_{0}, matrix_{nullptr} {
  CopyMatrix(other);
}

S21Matrix::S21Matrix(S21Matrix &&other) noexcept
    : rows_{0}, cols_{0}, matrix_{nullptr} {
  CopyMatrix(other);
  other.DeleteMatrix();
}

S21Matrix::~S21Matrix() { DeleteMatrix(); }

int S21Matrix::GetRows() const { return rows_; }

int S21Matrix::GetCols() const { return cols_; }

double **S21Matrix::GetMatrix() const { return matrix_; }

void S21Matrix::SetRows(const int &newRows) {
  if (newRows < 0) {
    throw std::invalid_argument("Rows is less than zero");
  }
  S21Matrix newMatrix(newRows, cols_);
  for (int i = 0; i < newRows && i < rows_; ++i) {
    for (int k = 0; k < cols_; ++k) {
      newMatrix(i, k) = matrix_[i][k];
    }
  }
  DeleteMatrix();
  rows_ = newRows;
  *this = newMatrix;
}

void S21Matrix::SetCols(const int &newCols) {
  if (newCols < 0) {
    throw std::invalid_argument("Columns is less than zero");
  }
  S21Matrix newMatrix(rows_, newCols);
  for (int i = 0; i < rows_; ++i) {
    for (int k = 0; k < cols_ && k < newCols; ++k) {
      newMatrix(i, k) = matrix_[i][k];
    }
  }
  DeleteMatrix();
  rows_ = newCols;
  *this = newMatrix;
}

void S21Matrix::SetMatrix(double **newMatrix, const int &row, const int &col) {
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

S21Matrix S21Matrix::operator+(const S21Matrix &other) const {
  S21Matrix sum(*this);
  sum.SumMatrix(other);
  return sum;
}

S21Matrix S21Matrix::operator-(const S21Matrix &other) const {
  S21Matrix sub(*this);
  sub.SubMatrix(other);
  return sub;
}

S21Matrix S21Matrix::operator*(const S21Matrix &other) const {
  S21Matrix mulM(*this);
  mulM.MulMatrix(other);
  return mulM;
}

S21Matrix operator*(const S21Matrix &other, const double &num) {
  S21Matrix mulN(other);
  mulN.MulNumber(num);
  return mulN;
}

S21Matrix operator*(const double &num, const S21Matrix &other) {
  S21Matrix mulN(other);
  mulN.MulNumber(num);
  return mulN;
}

S21Matrix &S21Matrix::operator+=(const S21Matrix &other) {
  SumMatrix(other);
  return *this;
}

S21Matrix &S21Matrix::operator-=(const S21Matrix &other) {
  SubMatrix(other);
  return *this;
}

S21Matrix &S21Matrix::operator*=(const S21Matrix &other) {
  MulMatrix(other);
  return *this;
}

S21Matrix &S21Matrix::operator*=(const double &num) {
  MulNumber(num);
  return *this;
}

S21Matrix &S21Matrix::operator=(const S21Matrix &other) {
  CopyMatrix(other);
  return *this;
}

bool S21Matrix::operator==(const S21Matrix &other) const {
  return EqMatrix(other);
}

double &S21Matrix::operator()(const int &i, const int &j) const {
  if (rows_ == 0 && cols_ == 0) {
    throw std::out_of_range("Matrix is empty");
  }
  if (i < 0 || j < 0 || i >= rows_ || j >= cols_) {
    throw std::out_of_range("Index less or grater than matrix size");
  }
  return matrix_[i][j];
}

bool S21Matrix::EqMatrix(const S21Matrix &other) const {
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

void S21Matrix::SumMatrix(const S21Matrix &other) { PlusMinus(other, 1); }

void S21Matrix::SubMatrix(const S21Matrix &other) { PlusMinus(other, -1); }

void S21Matrix::MulNumber(const double &num) {
  for (int i = 0; i < rows_; ++i) {
    for (int k = 0; k < cols_; ++k) {
      matrix_[i][k] = matrix_[i][k] * num;
    }
  }
}

void S21Matrix::MulMatrix(const S21Matrix &other) {
  if (cols_ != other.rows_) {
    std::out_of_range("Columns of matrix_1 not equal to Rows of matrix_2");
  }
  if (cols_ <= 0 || other.cols_ <= 0 || rows_ <= 0 || other.rows_ <= 0) {
    throw std::invalid_argument(
        "Some columns or some rows equal or less to zero");
  }
  S21Matrix newMatrix(rows_, other.cols_);
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

S21Matrix S21Matrix::Transpose() {
  S21Matrix newMatrix(cols_, rows_);
  for (int i = 0; i < cols_; ++i) {
    for (int k = 0; k < rows_; ++k) {
      newMatrix.matrix_[i][k] = matrix_[k][i];
    }
  }
  return newMatrix;
}

double S21Matrix::Determinant() {
  if (rows_ != cols_ || rows_ == 0) {
    throw std::out_of_range("Matrix is not square");
  }
  double det = 0;
  if (rows_ == 1) {
    return matrix_[0][0];
  } else if (rows_ == 2) {
    return matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
  } else {
    S21Matrix newMatrix(rows_ - 1, cols_ - 1);
    for (int i = 0; i < rows_; ++i) {
      CutMatrix(*this, i, 0, newMatrix);
      det += newMatrix.Determinant() * matrix_[i][0] * ((i % 2 == 1) ? -1 : 1);
    }
    newMatrix.DeleteMatrix();
  }
  return det;
}

S21Matrix S21Matrix::CalcComplements() {
  S21Matrix minor(rows_, cols_);
  if (rows_ == cols_) {
    S21Matrix cut(rows_ - 1, cols_ - 1);
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

S21Matrix S21Matrix::InverseMatrix() {
  S21Matrix res(rows_, cols_);
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