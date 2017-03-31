#if !defined(__nmforwsub_h__)
#define __nmforwsub_h__

#include "matrixBuffer.h"

namespace nm {


	long long AbsMaxIndex(const Eigen::VectorXd& x, long long start = 0) {
		long long index = start;
		long double _max = 0.0f;
		for(long long i = start; i < x.rows(); i++) {
			if(std::fabsl(x(i)) > _max) {
				_max = std::fabsl(x(i));
				index = i;
			}
		}
		return index;
	}

	//backward-sub assumes matrix U is upper triangular positive-definiate matrix (no-pivoting)
	//Solves the system Ux = y for x
	//arguments are not check for validity.
	Eigen::VectorXd BackwardSub(const Eigen::Ref<Eigen::MatrixXd>& U, const Eigen::VectorXd& y) {
		Eigen::VectorXd x = y; //set output vector x = y
		ll n = U.rows(); // set n = rows of matrix U
		for(ll j = n - 1; j >= 0; j--) {
			x(j) *= (1.0f/U(j,j)); //scale the jth element of y by the leading element
			for(ll k = j - 1; k >= 0; k--)
				x(k) -= (x(j) * U(k,j));
		}
		return x;
	}


	//forward-sub assumes matrix L is lower triangluar positive-definate matrix (no-pivoting)
	//Solves the system Ly = b for y
	//arguments are not checked for validity.
	Eigen::VectorXd ForwardSub(const Eigen::Ref<Eigen::MatrixXd>& L, const Eigen::VectorXd& b) {
		Eigen::VectorXd y = b; //set output vector y equal to b
		long long n = L.rows();// set n = rows of matrix L
		long double c = 0.0f; // initialize c to zero. c will be used to hold values needed for scaling
		for(long long j = 0; j < n; j++) { //the outer most loop will iterate through the columns of L
			c = L(j,j); //set scalar c = the jth element along the diagonal of L
			y(j) *= (1.0f/c); //scale the jth element of b by the leading element
			for(long long k = j + 1; k < n; k++) //this inner loop will eliminate the elements in the jth column below the jth row
				y(k) = y(k) - y(j) * L(k,j);
		}
		return y;
	}

}

#endif