#if !defined(__jlpowermethod_h__)
#define __jlpowermethod_h__

#include<cmath>
#include<Dense>
#include"nmutility.h"
namespace nm {

	long long AbsoluteMax(const Eigen::Ref<const Eigen::VectorXd>& y) {
		long double m = 0;
		long long _index = 0;
		for(long long i = 0; i < y.size(); i++) {
			if(std::fabsl(y(i)) > m) {
				m = std::fabsl(y(i));
				_index = i;
			}
		}
		return _index;
	}


	bool Coverged(const Eigen::VectorXd& A, const Eigen::VectorXd& B, int UnitsOfPrecision) {
		/* I ran into a problem where, in one of the example files pp3TestC2.txt, convergence was not occuring @ point 387 (even after 200 million iterations!!)
		   My original method returned true if for each element in the vectors A and B:
		     std::fabsl(A(i,0) - B(i,0)) < std::numeric_limits<long double>::epsilon();
		   
		   As a quick fix I would return the eigenvector if the iterations had exceeded 200 thousand. The result was correct, but the time to run could be very long for some of the point clouds
		   I was researching std::numeric_limits and came across an example that tested if two doubles were equal for a some given units of precision.
		   I found that the eigenvalue I was calculating actually had two more units of precision more than the eigenvalue calculated using the Eigen Library.
		   So, after spending a couple of hours trying to solve the problem on my own I decided to implement the 'Almost Equal' approach I found online.

		   The source of the code on line 53 is located at http://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
		 */
		long double _distance = 0.0f;
		for(int i = 0; i < A.rows(); i++)
			_distance += std::powl(A(i,0) - B(i,0), 2.0f);
		_distance = std::sqrtl(_distance);
		return _distance < std::numeric_limits<long double>::epsilon() * UnitsOfPrecision;
	}


	/*Returns Eigen::VectorXd containing the eigenvector associated with the smallest eigenvalue of the matrix passed as an argument
	  The arugment matrix must be a symmetric positive definate matrix.

	  See readme file for algorithm source references

	*/
	Eigen::VectorXd InversePowerMethod(const Eigen::Ref<Eigen::MatrixXd>& PositiveDefMatrix, int UnitsOfPrecision = 15) {
		
		//DECLARE AND INITIALIZE VARIABLES
		Eigen::VectorXd ForwardSubOutputVector;
		Eigen::VectorXd BackwardSubOutputVector;
		Eigen::VectorXd OutputVector;
		Eigen::VectorXd PrevIterationOutputVector;
		Eigen::MatrixXd LowerTrianglarMatrix;
		Eigen::MatrixXd UpperTrianglarMatrix;
		long double EigenValue = 0.0f;/*...............................................Please note the name can be miss leading b/c the eigenvalue is actually 1/EigenValue*/
		long double PrevIterationEigenValue = 0.0f;
		nm::ll IndexToLargestVectorElement = 0;
		nm::ll TotalVectorRows = PositiveDefMatrix.rows();

		BackwardSubOutputVector.resize(TotalVectorRows);/*...............................Allocate space for vector that will contain the solution to Ax(k) = y(k - 1) [solving for x(k)]*/
		BackwardSubOutputVector.fill(1.0f); /*....................................................Initialize vector x(0) to [1, 1, 1,...,n]*/
		OutputVector = ForwardSubOutputVector = BackwardSubOutputVector;/*....................................Initialize vectors*/
		LowerTrianglarMatrix = nm::choleskyDecomp(PositiveDefMatrix);/*............................Decompose A = LU via Cholesky Decomposition*/
		UpperTrianglarMatrix = LowerTrianglarMatrix.transpose();/*.................................Initialize matrix U = LT*/

		//ITERATE UNITL EIGENVECTOR (i.e. OUTPUTVECTOR) CONVERGES
		do {
			ForwardSubOutputVector = nm::ForwardSub(LowerTrianglarMatrix, OutputVector);/*...............Solve the system Ly = b for y --> LowerTriangluarMatrix * ForwardSubOutputVector = OutputVector*/
			BackwardSubOutputVector = nm::BackwardSub(UpperTrianglarMatrix, ForwardSubOutputVector);/*...Solve the system Ux = y for x --> UpperTriangluarMatrix * OutputVector = BackwardSubOutputVector*/
			IndexToLargestVectorElement = AbsMaxIndex(BackwardSubOutputVector);/*........................Find the index of the absolute largest/most dominate element in the solution vector*/
			EigenValue = BackwardSubOutputVector(IndexToLargestVectorElement);/*.........................Assign the dominate element to the EigenValue variable*/
			PrevIterationOutputVector = OutputVector; /*.................................................Store the output vector be for update --> thus y(k) can be tested against y(k-1) for convergence {i.e. OutputVector and PrevIterationOutputVector}*/
			for(nm::ll row = 0; row < TotalVectorRows; row++)/*..........................................Loop through BackwardSubOutputVector*/
				OutputVector(row) = (1/EigenValue) * BackwardSubOutputVector(row);/*.....................Ensure the largest element in the OutputVector is == 1. This is acceptable be cause the scale of the vector does not change it's direction/characteristics*/
		}while(!Coverged(PrevIterationOutputVector, OutputVector, UnitsOfPrecision));/*..................Continue to loop until convergence to the given units of precision has occurred.*/

		return nm::Normalize(OutputVector);/*......................................................Return normalized eigenvector*/


	}


}

#endif