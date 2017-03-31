#if !defined(__NMUTILITY_H__)
#define __NMUTILITY_H__

namespace nm {
#if !defined(__nmLONGLONG__)
#define __nmLONGLONG__
	typedef long long ll;
#endif

	//returns sqrt(v0*v0 + v1+v1 + v2*v2 + ... + vN*vN)
	long double VectorMagnitude(const Eigen::VectorXd& v) {
		long double _m = 0.0f;
		for(ll i = 0; i < v.size(); i++)
			_m += std::powl(v(i),2.0f);
		return std::sqrtl(_m);
	}

	//retuns v / ||v||
	Eigen::VectorXd Normalize(const Eigen::VectorXd& v) {
		long double _m = VectorMagnitude(v);
		return v * (1.0f/_m);
	}


	//Computes the average of a given Matrix and returns a vector
	//Used in PP3
	//No sources need -- the implemenation is pretty straight forward
	//Not too concerned with efficiency either -- the centorid method is only called once
	Eigen::VectorXd Centroid(const Eigen::MatrixXd& m) {
		Eigen::VectorXd v;
		ll _n = m.cols();
		ll _m = m.rows();
		long double* columnTotals = new long double[_n];
		v.resize(_n);
		for(ll i = 0; i < _n; i++)
			columnTotals[i] = 0;
		for(ll i = 0; i < _m; i++) {
			for(ll j = 0; j < _n; j++)
				columnTotals[j] += m(i,j);
		}
		for(ll i = 0; i < _n; i++)
			v(i,0) = columnTotals[i] / static_cast<long double>(_m);
		delete columnTotals;
		return v;
	}

	//Computes the dot product of two vectors and returns a double
	//Used by FlipNormal in PP3
	long double dot(const Eigen::VectorXd& A, const Eigen::VectorXd& B) {
		long double _d = 0.0f;
		for(nm::ll i = 0; i < A.rows(); i++)
			_d += (A(i,0) * B(i,0));
		return _d;
	}

	//source of algorithm is class notes/PP3 specifications
	bool FlipNormal(const Eigen::VectorXd& P, const Eigen::VectorXd& C, const Eigen::VectorXd& N) {
		return (nm::dot(P - C, N) < 0);
	}




}

#endif