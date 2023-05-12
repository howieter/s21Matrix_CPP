#ifndef S21_MATRIX_OOP_H_
#define S21_MATRIX_OOP_H_

#include <cmath>
#include <iostream>

using namespace std;
namespace S21 {
class Matrix {
 private:
  int rows_, cols_;
  double **matrix_;

 protected:
  bool SizeCompare(const Matrix &) const;
  void InitializeMatrix();
  void DeleteMatrix();
  void PlusMinus(const Matrix &, const int &);
  void CutMatrix(Matrix &, const int &, const int &, Matrix &);
  void CopyMatrix(const Matrix &);

 public:
  Matrix() noexcept;
  Matrix(const int &, const int &);
  Matrix(const Matrix &) noexcept;
  Matrix(Matrix &&) noexcept;
  ~Matrix();

  int GetRows() const;
  int GetCols() const;
  double **GetMatrix() const;

  void SetRows(const int &);
  void SetCols(const int &);
  void SetMatrix(double **, const int &, const int &);

  Matrix operator+(const Matrix &) const;
  Matrix operator-(const Matrix &) const;
  Matrix operator*(const Matrix &) const;
  friend Matrix operator*(const Matrix &, const double &);
  friend Matrix operator*(const double &, const Matrix &);
  Matrix &operator+=(const Matrix &);
  Matrix &operator-=(const Matrix &);
  Matrix &operator*=(const Matrix &);
  Matrix &operator*=(const double &);
  Matrix &operator=(const Matrix &);
  bool operator==(const Matrix &) const;
  double &operator()(const int &, const int &) const;

  bool EqMatrix(const Matrix &) const;
  void SumMatrix(const Matrix &);
  void SubMatrix(const Matrix &);
  void MulNumber(const double &);
  void MulMatrix(const Matrix &);
  Matrix Transpose();
  double Determinant();
  Matrix CalcComplements();
  Matrix InverseMatrix();
};
Matrix operator*(const Matrix &, const double &);
Matrix operator*(const double &, const Matrix &);
}  // namespace S21

#endif  //  S21_MATRIX_OOP_H_
