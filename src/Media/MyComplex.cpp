#include "MyComplex.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <fstream>

using namespace std;

void Abs(std::vector<Complex>& a)
{
	for (auto& c : a)
	{
		c.r = sqrt(c.r * c.r+c.i * c.i);
		c.i = 0;
	}
}

void FFT_inner(std::vector<Complex>& a, int lim, int opt)
{
	if (lim == 1) return;
	vector<Complex> a0(lim / 2), a1(lim / 2);
	for (int i = 0; i < lim; i += 2)
	{
		a0[i / 2] = a[i];
		a1[i / 2] = a[i + 1];
	}
	FFT_inner(a0, lim / 2, opt);
	FFT_inner(a1, lim / 2, opt);
	Complex wn(cos(2.0 * M_PI / lim), opt * sin(2.0 * M_PI / lim));
	Complex w(1, 0);
	for (int k = 0; k < lim / 2; ++k)
	{
		a[k] = a0[k] + w * a1[k];
		a[k + lim / 2] = a0[k] - w * a1[k];
		w = w * wn;
	}
	if (opt == -1)
		for (int i = 0; i < lim; ++i)
		{
			a[i].r /= (double)lim;
			a[i].i = -a[i].i / (double)lim;
		}
}

void FFT(vector<Complex>& a, int lim)
{
	FFT_inner(a, lim, 1);
}

void iFFT(vector<Complex>& a, int lim)
{
	//FFT_inner(a, lim, -1);
	//return;
	for (auto& c : a)
	{
		c.i = -c.i;
	}
	FFT_inner(a, lim, 1);
	for (auto& c : a)
	{
		c.r /= lim;
		c.i = -c.i / lim;
	}
}

void ComplexWriteToFile(vector<Complex>& cp, std::string filename)
{
	ofstream ofs(filename, ios::out);
	if (!ofs.is_open())
		throw runtime_error("Can not open file " + filename);
	for (auto& c : cp)
		ofs << c.r << "," << c.i << endl;
	ofs.close();
}

int GetLowerAndNearest2Power(int n)
{
	int ret = 1;
	while (1)
	{
		if (ret * 2 > n)
			break;
		else
			ret *= 2;
	}
	return ret;
}