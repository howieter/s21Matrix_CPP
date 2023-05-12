#include <gtest/gtest.h>

#include "s21_matrix_oop.hpp"

namespace TestCase {
void genMatrix(double ***matrix, const int &rows, const int &cols) {
  if (rows < 0 || cols < 0) {
    throw std::invalid_argument("Matrix parameters less than zero");
  }
  if (!(*matrix)) {
    (*matrix) = new double *[rows];
    for (int j = 0; j < rows; ++j) {
      (*matrix)[j] = new double[cols];
    }
  }
  srand(time(nullptr) + rand());
  for (int i = 0; i < rows; ++i) {
    for (int k = 0; k < cols; ++k) {
      (*matrix)[i][k] = rand() % 24;
    }
  }
}

void printMatrix(double **matrix, const int &rows, const int &cols) {
  for (int i = 0; i < rows; ++i) {
    cout << endl;
    for (int k = 0; k < cols; ++k) {
      cout << matrix[i][k] << " ";
    }
  }
  cout << endl;
}

void deleteMatrix(double ***matrix, const int &rows) {
  if ((*matrix)) {
    for (int j = 0; j < rows; ++j) {
      if ((*matrix)[j]) delete[](*matrix)[j];
    }
    delete[](*matrix);
    (*matrix) = nullptr;
  }
}

void genMatrix(S21::Matrix &matrix) {
  srand(time(nullptr) + rand());
  const int rows = rand() % 10 + 1;
  const int cols = rand() % 10 + 1;
  double **newMatrix = nullptr;
  genMatrix(&newMatrix, rows, cols);
  matrix.SetMatrix(newMatrix, rows, cols);
  deleteMatrix(&newMatrix, rows);
}

void fillMatrix(S21::Matrix &matrix) {
  srand(time(nullptr) + rand());
  for (int i = 0; i < matrix.GetRows(); ++i) {
    for (int k = 0; k < matrix.GetCols(); ++k) {
      matrix(i, k) = rand() % 24;
    }
  }
}
}  // namespace TestCase

TEST(Getters, Rows) {
  S21::Matrix matrix(3, 4);
  ASSERT_TRUE(matrix.GetRows() == 3);
}

TEST(Getters, Cols) {
  S21::Matrix matrix(3, 4);
  ASSERT_TRUE(matrix.GetCols() == 4);
}

TEST(Getters, Matrix) {
  S21::Matrix matrix(3, 4);
  double **otherMatrix = matrix.GetMatrix();
  ASSERT_TRUE(matrix.GetMatrix() == otherMatrix);
}

TEST(Setters, RowsIncrease) {
  srand(time(nullptr) + rand());
  const int rows = rand() % 10 + 1;
  const int cols = rand() % 10 + 1;
  S21::Matrix matrix(rows, cols);
  const int newRows = rows + 5;
  matrix.SetRows(newRows);
  ASSERT_TRUE(matrix.GetRows() == newRows &&
              matrix(newRows - 1, cols - 1) == 0);
}

TEST(Setters, RowsDecrease) {
  S21::Matrix matrix(3, 4);
  matrix.SetRows(1);
  ASSERT_TRUE(matrix.GetRows() == 1);
}

TEST(Setters, ColsIncrease) {
  srand(time(nullptr) + rand());
  const int rows = rand() % 10 + 1;
  const int cols = rand() % 10 + 1;
  S21::Matrix matrix(rows, cols);
  const int newCols = cols + 5;
  matrix.SetCols(newCols);
  ASSERT_TRUE(matrix.GetCols() == newCols &&
              matrix(rows - 1, newCols - 1) == 0);
}

TEST(Setters, ColsDecrease) {
  S21::Matrix matrix(3, 4);
  matrix.SetCols(1);
  ASSERT_TRUE(matrix.GetCols() == 1);
}

TEST(Setters, SetMatrix) {
  srand(time(nullptr));
  const int rows = rand() % 10 + 1;
  const int cols = rand() % 10 + 1;
  S21::Matrix matrix;
  double **newMatrix = nullptr;
  TestCase::genMatrix(&newMatrix, rows, cols);
  matrix.SetMatrix(newMatrix, rows, cols);
  ASSERT_TRUE(matrix.GetRows() == rows && matrix.GetCols() == cols &&
              newMatrix[rows - 1][cols - 1] == matrix(rows - 1, cols - 1));
  TestCase::deleteMatrix(&newMatrix, rows);
}

