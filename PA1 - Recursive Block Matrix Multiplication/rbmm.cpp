#include<iostream>
#include<fstream>
#include"rbmm.h"

using namespace Eigen;
using namespace jl;


enum CASE_SPLIT {M_ONLY, N_ONLY, P_ONLY, M_AND_N, M_AND_P, N_AND_P, ALL};

inline CASE_SPLIT howToSplit(const Ref<MatrixXd>& A, const Ref<MatrixXd>& B);

inline CASE_SPLIT howToSplit(const Ref<const MatrixXd>& A, const Ref<const MatrixXd>& B) {
	bool m = (A.rows() > 2), n = (A.cols() > 2), p = (B.cols() > 2);
	if(m && !n && !p)
		return M_ONLY;
	if(!m && n && !p)
		return N_ONLY;
	if(!m && !n && p)
		return P_ONLY;
	if(m && n && !p)
		return M_AND_N;
	if(m && !n && p)
		return M_AND_P;
	if(!m && n && p)
		return N_AND_P;
	return ALL;
}

std::ofstream _outFile;

//Used the idea to have const Ref<const MatrixXd> from discussion post on D2L.
//I was have problems passing an Eigen::Block<MatrixXd> to a non-const parameter.

void rbmm(const Ref<const MatrixXd> A, const Ref<const MatrixXd> B, Ref<MatrixXd> C) {
	if(A.size() <= 2 && B.size() <= 2) {
		C += A * B;
	} else {
		int m0 = 0, m1 = 0, n0 = 0, n1 = 0, p0 = 0, p1 = 0;
		int m = A.rows(), n = A.cols(), p = B.cols();
		m0 = std::ceil(m/2);
		m1 = m - m0;
		n0 = std::ceil(n/2);
		n1 = n - n0;
		p0 = std::ceil(p/2);
		p1 = p - p0;

		switch (howToSplit(A,B))
		{
		case ALL:
			rbmm(A.block(0,0,m0,n0), B.block(0,0,n0,p0), C.block(0,0,m0,p0));
			rbmm(A.block(0,n0,m0,n1), B.block(n0,0,n1,p0), C.block(0,0,m0,p0));
			rbmm(A.block(0,0,m0,n0), B.block(0,p0,n0,p1), C.block(0,p0,m0,p1));
			rbmm(A.block(0,n0,m0,n1), B.block(n0,p0,n1,p1), C.block(0,p0,m0,p1));
			rbmm(A.block(m0,0,m1,n0), B.block(0,0,n0,p0), C.block(m0,0,m1,p0));
			rbmm(A.block(m0,n0,m1,n1), B.block(n0,0,n1,p0), C.block(m0,0,m1,p0));
			rbmm(A.block(m0,0,m1,n0), B.block(0,p0,n0,p1), C.block(m0,p0,m1,p1));
			rbmm(A.block(m0,n0,m1,n1), B.block(n0,p0,n1,p1), C.block(m0,p0,m1,p1));
			break;
		case M_ONLY:
			rbmm(A.block(0,0,m0,n), B, C.block(0,0,m0,p));
			rbmm(A.block(m0,0,m1,n), B, C.block(m0,0,m1,p));
			break;
		case N_ONLY:
			rbmm(A.block(0,0,m,n0), B.block(0,0,n0,p), C);
			rbmm(A.block(0,n0,m,n1), B.block(n0,0,n1,p), C);
			break;
		case P_ONLY:
			rbmm(A, B.block(0,0,n,p0), C.block(0,0,m,p0));
			rbmm(A, B.block(0,p0,n,p1), C.block(0,p0,m,p1));
			break;
		case M_AND_N:
			rbmm(A.block(0,0,m0,n0), B.block(0,0,n0,p), C.block(0,0,m0,p));
			rbmm(A.block(0,n0,m0,n1), B.block(n0,0,n1,p), C.block(0,0,m0,p));
			rbmm(A.block(m0,0,m1,n0), B.block(0,0,n0,p), C.block(m0,0,m1,p));
			rbmm(A.block(m0,n0,m1,n1), B.block(n0,0,n1,p), C.block(m0,0,m1,p));
			break;
		case M_AND_P:
			rbmm(A.block(0,0,m0,n), B.block(0,0,n,p0), C.block(0,0,m0,p0));
			rbmm(A.block(0,0,m0,n), B.block(0,p0,n,p1), C.block(0,p0,m0,p1));
			rbmm(A.block(m0,0,m1,n), B.block(0,0,n,p0), C.block(m0,0,m1,p0));
			rbmm(A.block(m0,0,m1,n), B.block(0,p0,n,p1), C.block(m0,p0,m1,p1));
			break;
		case N_AND_P:
			rbmm(A.block(0,0,m,n0), B.block(0,0,n0,p0), C.block(0,0,m,p0));
			rbmm(A.block(0,n0,m,n1), B.block(n0,0,n1,p0), C.block(0,0,m,p0));
			rbmm(A.block(0,0,m,n0), B.block(0,p0,n0,p1), C.block(0,p0,m,p1));
			rbmm(A.block(0,n0,m,n1), B.block(n0,p0,n1,p1), C.block(0,p0,m,p1));
			break;
		}
	}
}

int main(int argc, char* argv[]) try {
	
	jl::inputBuffer _buffer;
	MatrixXd* A = jl::readInput(argv[1], _buffer);
	MatrixXd* B = jl::readInput(argv[2], _buffer);
	
	if(A->cols() != B->rows())
		throw std::exception("failure:\t matrices are incompatible for multiplication");
	MatrixXd C(A->rows(), B->cols());
	_outFile.open(argv[3]);
	C.setZero();
	rbmm(*A, *B, C);
	_outFile << C;
	_outFile.close();
	delete A;
	delete B;
	return 0;
} catch(std::exception& e) {
	std::cout << e.what() << '\n';
}



