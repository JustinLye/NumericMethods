//Justin Lye CS 3513 PP4
//cubicOptimized & cubicNewtons are based off of what was presented in class
#if !defined(__NM_CUBIC_FUNCTIONS_H__)
#define __NM_CUBIC_FUNCTIONS_H__
#include<cmath>
namespace nm {
	const double TWO_OVER_THIRDS[] = { 1.0f, 1.2599210498948731647672106072782, 1.5874010519681994747517056392723 };
	const double ONE_THIRD = 0.33333333333333333333333333333333;
	const double TWO_THIRDS = 0.66666666666666666666666666666667;
	const double C1 = 0.5874010519682; // C1 = 2 * (1/2)^(1/3) - 1
	const double C2 = 0.4125989480318; // C2 = 1 - C1
	const int iterationLimit = 50; //Iteration limit for netwons method.
	const double relErrorEsp = 0.000000001;
	//standard version
	inline double cubicStandard(const double& y) { return std::pow(y, (1.0f/3.0f)); }
	//null version
	inline double cubicNull(const double& y) { return 1.0; }
	//optimized version
	inline double cubicOptimized(const double& y) {
		//initialize local variables
		int b = 0;
		int q = 0;
		int r = 0;
		double a = frexp(y, &b); //get the exponent and mantissa of y
		double X1 = std::fma(a, C2, C1); //calculate initial guess C1 + a * C2
		double X2 = TWO_THIRDS * X1 + ONE_THIRD * (a/X1/X1); //second iteration
		double X3 = TWO_THIRDS * X2 + ONE_THIRD * (a/X2/X2); //third iteration
		q = b / 3; //get the quotinent of b/3
		r = b % 3; //get the remainder of b/3
		return X3 * ldexp(TWO_OVER_THIRDS[r], q); //return X3 * 2^b/3
	}
	//newton's version
	inline double cubicNewtons(const double& y) {
		double Xnew = y; //set initial guess = y.
		double Xold = 0.0f; //initialize local variable
		int iteration = 0; //initialize local variable
		do {
			Xold = Xnew; //set Xold = Xnew
			Xnew = Xold - ((Xold * Xold * Xold) - y) / (3.0f * (Xold * Xold)); //calculate new X using newton's iteration formula Xn+1 = Xn - (Xn^(3) - y) / 3Xn^(2).
		}while(((std::abs(Xnew - Xold)) / Xnew) > relErrorEsp && iteration++ < iterationLimit); //exit loop the relative error is < 10^(-9) or the iterations limit has been reached. Also, I was under the impression that the iteration count should not have exceeded 15 iterations; however, I was getting an iteration count of 30 or more consistantly.
		return Xnew; //return cubic root
	}
}

#endif
