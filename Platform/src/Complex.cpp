#include "math.h"
#include "Complex.h"


Complex::Complex()
{
	_imag = 0;
	_real = 0;
}

Complex::Complex(double Re, double Im)
{
	_real = Re;
	_imag = Im;
}

Complex::Complex(double Re)
{
	_real = Re;
	_imag = 0;
}

Complex::Complex(const Complex &rhs)
{
	_real = rhs._real;
	_imag = rhs._imag;
}

Complex::~Complex(void)
{
}

Complex operator+(const Complex& c1, const Complex& c2)
{
	return Complex(c1.Real() + c2.Real(), c2.Imag() + c1.Imag());
}

Complex operator+(const Complex& c)
{  
	return Complex(c.Real(), c.Imag());
}

Complex operator-(const Complex& c1, const Complex& c2)
{
	return Complex(c1.Real() - c2.Real(), c1.Imag() - c2.Imag());
}

Complex operator-(const Complex& c)
{  
	return Complex(-c.Real(), -c.Imag());
}

Complex& Complex::operator+=(const double&  d)
{
	_real += d;
	return *this;
}

Complex& Complex::operator+=(const Complex& c)
{
	_real += c.Real();
	_imag += c.Imag();
	return *this;
}


Complex& Complex::operator-=(const double&  d)
{
	_real -= d;
	return *this;
}

Complex& Complex::operator-=(const Complex& c)
{
	_real -= c.Real();
	_imag -= c.Imag();
	return *this;
}

void Complex::Set(const double& re, const double& im)
{ 
	_real = re; 
	_imag = im; 
}

Complex& Complex::operator*=(const double&  d)
{
	_real *= d;
	_imag *= d;
	return *this;
}

Complex& Complex::operator*=(const Complex& c)
{
	double _realTmp;
	
	_realTmp = _real * c.Real() - _imag * c.Imag();
	_imag    = _real * c.Imag() + _imag * c.Real();

	_real = _realTmp;
	
	return *this;
}
	
Complex& Complex::operator/=(const double& d)
{
	_real /= d;
	_imag /= d;
	return *this;
}

Complex& Complex::operator/=(const Complex& c)
{
	*this *= Conj(c);
	*this /= ModSquare(c);
	return *this;
}

Complex& Complex::operator=(const Complex& C)
{
	_real = C.Real();
	_imag = C.Imag();

	return *this;
}

double Mod(const Complex& c) 
{
	return sqrt(c.Real() * c.Real() + c.Imag() * c.Imag());
}

double ModSquare(const Complex& c)
{
	return (c.Real() * c.Real() + c.Imag() * c.Imag());
}

Complex Conj(const Complex& c) 
{
	return Complex(c.Real(), -c.Imag());
}

Complex operator*(const Complex& c1, const Complex& c2)
{
	double Re = c1.Real() * c2.Real() - c1.Imag() * c2.Imag();
	double Im = c1.Real() * c2.Imag() + c1.Imag() * c2.Real();
	return Complex(Re, Im);
}

Complex operator*(const Complex& c1, const double d2)
{
	return Complex(c1.Real() * d2, c1.Imag() * d2);
}

Complex operator*(const double d1, const Complex& c2)
{
	return Complex(c2.Real() * d1, c2.Imag() * d1);
}

Complex operator/(const Complex& c1, const Complex& c2)
{
	return (c1 * Conj(c2) / ModSquare(c2));
}

Complex operator/(const Complex& c1, const double   d2)
{
	return Complex(c1.Real() / d2, c1.Imag() / d2);
}
Complex operator/(const double   d1, const Complex& c2)
{
	return (d1 * Conj(c2) / ModSquare(c2));
}