TEST(Constructors, Default) {
  S21::Matrix matrix;
  ASSERT_TRUE(matrix.GetRows() == 0 && matrix.GetCols() == 0 &&
              !matrix.GetMatrix());
}

TEST(Constructors, Parametres) {
  S21::Matrix matrix(3, 4);
  ASSERT_TRUE(matrix.GetRows() == 3 && matrix.GetCols() == 4 &&
              matrix(0, 0) == 0);
}

TEST(Constructors, Copy) {
  S21::Matrix matrix1;
  TestCase::genMatrix(matrix1);
  S21::Matrix matrix2(matrix1);
  ASSERT_TRUE(matrix1 == matrix2);
}

TEST(Constructors, Move) {
  S21::Matrix matrix1;
  TestCase::genMatrix(matrix1);
  S21::Matrix matrix1copy(matrix1);
  S21::Matrix matrix2 = (S21::Matrix &&) matrix1;
  S21::Matrix matrix0;
  ASSERT_TRUE(matrix1 == matrix0 && matrix2 == matrix1copy);
}

TEST(Functions, EqMatrixTrue) {
  S21::Matrix matrix1;
  TestCase::genMatrix(matrix1);
  S21::Matrix matrix2(matrix1);
  ASSERT_TRUE(matrix1.EqMatrix(matrix2));
}

TEST(Functions, EqMatrixFalse) {
  S21::Matrix matrix1;
  S21::Matrix matrix2;
  TestCase::genMatrix(matrix1);
  TestCase::genMatrix(matrix2);
  ASSERT_FALSE(matrix1.EqMatrix(matrix2));
}

TEST(Functions, SumMatrix) {
  srand(time(nullptr));
  const int rows = rand() % 10 + 1;
  const int cols = rand() % 10 + 1;
  double **newMatrix = nullptr;
  TestCase::genMatrix(&newMatrix, rows, cols);
  S21::Matrix matrix2;
  matrix2.SetMatrix(newMatrix, rows, cols);
  TestCase::deleteMatrix(&newMatrix, rows);
  S21::Matrix matrix1(rows, cols);
  matrix1.SumMatrix(matrix2);
  ASSERT_TRUE(matrix1 == matrix2);
}

TEST(Functions, SubMatrix) {
  srand(time(nullptr));
  const int rows = rand() % 10 + 1;
  const int cols = rand() % 10 + 1;
  double **newMatrix = nullptr;
  TestCase::genMatrix(&newMatrix, rows, cols);
  S21::Matrix matrix1;
  matrix1.SetMatrix(newMatrix, rows, cols);
  TestCase::deleteMatrix(&newMatrix, rows);
  S21::Matrix matrix2(matrix1);
  matrix1.SubMatrix(matrix2);
  S21::Matrix matrix0(rows, cols);
  ASSERT_TRUE(matrix1 == matrix0);
}

TEST(Functions, MulNumber) {
  S21::Matrix matrix1;
  TestCase::genMatrix(matrix1);
  S21::Matrix matrix2(matrix1);
  const double mul = 2.0;
  matrix1.MulNumber(mul);
  srand(time(nullptr));
  const int i = rand() % matrix1.GetRows();
  const int k = rand() % matrix2.GetCols();
  ASSERT_TRUE(matrix1(i, k) == (matrix2(i, k) * mul));
}

TEST(Functions, MulMatrix) {
  srand(time(nullptr) + rand());
  const int rows = rand() % 10 + 1;
  const int cols = rand() % 10 + 1;
  S21::Matrix matrix1(rows, cols);
  S21::Matrix matrix2(cols, rows);
  TestCase::fillMatrix(matrix1);
  S21::Matrix matrix1Copy(matrix1);
  TestCase::fillMatrix(matrix2);
  matrix1.MulMatrix(matrix2);
  auto mulMat = [&]() -> bool {
    for (int m = 0; m < matrix1Copy.GetRows(); ++m) {
      for (int n = 0; n < matrix2.GetCols(); ++n) {
        double val = 0.;
        for (int k = 0; k < matrix2.GetRows(); ++k) {
          val += matrix1Copy(m, k) * matrix2(k, n);
        }
        if ((n == matrix2.GetCols() - 1) && (matrix1(m, n) != val)) {
          return false;
        }
      }
    }
    return true;
  };
  bool valCorrect = mulMat();
  ASSERT_TRUE(matrix1.GetRows() == rows && matrix1.GetCols() == rows &&
              valCorrect);
}

