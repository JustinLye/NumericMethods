#if !defined(__CHOLESKY_H__)
#define __CHOLESKY_H__

#include<Dense>
#include<cmath>
namespace jl {
#if !defined(__JLLONGDOUBLEMATRIX__)
#define __JLLONGDOUBLEMATRIX__
	typedef Eigen::Matrix<long double, Eigen::Dynamic, Eigen::Dynamic> MatrixXld;
#endif

	jl::MatrixXld choleskyDecomp(const Eigen::Ref<const jl::MatrixXld>& G) {
	jl::MatrixXld L;
	L.resize(G.rows(), G.cols());
	L.setZero();
	//need to ensure square matrix here
	long long n = G.rows();
	long double s = 0, v = 0; //declare summation variables
	for(long long k = 0; k < n; k++) { //outer loop of Gramm matrix rows
		for(long long i = 0; i < k; i++) { //inner loop of Gamm matrix columns for each row
			s = 0;
			for(long long j = 0; j < i; j++) {
				s = s + (L(i,j) * L(k,j));
			}
			L(k,i) = (G(k,i) - s) / L(i,i);
		}
		v = 0;
		for(long long j = 0; j < k; j++) {
			v += std::powl(L(k,j), 2.0);
		}
		L(k,k) = std::sqrtl((G(k,k) - v));
	}
	return L;
	}
}
#endif