#include "Complex.h"
#include "MatrixClass.h"
#include "ErrorHandling.h"
#include "math.h"

Matrix::Matrix(void)
{
	_colNum = 1;
	_rowNum = 1;

	_buf = new Complex[_colNum * _rowNum];

	if (_buf == 0)
	{
		ErrorMsg(MemAllocateFail, __FILE__, __LINE__, "Cannot new Complex, col = %d; row = %d", _colNum, _rowNum);
	}

	reset();
}

Matrix::Matrix(int N)
{
	int r = N;
	int c = N;
	
	if (r * c == 0)
	{
		ErrorMsg(ParameterIncorrect, __FILE__, __LINE__,
			     "r = %d c = %d", r, c);
	}
	
	_rowNum = r;
	_colNum = c;

	_buf = new Complex[_colNum * _rowNum];

	if (_buf == 0)
	{
		ErrorMsg(MemAllocateFail, __FILE__, __LINE__, "Cannot new Complex, col = %d; row = %d", _colNum, _rowNum);
	}

	reset();
}

Matrix::Matrix(int r, int c)
{
	if (r * c == 0)
	{
		ErrorMsg(ParameterIncorrect, __FILE__, __LINE__,
			     "r = %d c = %d", r, c);
	}
	
	_rowNum = r;
	_colNum = c;

	_buf = new Complex[_colNum * _rowNum];

	if (_buf == 0)
	{
		ErrorMsg(MemAllocateFail, __FILE__, __LINE__, "Cannot new Complex, col = %d; row = %d", _colNum, _rowNum);
	}

	reset();
}

Complex Matrix::get(const int idx) const
{
	return _buf[idx];
}

Complex Matrix::get(const int rowIdx, const int colIdx) const
{
	int idx = colIdx * _rowNum + rowIdx;
	return _buf[idx];
}


Matrix::Matrix(const Matrix & M)
{
	_rowNum = M._rowNum;
	_colNum = M._colNum;
	
	_buf = new Complex[_colNum * _rowNum];

	if (_buf == 0)
	{
		ErrorMsg(MemAllocateFail, __FILE__, __LINE__, "Cannot new Complex, col = %d; row = %d", _colNum, _rowNum);
	}	

	for (int idx = 0; idx < _colNum * _rowNum; idx++)
	{
		insert(idx, M.get(idx));
	}
}
	

Matrix::~Matrix(void)
{
	delete[] _buf;
}

void Matrix::insert(const int rowIdx, const int colIdx, const Complex& c)
{
	_buf[colIdx * _rowNum + rowIdx] = c;
}

void Matrix::insert(const int idx, const Complex& c)
{
	if (idx < 0 || idx >= _colNum * _rowNum)
	{
		WaringMsg(OutOfBufBoundery, 
			      __FILE__, __LINE__, 
			      "idx = %d should be within [0, %d]", idx, _colNum * _rowNum - 1);
		return;
	}

	_buf[idx] = c;
}

void Matrix::insert(const Complex& c)
{
	_buf[0] = c;
}

Matrix& Matrix::operator=(const Matrix& M)
{
	_rowNum = M._rowNum;
	_colNum = M._colNum;

	delete[] _buf;
	
	_buf = new Complex[_colNum * _rowNum];

	if (_buf == 0)
	{
		ErrorMsg(MemAllocateFail, __FILE__, __LINE__, "Cannot new Complex, col = %d; row = %d", _colNum, _rowNum);
	}	

	for (int idx = 0; idx < _colNum * _rowNum; idx++)
	{
		insert(idx, M.get(idx));
	}	

	return * this;
}

Matrix& Matrix::operator+=(const Matrix& M)
{
	if ((_rowNum != M.RowNum()) || (_colNum != M.ColNum()))
	{
		ErrorMsg(GeneralError, __FILE__, __LINE__, "Two matrix have different dimention, can not be added!");
	}

	int N = _rowNum * _colNum;

	for (int idx = 0; idx < N; idx++)
	{
		_buf[idx] += M.get(idx);
	}

	return *this;
}

Matrix& Matrix::operator*=(const double& d)
{
	int N = _rowNum * _colNum;

	for (int idx = 0; idx < N; idx++)
	{
		_buf[idx] *= d;
	}
	
	return *this;
}

int Matrix::RowNum() const
{
	return _rowNum;
}

int Matrix::ColNum() const
{
	return _colNum;
}

bool Matrix::isSqare() const
{
	return (_colNum == _rowNum);
}

void Matrix::reset()
{
	for (int idx = 0; idx < _rowNum * _colNum; idx++)
	{
		_buf[idx] = Complex(0, 0);
	}
}

