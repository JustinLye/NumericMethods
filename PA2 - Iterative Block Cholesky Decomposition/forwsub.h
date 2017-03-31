#if !defined(__jlforwsub_h__)
#define __jlforwsub_h__

#include "rbmm.h"

namespace jl {
	
	void swap(jl::MatrixXld&, long long, long long);

	long long pivot(jl::MatrixXld& A, long long pivot_row, long long pivot_column) {
		long double _max = 0;
		long long _pivot = -1;		
		
		for(long long i = pivot_row; i < A.rows(); i++) {
			if(_max < std::fabsl(A(i,pivot_column))) {
				_max = std::fabsl(A(i,pivot_column));
				_pivot = i;
			}
		}
		return _pivot;
	}

	void pivot(jl::MatrixXld& A, jl::MatrixXld& b, long long pivot_row, long long pivot_column) {
		long long _pivot = pivot(A,pivot_row,pivot_column);
		if(_pivot > -1) {
			swap(A, pivot_row, _pivot);
			swap(b, pivot_row, _pivot);
		}
	}

	void swap(jl::MatrixXld& A, long long u, long long l) {
		jl::MatrixXld temp = A.block(u, 0, 1, A.cols());
		A.block(u, 0, 1, A.cols()) = A.block(l, 0, 1, A.cols());
		A.block(l, 0, 1, A.cols()) = temp.block(0,0,1, temp.cols());
	}
	
	jl::MatrixXld backSub(jl::MatrixXld& U, jl::MatrixXld& y) {
		jl::MatrixXld x = y;
		long long n = U.rows();
		for(long long p = n - 1; p >= 0; p--) {
			for(long long r = 0; r < p; r++) {
				x(r,0) = x(r,0) - (U(r,p) * x(p,0)) / U(p,p);
			}
		}
		return x;
	}
	
	void forwardSub(const Eigen::Ref<const jl::MatrixXld>& A, const Eigen::Ref<const jl::MatrixXld>& b, jl::MatrixXld& U, jl::MatrixXld& y) {
		U = A;
		y = b;
		long double s = 0;
		long long n = U.rows();
		for(long long p = 0; p < n; p++) {
			jl::pivot(U, y, p, p);
			s = 1/U(p,p);
			y(p,0) = s * y(p,0);
			for(long long c = p; c < n; c++)
				U(p,c) = s * U(p,c);
			for(long long r = p + 1; r < n; r++) {
				s = -1 * U(r,p);
				y(r,0) = y(r,0) + s * y(p,0);
				for(long long c = p; c < n; c++) {
					U(r,c) = U(r,c) + s * U(p,c);
				}
			}
		}
	}

	jl::MatrixXld forwardSub(const Eigen::Ref<const jl::MatrixXld>& A, const Eigen::Ref<const jl::MatrixXld>& b) {
		jl::MatrixXld U = A;
		jl::MatrixXld y = b;
		long double s = 0;
		long long n = U.rows();
		for(long long p = 0; p < n; p++) {
			jl::pivot(U, y, p, p);
			s = 1/U(p,p);
			y(p,0) = s * y(p,0);
			for(long long c = p; c < n; c++)
				U(p,c) = s * U(p,c);
			for(long long r = p + 1; r < n; r++) {
				s = -1 * U(r,p);
				y(r,0) = y(r,0) + s * y(p,0);
				for(long long c = p; c < n; c++) {
					U(r,c) = U(r,c) + s * U(p,c);
				}
			}
		}
		return y;
	}


}

#endif