#ifndef _COMPLEX_H_
#define _COMPLEX_H_

#define PI 3.141592653589793

class Complex
{
public:
	Complex();
	Complex(double _Re);
	Complex(double _Re, double _Im);
	Complex(const Complex &rhs);

	double Real() const    { return _real; }
	double Imag() const    { return _imag; }

	Complex& operator+=(const double&  d);
	Complex& operator+=(const Complex& c);
	
	Complex& operator-=(const double&  d);
	Complex& operator-=(const Complex& c);

	Complex& operator*=(const double&  d);
	Complex& operator*=(const Complex& c);
		
	Complex& operator/=(const double&  d);
	Complex& operator/=(const Complex& c);

	Complex& operator=(const Complex& C);

	void Set(const double& re, const double& im);
	
	~Complex(void);
private:
	double _real;
	double _imag;
};

Complex operator+(const Complex& c1, const Complex& c2);
Complex operator-(const Complex& c1, const Complex& c2);

Complex operator*(const Complex& c1, const Complex& c2);
Complex operator*(const Complex& c1, const double   d2);
Complex operator*(const double   d1, const Complex& c2);

Complex operator/(const Complex& c1, const Complex& c2);
Complex operator/(const Complex& c1, const double   d2);
Complex operator/(const double   d1, const Complex& c2);

Complex operator+(const Complex& c);
Complex operator-(const Complex& c);

Complex Conj(const Complex& c);
double Mod(const Complex& c);
double ModSquare(const Complex& c);

#define ABSComplex(c) Mod(c)
#define ABSInt(c) (((c) > 0) ? (c) : -1 * (c))

#endif
