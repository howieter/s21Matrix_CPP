#ifndef S21_MATRIX_OOP_H_
#define S21_MATRIX_OOP_H_

#include <cmath>
#include <iostream>

using namespace std;
namespace S21 {
class S21Matrix {
 private:
  int rows_, cols_;
  double **matrix_;

 protected:
  bool SizeCompare(const S21Matrix &) const;
  void InitializeMatrix();
  void DeleteMatrix();
  void PlusMinus(const S21Matrix &, const int &);
  void CutMatrix(S21Matrix &, const int &, const int &, S21Matrix &);
  void CopyMatrix(const S21Matrix &);

 public:
  S21Matrix() noexcept;
  S21Matrix(const int &, const int &);
  S21Matrix(const S21Matrix &) noexcept;
  S21Matrix(S21Matrix &&) noexcept;
  ~S21Matrix();

  int GetRows() const;
  int GetCols() const;
  double **GetMatrix() const;

  void SetRows(const int &);
  void SetCols(const int &);
  void SetMatrix(double **, const int &, const int &);

  S21Matrix operator+(const S21Matrix &) const;
  S21Matrix operator-(const S21Matrix &) const;
  S21Matrix operator*(const S21Matrix &) const;
  friend S21Matrix operator*(const S21Matrix &, const double &);
  friend S21Matrix operator*(const double &, const S21Matrix &);
  S21Matrix &operator+=(const S21Matrix &);
  S21Matrix &operator-=(const S21Matrix &);
  S21Matrix &operator*=(const S21Matrix &);
  S21Matrix &operator*=(const double &);
  S21Matrix &operator=(const S21Matrix &);
  bool operator==(const S21Matrix &) const;
  double &operator()(const int &, const int &) const;

  bool EqMatrix(const S21Matrix &) const;
  void SumMatrix(const S21Matrix &);
  void SubMatrix(const S21Matrix &);
  void MulNumber(const double &);
  void MulMatrix(const S21Matrix &);
  S21Matrix Transpose();
  double Determinant();
  S21Matrix CalcComplements();
  S21Matrix InverseMatrix();
};
S21Matrix operator*(const S21Matrix &, const double &);
S21Matrix operator*(const double &, const S21Matrix &);
}  // namespace S21

#endif  //  S21_MATRIX_OOP_H_
