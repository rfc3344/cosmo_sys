#ifndef _MATRIXCLASS_H_
#define _MATRIXCLASS_H_

#include "Complex.h"

class Matrix
{
public:
	Matrix(void);
	Matrix(int N);
	Matrix(int r, int c);

	Matrix(const Matrix & M);

	/* RowIdx first ... [idx = colIdx * _rowNum + rowIdx]  */
	void insert(const int rowIdx, const int colIdx, const Complex& c);
	void insert(const int idx, const Complex& c);
	void insert(const Complex& c);

	Complex get(const int row, const int col) const;
	Complex get(const int idx) const;
	Complex get() const;

	bool isSqare() const;

	int RowNum() const;
	int ColNum() const;
	
	Matrix& operator=(const Matrix& M);
	Matrix& operator+=(const Matrix& M);

	Matrix& operator*=(const double& d);
	
	~Matrix(void);
private:
	int _rowNum;
	int _colNum;

	Complex * _buf;
	void reset();
};

class Eye : public Matrix
{
public:
	Eye(int N);
	~Eye();
private:	
};

Matrix operator*(const Matrix& M1, const Matrix& M2);

Matrix kron(const Matrix& M1, const Matrix& M2);

Matrix chol(const Matrix& M);

Matrix ConjTranspose(const Matrix& M);

#endif
