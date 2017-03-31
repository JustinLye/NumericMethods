#include<iostream>
#include<iomanip>
#include<fstream>
#include<cmath>
#include<ctime>
#include"numMethod.h"

Eigen::MatrixXd pp3InversePower(Eigen::MatrixXd*, nm::ll, Eigen::VectorXd&);
Eigen::MatrixXd pp3RequiredVersion(Eigen::MatrixXd*, nm::ll, Eigen::VectorXd&);
Eigen::VectorXd GetEigenVector(Eigen::EigenSolver<Eigen::MatrixXd>&, Eigen::MatrixXd&); //this method is called by pp3RequiredVersion. It uses the Eigen library to find the Eigenvector associated with the smallest Eigenvalue

int main(int argc, char* argv[]) try {

	nm::inputBuffer _buffer; //buffer used to read input file
	Eigen::MatrixXd* PointCloud = nm::readInput(argv[1], _buffer); //read the input file
	nm::ll k = std::atol(argv[3]); //k points
	Eigen::VectorXd centroid = nm::Centroid(*PointCloud); //calculate the centroid for the entire point cloud. [nm::Centriod is defined in nmutility.h]
	Eigen::MatrixXd OutputMatrix; //matrix to be output to file
	std::ofstream outFile; //ofstream object used to write to output file

	bool UseInversePowerMethod = false;
	if(argc >= 5)
		UseInversePowerMethod = std::atoi(argv[4]) == 1;
	
	if(UseInversePowerMethod)
		OutputMatrix = pp3InversePower(PointCloud, k, centroid);
	else
		OutputMatrix = pp3RequiredVersion(PointCloud, k, centroid);


	outFile.open(argv[2]);
	outFile << OutputMatrix;
	outFile.close();
	delete PointCloud;

} catch(std::exception& e) {
	std::cout << e.what() << '\n';
}

Eigen::MatrixXd pp3InversePower(Eigen::MatrixXd* PointCloud, nm::ll k, Eigen::VectorXd& Centroid) {
	
	//DECLARE AND INITIALIZE VARIABLES
	clock_t TimeAtStart = clock(); /*....................................................................Get current time -- I wanted to see how long this was taking and if the time could be shorted*/
	Eigen::MatrixXd OutputMatrix;/*.......................................................................Matrix that will contain the point and normals for each row (i.e. the output) -- will be returned to the calling function*/
	Eigen::MatrixXd CovarianceMatrix;/*...................................................................Matrix that will contain the covariance matrix for each point -- it will store the output of the Covariance method for each iteration (i.e. point)*/
	Eigen::VectorXd NormalsVector;/*......................................................................Vector that will contain the normals for each point -- it will store the output of the Inverse Power Method of each iteration (i.e. point)*/
	Eigen::VectorXd PointsVector;/*.......................................................................Vector that will contain the x, y, and z points for each iteration -- it will be used to copy info from the original matrix PointCloud into the OutputMatrix*/
	nm::ll totalPointsInCloud = PointCloud->rows();/*.....................................................long long that will contain the total number of points (i.e. rows in the PointCloud matrix) that need to be processed*/

	OutputMatrix.resize(totalPointsInCloud, 6);/*.........................................................Resize the OutputMatrix so that information can be stored in the matrix*/
	PointsVector.resize(3);/*.............................................................................Resize the PointsVector. The sole reason for the PointsVector is to pass the x, y, and z points to the flip normal method*/

	//Inform the users that the Inverse Power Method will be used to calculate the eigenvector
	std::cout << "Inverse Power Method is used to find eigenvector (Extra Credit)\n\n";

	//PROCESS POINT CLOUD
	for(nm::ll currentPoint = 0; currentPoint < totalPointsInCloud; currentPoint++) {//Start PointCloud Iteration -- Iterate through each point in the cloud (outer most loop)
		
		//GET COVARIANCE MATRIX
		CovarianceMatrix = nm::CovarianceMatrix(*PointCloud, currentPoint, k);/*............................Returns the covariance matrix of the current point 
																			                                and it's k closest neighbors in the PointCloud
																											nm::CovarianceMatrix is defined in covariance.h*/
		//Get EIGENVECTOR
		NormalsVector = nm::InversePowerMethod(CovarianceMatrix,18);/*.........................................Calculate the EigenVector
																											nm::InversePowerMethod is defined in powermethod.h*/
		//COPY X, Y, Z POINTS INTO OUTPUT MATRIX
		for(int pointCloudColumn = 0; pointCloudColumn < 3; pointCloudColumn++)/*...........................Copy Points (x,y,z) into the OutputMatrix and PointsVector*/
			OutputMatrix(currentPoint, pointCloudColumn) = 
			PointsVector(pointCloudColumn) = PointCloud->operator()(currentPoint, pointCloudColumn);
		
		//REORIENT NORMALIZED EIGENVECTOR
		if(nm::FlipNormal(PointsVector, Centroid, NormalsVector))/*.........................................Reorient sign of eigenvector if needed nm::FlipNormal is defined in nmutility.h*/
			NormalsVector *= -1;

		//COPY NORMALIZED EIGENVECTOR INTO OUTPUT MATRIX
		for(int pointCloudColumn = 0; pointCloudColumn < 3; pointCloudColumn++)/*...........................Copy normalized EigenVector into the last three columns of the OutputMatrix*/
			OutputMatrix(currentPoint, pointCloudColumn + 3) = NormalsVector(pointCloudColumn,0);

	}//End PointCloud Iteration

	std::cout << std::setprecision(18);
	std::cout << "Total time required (seconds):\t" << static_cast<long double>((clock() - TimeAtStart)) / static_cast<long double>(CLOCKS_PER_SEC) << "\n\n";

	return OutputMatrix; //return the output matrix when all points have been processed
}

