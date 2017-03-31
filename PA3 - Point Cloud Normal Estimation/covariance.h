#if !defined(__JLCOVARIANCE_H__)
#define __JLCOVARIANCE_H__
#include<vector>
#include<algorithm>
#include<iomanip>
#include<Dense>
#include"nmutility.h"

namespace nm {
//#define _NMDEBUG
Eigen::MatrixXd CovarianceMatrix(const Eigen::Ref<const Eigen::MatrixXd>&,long long, long long, long long = 3);
Eigen::MatrixXd CovarianceMatrix(const Eigen::Ref<const Eigen::MatrixXd>& P,long long index, long long k, long long dimension) {
	std::vector<std::pair<long double, long long>> _v; //vector used to store distance and index of points in P relative to P(index)
	long double _d = 0.0f; //variable to store distance 
	for(ll i = 0; i < P.rows(); i++) { //loop through P pushing pairs (distance, index) into vector _v
		_d = 0.0f;
		for(int p = 0; p < dimension; p++) //loop through the three columns x, y, and z
			_d += std::powl((static_cast<long double>(P(index,p)) - static_cast<long double>(P(i,p))),2); //sum the squares of the differances
		_d = std::sqrtl(_d); //take the square root of the sum of squared differences
		_v.push_back(std::pair<long double, long long>(_d, i)); //push the pair into _v
	}

	std::sort(_v.begin(), _v.end(), std::less<std::pair<long double, long long>>());//sort the pairs by distance
	Eigen::MatrixXd Z;

	Z.resize(k+1, dimension);
	long double* s = new long double[dimension];
	for(ll i = 0; i < dimension; i++)
		s[i] = 0.0f;//initialize array of long doubles that will be used to store the averages of x, y, and z.
	for(ll i = 0; i < k+1; i++) {
		for(int p = 0; p < dimension; p++) {
			Z(i,p) = P(_v[i].second, p);//copy the top k rows into Z
			s[p] += Z(i,p);//sum elements
		}
	}

	for(int i = 0; i < dimension; i++)
		s[i] = s[i] / static_cast<long double>(k+1); //compute the average
	for(ll i = 0; i < k+1; i++) {
		for(int p = 0; p < dimension; p++)
			Z(i,p) -= s[p];//remove the average from the neighboring points in Z
	}
	delete s; //free allocated memory

	Z = Z.transpose() * Z; //compute covariance matrix
	Z = Z * (static_cast<long double>(1.0) / static_cast<long double>(k+1));
	return Z; //return covariance matrix

}
}

#endif