Eye::Eye(int N)
	: Matrix(N)
{
	for (int rowIdx = 0; rowIdx < N; rowIdx++)
	{
		for (int colIdx = 0; colIdx < N; colIdx++)
		{
			double real = (colIdx == rowIdx) ? 1 : 0;
			double imag = real;
			
			insert(rowIdx, colIdx, Complex(real, imag));
		}
	}
}


Eye::~Eye()
{
}

Matrix operator*(const Matrix& M1, const Matrix& M2)
{
	int Mid;

	Mid = M1.ColNum();
	
	int M1Row = M1.RowNum();
	int M2Col = M2.ColNum();
	
	if (M1.ColNum() != M2.RowNum())
	{
		ErrorMsg(ParameterIncorrect, __FILE__, __LINE__,
			     "two matrix's dimention does't allow them to multiply (M1.Col = %d, M2.Row = %d",
			     M1.ColNum(), M2.RowNum());	
	}

	Matrix rlt(M1Row, M2Col);

	for (int rowIdx = 0; rowIdx < M1Row; rowIdx++)
	{
		for (int colIdx = 0; colIdx < M2Col; colIdx++)
		{
			Complex cplx(0, 0);
			
			for (int MidIdx = 0; MidIdx < Mid; MidIdx++)
			{
				cplx += M1.get(rowIdx, MidIdx) * M2.get(MidIdx, colIdx);
			}

			rlt.insert(rowIdx, colIdx, cplx);
		}
	}

	return rlt;
}

Matrix kron(const Matrix& M1, const Matrix& M2)
{
	int rltRowNum = M1.RowNum() * M2.RowNum();
	int rltColNum = M1.ColNum() * M2.ColNum();

	Matrix M(rltRowNum, rltColNum);

	for (int rowIdx = 0; rowIdx < rltRowNum; rowIdx++)
	{
		for (int colIdx = 0; colIdx < rltColNum; colIdx++)
		{
			int rowIdxTx = (int)(rowIdx / M2.RowNum());
			int rowIdxRx = rowIdx % M2.RowNum();

			int colIdxTx = (int)(colIdx / M2.ColNum());
			int colIdxRx = colIdx % M2.ColNum();

			M.insert(rowIdx, 
				     colIdx, 
				     M1.get(rowIdxTx, colIdxTx) * M2.get(rowIdxRx, colIdxRx));
		}
	}

	return M;
}

Matrix ConjTranspose(const Matrix& M)
{
	Matrix conjTransposeM(M.ColNum(), M.RowNum());

	for (int colIdx = 0; colIdx < M.ColNum(); colIdx++)
	{
		for (int rowIdx = 0; rowIdx < M.RowNum(); rowIdx++)
		{
			conjTransposeM.insert(colIdx, rowIdx, Conj(M.get(rowIdx, colIdx)));
		}
	}

	return conjTransposeM;
}

/********************************************************************************
*
*   Cholesky factorization.  R = CHOL(A) where R'*R = A
*
*	CHOL(A) uses only the diagonal and upper triangle of A.	
*	The lower triangle is assumed to be the (complex conjugate)
*	transpose of the upper triangle.  If A is positive definite, then
*	R = CHOL(A) produces an upper triangular R so that R'*R = A.
*
*	If A is not positive definite, program is stopped.
*
*********************************************************************************/
Matrix chol(const Matrix& A)
{
	if (!A.isSqare())
	{
		ErrorMsg(GeneralError, 
			     __FILE__, __LINE__, 
			     "A should be square Matrix. A.RowNum = %d, A.ColNum = %d", 
			     A.RowNum(), A.ColNum());
	}

	int N = A.ColNum();

	Matrix R(N);

	for (int idx0 = 0; idx0 < N; idx0++)
	{
		/* calculate R(idx0, idx0) */
		Complex c0(0, 0);
		
		for (int idx1 = 0; idx1 < idx0; idx1++)
		{
			c0 += ModSquare(R.get(idx1, idx0));
		}
		
		double RealTmp = ((A.get(idx0, idx0) - c0)).Real();

		if (RealTmp < 0)
		{
			ErrorMsg(GeneralError, 
				     __FILE__, __LINE__, 
				     "The input Matrix is positive definite ? ");
		}

		R.insert(idx0, idx0, Complex(sqrt(RealTmp), 0));

		/* calculate R(idx0, [(idx0 + 1) : end]) */
		for (int idx1 = idx0 + 1; idx1 < N; idx1++)
		{
			c0.Set(0, 0);			
			for (int idx2 = 0; idx2 < idx0; idx2++)
			{
				c0 += Conj(R.get(idx2, idx0)) * R.get(idx2, idx1);
			}

			Complex CplxTmp = (A.get(idx0, idx1) - c0) / Conj(R.get(idx0, idx0));

			R.insert(idx0, idx1, CplxTmp);
		}
	}

	return R;
}