Eigen::MatrixXd pp3RequiredVersion(Eigen::MatrixXd* PointCloud, nm::ll k, Eigen::VectorXd& Centroid) {
	
	//DECLARE AND INITIALIZE VARIABLES
	clock_t TimeAtStart = clock(); /*....................................................................Get current time -- I wanted to see how long this was taking and if the time could be shorted*/
	Eigen::MatrixXd OutputMatrix;/*.......................................................................Matrix that will contain the point and normals for each row (i.e. the output) -- will be returned to the calling function*/
	Eigen::MatrixXd CovarianceMatrix;/*...................................................................Matrix that will contain the covariance matrix for each point -- it will store the output of the Covariance method for each iteration (i.e. point)*/
	Eigen::VectorXd NormalsVector;/*......................................................................Vector that will contain the normals for each point -- it will store the output of the Inverse Power Method of each iteration (i.e. point)*/
	Eigen::VectorXd PointsVector;/*.......................................................................Vector that will contain the x, y, and z points for each iteration -- it will be used to copy info from the original matrix PointCloud into the OutputMatrix*/
	Eigen::EigenSolver<Eigen::MatrixXd> StandardEigenSolver;/*............................................Special matrix from the Eigen Library that is used to compute Eigenvalues & Eigenvectors of a given matrix*/
	nm::ll totalPointsInCloud = PointCloud->rows();/*.....................................................long long that will contain the total number of points (i.e. rows in the PointCloud matrix) that need to be processed*/

	OutputMatrix.resize(totalPointsInCloud, 6);/*.........................................................Resize the OutputMatrix so that information can be stored in the matrix*/
	PointsVector.resize(3);/*.............................................................................Resize the PointsVector. The sole reason for the PointsVector is to pass the x, y, and z points to the flip normal method*/

	//Inform the users that Eigen Library will be used to calculate the eigenvector (Required Version)
	std::cout << "Eigen::EigenSolver is used to find eigenvector (Required Version)\n\n";

	//PROCESS POINT CLOUD
	for(nm::ll currentPoint = 0; currentPoint < totalPointsInCloud; currentPoint++) {//Start PointCloud Iteration -- Iterate through each point in the cloud (outer most loop)
		
		//GET COVARIANCE MATRIX
		CovarianceMatrix = nm::CovarianceMatrix(*PointCloud, currentPoint, k);/*............................Returns the covariance matrix of the current point 
																			                                and it's k closest neighbors in the PointCloud
																											nm::CovarianceMatrix is defined in covariance.h*/
		//Get EIGENVECTOR
		NormalsVector = GetEigenVector(StandardEigenSolver, CovarianceMatrix);/*............................Calculate the EigenVector using Eigen Library*/
																											
		//COPY X, Y, Z POINTS INTO OUTPUT MATRIX
		for(int pointCloudColumn = 0; pointCloudColumn < 3; pointCloudColumn++)/*...........................Copy Points (x,y,z) into the OutputMatrix and PointsVector*/
			OutputMatrix(currentPoint, pointCloudColumn) = 
			PointsVector(pointCloudColumn) = PointCloud->operator()(currentPoint, pointCloudColumn);
		
		//REORIENT NORMALIZED EIGENVECTOR
		if(nm::FlipNormal(PointsVector, Centroid, NormalsVector))/*.........................................Reorient sign of eigenvector if needed nm::FlipNormal is defined in nmutility.h*/
			NormalsVector *= -1;

		//COPY NORMALIZED EIGENVECTOR INTO OUTPUT MATRIX
		for(int pointCloudColumn = 0; pointCloudColumn < 3; pointCloudColumn++)/*...........................Copy normalized EigenVector into the last three columns of the OutputMatrix*/
			OutputMatrix(currentPoint, pointCloudColumn + 3) = NormalsVector(pointCloudColumn,0);

	}//End PointCloud Iteration

	std::cout << std::setprecision(18);
	std::cout << "Total time required (seconds):\t" << static_cast<long double>((clock() - TimeAtStart)) / static_cast<long double>(CLOCKS_PER_SEC) << "\n\n";

	return OutputMatrix; //return the output matrix when all points have been processed
}

