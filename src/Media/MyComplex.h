#pragma once
#include <vector>
#include <string>

class Complex
{
private:
public:
    double r, i;
    Complex() :r(0), i(0) {}
    Complex(double real, double imag) :r(real), i(imag) {}
    Complex operator+(Complex c)
    {
        return Complex(r + c.r, i + c.i);
    }
    Complex operator-(Complex c)
    {
        return Complex(r - c.r, i - c.i);
    }
    Complex operator*(Complex c)
    {
        return Complex(r * c.r - i * c.i, r * c.i + i * c.r);
    }
};

void Abs(std::vector<Complex>& a);
void FFT_inner(std::vector<Complex>& a, int lim, int opt);
void FFT(std::vector<Complex>& a, int lim);
void iFFT(std::vector<Complex>& a, int lim);
void ComplexWriteToFile(std::vector<Complex>& cp, std::string filename);

//eg. input 44100 output 32768
int GetLowerAndNearest2Power(int n);