TEST(Functions, Transpose) {
  S21::Matrix matrix1(3, 4);
  S21::Matrix matrix2 = matrix1.Transpose();
  ASSERT_TRUE(matrix1.GetRows() == matrix2.GetCols() &&
              matrix1.GetCols() == matrix2.GetRows());
}

TEST(Functions, Determinant) {
  S21::Matrix matrix(4, 4);
  matrix(0, 0) = 1;
  matrix(0, 1) = 12;
  matrix(0, 2) = 12;
  matrix(0, 3) = -9;
  matrix(1, 0) = 5;
  matrix(1, 1) = 15;
  matrix(1, 2) = 20;
  matrix(1, 3) = -2;
  matrix(2, 0) = 16;
  matrix(2, 1) = 6;
  matrix(2, 2) = 5;
  matrix(2, 3) = 41;
  matrix(3, 0) = 15;
  matrix(3, 1) = 3;
  matrix(3, 2) = 21;
  matrix(3, 3) = 7;
  ASSERT_TRUE(matrix.Determinant() == -27000);
}

TEST(Functions, CalcComplements) {
  S21::Matrix matrix1(4, 4);
  matrix1(0, 0) = 1;
  matrix1(0, 1) = 12;
  matrix1(0, 2) = 12;
  matrix1(0, 3) = -9;
  matrix1(1, 0) = 5;
  matrix1(1, 1) = 15;
  matrix1(1, 2) = 20;
  matrix1(1, 3) = -2;
  matrix1(2, 0) = 16;
  matrix1(2, 1) = 6;
  matrix1(2, 2) = 5;
  matrix1(2, 3) = 41;
  matrix1(3, 0) = 15;
  matrix1(3, 1) = 3;
  matrix1(3, 2) = 21;
  matrix1(3, 3) = 7;

  S21::Matrix matrix2(4, 4);
  matrix2(0, 0) = -10992;
  matrix2(0, 1) = -5408;
  matrix2(0, 2) = 7224;
  matrix2(0, 3) = 4200;
  matrix2(1, 0) = 9939;
  matrix2(1, 1) = 2861;
  matrix2(1, 2) = -6333;
  matrix2(1, 3) = -3525;
  matrix2(2, 0) = -1443;
  matrix2(2, 1) = -1157;
  matrix2(2, 2) = 1221;
  matrix2(2, 3) = -75;
  matrix2(3, 0) = -2841;
  matrix2(3, 1) = 641;
  matrix2(3, 2) = 327;
  matrix2(3, 3) = 975;
  ASSERT_TRUE(matrix1.CalcComplements() == matrix2);
}

TEST(Functions, Inverse) {
  S21::Matrix matrix1(4, 4);
  matrix1(0, 0) = 1;
  matrix1(0, 1) = 12;
  matrix1(0, 2) = 12;
  matrix1(0, 3) = -9;
  matrix1(1, 0) = 5;
  matrix1(1, 1) = 15;
  matrix1(1, 2) = 20;
  matrix1(1, 3) = -2;
  matrix1(2, 0) = 16;
  matrix1(2, 1) = 6;
  matrix1(2, 2) = 5;
  matrix1(2, 3) = 41;
  matrix1(3, 0) = 15;
  matrix1(3, 1) = 3;
  matrix1(3, 2) = 21;
  matrix1(3, 3) = 7;

  S21::Matrix matrix2(4, 4);
  matrix2(0, 0) = 0.407111111;
  matrix2(0, 1) = -0.368111111;
  matrix2(0, 2) = 0.053444444;
  matrix2(0, 3) = 0.105222222;
  matrix2(1, 0) = 0.200296296;
  matrix2(1, 1) = -0.105962963;
  matrix2(1, 2) = 0.042851852;
  matrix2(1, 3) = -0.023740741;
  matrix2(2, 0) = -0.267555556;
  matrix2(2, 1) = 0.234555556;
  matrix2(2, 2) = -0.045222222;
  matrix2(2, 3) = -0.012111111;
  matrix2(3, 0) = -0.155555556;
  matrix2(3, 1) = 0.130555556;
  matrix2(3, 2) = 0.002777778;
  matrix2(3, 3) = -0.036111111;
  ASSERT_TRUE(matrix1.InverseMatrix() == matrix2);
}