Eigen::VectorXd GetEigenVector(Eigen::EigenSolver<Eigen::MatrixXd>& Solver, Eigen::MatrixXd& CovarianceMatrix) {
	
	//DECLARE AND INITIALIZE VARIABLES
	long double smallestEigenValue = 0.0f;/*.....................used to find the index of the smallest eigenvalue*/
	int indexOfSmallestEigenValue = 0;/*.........................used as reference to the appropriate column in the Eigenvectors matrix to be computed by the EigenSolver*/
	Eigen::VectorXd OutputVector;/*..............................Vector will store the resulting eigenvector and be returned to the calling function*/

	OutputVector.resize(3);/*....................................Resize the OutputVector*/
	
	//COMPUTE EIGENVALUES & EIGENVECTORS
	Solver.compute(CovarianceMatrix);/*..........................Call Eigen::EigenSolver.compute() method to calculate Eigenvectors and Eigenvalues*/

	//GET THE SMALLEST EIGENVALUE
	smallestEigenValue = Solver.eigenvalues().real().minCoeff();/*...get the minimum real number component of all computed Eigenvalues*/
	
	//FIND THE INDEX OF THE SMALLEST EIGENVALUE
	for(int eigenValueIndex = 0; eigenValueIndex < 3; eigenValueIndex++) {/*......loop through the eigenvalues matrix to find the index associated with the smallest eigenvalue*/
		if(smallestEigenValue == Solver.eigenvalues().real().operator()(eigenValueIndex,0)) {
			indexOfSmallestEigenValue = eigenValueIndex;/*........if a match is found, then assign the current index*/
			break;/*..............................................if a match is found, then exit the loop*/
		}
	}

	//COPY THE EIGENVECTOR INTO THE OUTPUT VECTOR
	for(int eigenVectorRow = 0; eigenVectorRow < 3; eigenVectorRow++)/*.........copy the corresponding eigenvector into the output vector*/
		OutputVector(eigenVectorRow, 0) = Solver.eigenvectors().real().operator()(eigenVectorRow, indexOfSmallestEigenValue);

	return OutputVector;
}