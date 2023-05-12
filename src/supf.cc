#include "s21_matrix_oop.hpp"

// Support functions
namespace S21 {
void Matrix::InitializeMatrix() {
  if (rows_ <= 0 && cols_ <= 0) {
    throw std::invalid_argument("matrix_ parameters less or equal to zero");
  }
  matrix_ = new double *[rows_];
  for (int i = 0; i < rows_; ++i) {
    matrix_[i] = new double[cols_];
    for (int k = 0; k < cols_; ++k) {
      matrix_[i][k] = 0;
    }
  }
}

void Matrix::DeleteMatrix() {
  if (matrix_) {
    for (int i = 0; i < rows_; ++i) {
      delete[] matrix_[i];
    }
    delete[] matrix_;
    matrix_ = nullptr;
    rows_ = 0;
    cols_ = 0;
  }
}

bool Matrix::SizeCompare(const Matrix &other) const {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    return false;
  }
  return true;
}

void Matrix::PlusMinus(const Matrix &other, const int &sign) {
  if (SizeCompare(other) == false) {
    throw std::out_of_range("Matrix parameters are not equal to each other");
  }
  for (int i = 0; i < rows_; ++i) {
    for (int k = 0; k < cols_; ++k) {
      matrix_[i][k] = matrix_[i][k] + sign * other.matrix_[i][k];
    }
  }
}

void Matrix::CutMatrix(Matrix &A, const int &rows_del, const int &columns_del,
                       Matrix &R) {
  for (int i = 0, j = 0; i < A.rows_; ++i) {
    if (i == rows_del) {
      continue;
    }
    for (int k = 0, l = 0; k < A.cols_; ++k) {
      if (k == columns_del) {
        continue;
      }
      R.matrix_[j][l] = A.matrix_[i][k];
      ++l;
    }
    ++j;
  }
}

void Matrix::CopyMatrix(const Matrix &A) {
  if (!EqMatrix(A)) {
    DeleteMatrix();
    rows_ = A.rows_;
    cols_ = A.cols_;
    InitializeMatrix();
    for (int i = 0; i < rows_; ++i) {
      for (int k = 0; k < cols_; ++k) {
        matrix_[i][k] = A.matrix_[i][k];
      }
    }
  }
}
}  // namespace S21