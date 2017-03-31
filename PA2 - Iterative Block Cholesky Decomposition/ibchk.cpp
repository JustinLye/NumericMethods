#include<iostream>
#include<iomanip>
#include<fstream>
#include<Dense>
#include"rbmm.h"
#include"forwsub.h"
#include"cholesky.h"

#define DEBUGGING 

//this function is just used to help find the largest element difference in A*AT and L*LT
#if defined(DEBUGGING)
	long double findLargestDeviation(const jl::MatrixXld& L) {
		long double max = 0;
		for(long long i = 0; i < L.size(); i++)
			if(max < std::fabsl(L(i))) max = std::fabsl(L(i));
		return max;
	}
#endif

int main(int argc, char* argv[]) try {

	jl::inputBuffer _buffer; //declare input buffer to assist in reading data
	jl::MatrixXld* G = jl::gramm(argv[1], _buffer); //read input data and return gramm matrix
	jl::MatrixXld L; //declare matrix L, which will be the output
	jl::MatrixXld L10T; //delcare matrix L10T, will be used to represent the upper right block of matrix L
	jl::MatrixXld L10; //declare matrix L10, will be used to represent the lower left block of matrix L
	jl::MatrixXld L00; //declare matrix L00, will be used to represent the upper left block of matrix L

#if defined(DEBUGGING)
	jl::MatrixXld A = *G; //if debugging, keep a copy of the original matrix for comparison at the end of the output
#endif

	L.resize(G->rows(), G->rows()); //resize L to size of the gramm matrix
	L.setZero(); //initialize the elements of L to zero
	
	long long b = 8; //declare and intialize b to a default count of 8
	long long n = G->rows(); //declare and intialize n to the total rows of the gramm matrix. n will be used to establish the appropriate size of blocks.
	long long iterations = 1; //declare and intialize interations = 1. interations will be used to establish the appropriate size of blocks.

	if(argc == 4) b = std::atol(argv[3]); //if a fourth argument has been passed, assume that argument is the block size b.
	
	if(n <= b)
		L = jl::choleskyDecomp(*G); //if the block size is greater than or equal to the gramm matrix size then, use unblocked cholesky decomposition
	else { //if the block size is less than the size of the gramm matrix the iterate down the diagonal of the gramm matrix by block, solving for L10 & L11 each iteration
		L00.resize(b,b); //resize L00. Unlike L10/L10T & L11 matrix L00 will not change in size. It's size is set once here.
		do {
			//with each iteration, sub matrices L10, L10T, and L11 will need to be resizeD appropriately before assigning values to them
			L10.resize(n - iterations*b,b);
			L10.setZero(); //reset L10
			L10T.resize(b, n - iterations * b);
			L10T.setZero(); //reset L10T
			L00.setZero(); //reset L00

			L00 = jl::choleskyDecomp(G->block((iterations - 1) * b, (iterations - 1) * b, b, b)); //decompose L00 using unblocked cholesky factorization
			for(long long i = 1; i <= (n - iterations * b); i++) //use forward substitution to solve for columns of L10T
				L10T.block(0,i-1,L10T.rows(),1) = jl::forwardSub(L00, G->block((iterations - 1)*b, (iterations * b - 1) + i, b, 1));
			L10 = L10T.transpose(); //set L10 == to its Transpose transposed
			G->block(iterations*b,iterations*b, (n-iterations*b), (n-iterations*b)) -= L10*L10T; //D = L11 - L10 * L10T
			L.block((iterations-1)*b, (iterations-1)*b, b, b) = L00; //update L (upper left block)
			L.block(iterations*b, (iterations - 1) * b, (n-iterations*b), b) = L10;
			iterations++;
			if((n - iterations*b) <= 0)  //if the remaining elements are less than the block size, use unblock cholesky for the remaining elements.
				L.block((iterations-1)*b,(iterations-1)*b, n - (iterations-1)*b, n - (iterations - 1) * b) = jl::choleskyDecomp(G->block((iterations-1)*b,(iterations-1)*b,n - (iterations-1) * b, n - (iterations - 1) * b));
		} while((n - iterations*b > 0));
	}
	
	std::ofstream _outfile; //declare output file
	_outfile.open(argv[2]); //open output file
	_outfile << std::setprecision(20) << std::scientific << L; //output resulting L matrix
	_outfile.close(); //close output file

#if defined(DEBUGGING)
	std::cout << "Gramm Matrix:\n\n" << A << "\n\n" << "L*LT:\n\n" << L*L.transpose() << "\n\n";
	std::cout << "Largest Deviation: " << findLargestDeviation(A - (L*L.transpose())) << "\n\n" << A - (L*L.transpose()) << '\n';
#endif

	delete G; //release memory at G
	return 0;
} catch(std::exception& e) {
	std::cout << e.what() << '\n';
}