TEST(Operators, Equal) {
  S21::Matrix matrix1;
  TestCase::genMatrix(matrix1);
  S21::Matrix matrix2(matrix1);
  ASSERT_TRUE(matrix1.EqMatrix(matrix2) == (matrix1 == matrix2));
}

TEST(Operators, Assignment) {
  S21::Matrix matrix1;
  TestCase::genMatrix(matrix1);
  S21::Matrix matrix2 = matrix1;
  ASSERT_TRUE(matrix1 == matrix2);
}

TEST(Operators, ReturnElement) {
  S21::Matrix matrix(3, 4);
  TestCase::fillMatrix(matrix);
  srand(time(nullptr));
  int i = rand() % 3;
  int k = rand() % 4;
  ASSERT_TRUE(matrix.GetMatrix()[i][k] == matrix(i, k));
}

TEST(Operators, SumMatrix) {
  S21::Matrix matrix1(3, 4);
  S21::Matrix matrix2(3, 4);
  TestCase::fillMatrix(matrix1);
  TestCase::fillMatrix(matrix2);
  S21::Matrix matrix3(matrix1);
  matrix3.SumMatrix(matrix2);
  ASSERT_TRUE(matrix3 == (matrix1 + matrix2));
}

TEST(Operators, SumMatrixIncrement) {
  S21::Matrix matrix1(3, 4);
  S21::Matrix matrix2(3, 4);
  TestCase::fillMatrix(matrix1);
  TestCase::fillMatrix(matrix2);
  S21::Matrix matrix3(matrix1);
  matrix3.SumMatrix(matrix2);
  ASSERT_TRUE(matrix3 == (matrix1 += matrix2));
}

TEST(Operators, SubMatrix) {
  S21::Matrix matrix1(3, 4);
  S21::Matrix matrix2(3, 4);
  TestCase::fillMatrix(matrix1);
  TestCase::fillMatrix(matrix2);
  S21::Matrix matrix3(matrix1);
  matrix3.SubMatrix(matrix2);
  ASSERT_TRUE(matrix3 == (matrix1 - matrix2));
}

TEST(Operators, SubMatrixDecrement) {
  S21::Matrix matrix1(3, 4);
  S21::Matrix matrix2(3, 4);
  TestCase::fillMatrix(matrix1);
  TestCase::fillMatrix(matrix2);
  S21::Matrix matrix3(matrix1);
  matrix3.SubMatrix(matrix2);
  ASSERT_TRUE(matrix3 == (matrix1 -= matrix2));
}

TEST(Operators, MulToNums) {
  S21::Matrix matrix1;
  TestCase::genMatrix(matrix1);
  S21::Matrix matrix2(matrix1);
  matrix1.MulNumber(2);
  ASSERT_TRUE(matrix1 == (matrix2 * 2));
}

TEST(Operators, MulNumsTo) {
  S21::Matrix matrix1;
  TestCase::genMatrix(matrix1);
  S21::Matrix matrix2(matrix1);
  matrix1.MulNumber(2);
  ASSERT_TRUE(matrix1 == (2 * matrix2));
}

TEST(Operators, MulNumsIncrement) {
  S21::Matrix matrix1;
  TestCase::genMatrix(matrix1);
  S21::Matrix matrix2(matrix1);
  matrix1.MulNumber(2);
  ASSERT_TRUE(matrix1 == (matrix2 *= 2));
}

TEST(Operators, MulMatrix) {
  S21::Matrix matrix1(3, 4);
  S21::Matrix matrix2(3, 4);
  TestCase::fillMatrix(matrix1);
  TestCase::fillMatrix(matrix2);
  S21::Matrix matrix3(matrix1);
  matrix3.MulMatrix(matrix2);
  ASSERT_TRUE(matrix3 == (matrix1 * matrix2));
}

TEST(Operators, MulMatrixIncrement) {
  S21::Matrix matrix1(3, 4);
  S21::Matrix matrix2(3, 4);
  TestCase::fillMatrix(matrix1);
  TestCase::fillMatrix(matrix2);
  S21::Matrix matrix3(matrix1);
  matrix3.MulMatrix(matrix2);
  ASSERT_TRUE(matrix3 == (matrix1 *= matrix2));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}