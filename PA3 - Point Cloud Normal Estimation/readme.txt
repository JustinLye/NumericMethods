Justin Lye
CS 3513 - SPRING 2016
PP3 - Principle Component Analysis

------------------------------------------------------------------------------------------------------------------------------------------------------------

NOTICE: I attempted the extra credit. Please review the RUNNING THE PROGRAM section to find how to run using the Inverse Power
        implementation.

------------------------------------------------------------------------------------------------------------------------------------------------------------

HOW TO COMPILE:

Please use VS2012 or higher to compile.

1. Open the Visual Studio Command Prompt. To open the VS Command Prompt take the following steps:
       > Open VS2012
       > Click on the TOOLS drop down menu -- located near the top of the screen
       > Click on the Visual Studio Command Prompt -- located in the bottom part of the list
2. With the Visual Studio Command Prompt open, navigate to the directory/folder containing the source documents.
   Use the cd command to do this.

   For example if the current directory is c:\program files (x86)\microsoft visual studio 11.0\vc\bin,
   then VS Command Prompt will look like this:

       c:\program files (x86)\microsoft visual studio 11.0\vc\bin>_

   If the directory/folder containing the source documents is located in the directory
   c:\Users\Justin Lye\Documents\CS3513 PP3 - Justin Lye, then the following command would be used:

       c:\program files (x86)\microsoft visual studio 11.0\vc\bin>cd "c:\Users\Justin Lye\Documents\CS3513 PP3 - Justin Lye"

   After pressing enter the VS Command Prompt will look like this:

       c:\Users\Justin Lye\Documents\CS3513 PP3 - Justin Lye>_

3. Ensure the contents of the submitted directory are complete. The contents of an directory/folder can be
   viewed using the dir command. Type dir and press enter (in the VS Command Prompt):

       c:\Users\Justin Lye\Documents\CS3513 PP3 - Justin Lye>dir   

   The directory should contain all of the following files & folder:
       > Eigen (this is a folder)
       > cholesky.h
       > covariance.h
       > iterativeMethods.h
       > matrixBuffer.h
       > nmutility.h
       > numMethod.h
       > powermethod.h
       > pp3.cpp
       > readme.txt (not needed to compile)

   Other files may also be in the directory, however, all of the files listed above must be in the directory for the
   program to compile.

4. Finally, compile the pp3.cpp compilation unit using the cl command. Continuing with the example used above the
   following command would be entered to compile the code:

       c:\Users\Justin Lye\Documents\CS3513 PP3 - Justin Lye>cl pp3.cpp /IEigen /EHsc
------------------------------------------------------------------------------------------------------------------------------------------------------------

RUNNING THE PROGRAM:

pp3.exe <input file path> <output file path> <k> [Implementation]

<input file path>  (Required Input) the path of the input file containing to point cloud
<output file path> (Required Input) the path of the output file containing the point cloud and the normalized eigenvectors
<k>                (Required Input) the number of neighboring points that should be used when calculating the covariance matrix.
[Implementation]   (Optional Input) enter a 1 to use the Inverse Power Method implementation. I attempted the extra credit
                    offered in the assignment specification document. To use the Inverse Power Method I created to find
                    the eigenvectors enter a one as the final argument. Not entering an argument (i.e. leaving it blank)
                    will use the Eigen::EigenSolver class to find the eigenvectors. This implementation uses the Eigen Library
                    to calculate the eigenvectors.

Input Notation: <required input>, [optional input]

Suggestion: If the input files are located in the same directory as pp3.exe, then only the name of the input file has to be
entered. The same goes for the output file. However, if the output file does not exist one will be created.

When the program is executed, a notification will appear on the command screen indicating which implemenation is being used.
When to program has completed, a notification indicating how long the process took will be displayed.

------------------------------------------------------------------------------------------------------------------------------------------------------------

PROGRAM OVERVIEW

	>Read input file passed as the second command line argument and store file content in a matrix.
	>Read neighborhood size given in the fourth command line argument and store in long long variable k
	>Calculate centriod of input file content and store in vector.
	>If a fifth command line argument is passed, then if that argument is equal to 1 use alternate Inverse Power
	 Method. Else, use Eigen library method.
	>Store the output of either the Inverse Power Method (pp3InversePower) or the Eigen Library Method
	 (pp3RequiredVersion) in a matrix variable called OutputMatrix.
	>Open the output file passed as the third command line argument, write the output matrix to the file, then
	 close the file.


------------------------------------------------------------------------------------------------------------------------------------------------------------
SOURCE REFERENCES:

Inverse Power Method Algorithm

	I based my implementation of the Inverse Power Method from a combination of
	a power iteration method I found in a Linear Algebra textbook and the pseudocode
	for the inverse power method found in the course textbook. Specifically, I did not understand
	the description of the power method entirely. I was reviewing the Eigenvectors chapter
	in textbook for a Linear Algebra course I took previously.
	Section 4.6 'Iterative Methods for Computing Eigenvalues' (the section was not covered in
	my Linear Algebra coures) gave a detailed description of the Power Method. I was able to 
	understand (to some degree) why the power method works. The Linear Algebra textbook also states
	a single step can be replaced by gaussian elmination to implement the Inverse Power Method (i.e.
	find the smallest eigenvalue). I used cholesky decomposition and forward + backward substitution
	to implement a solution of the system Ax = y (solving for x). The idea that I could use
	cholesky decomposition is from the course slides.

	File:		powermethod.h
        Lines:		64 to 85
	Function:	Eigen::VectorXd InversePowerMethod(const Eigen::Ref<Eigen::MatrixXd>& A)

        Source:		Linear Algebra Textbook
                	Poole, D. (2014). Linear algebra a modern introduction (4th ed.). Boston, MA: Cengage Brooks/Cole.
	                Top of Page 315 and Top of Page 317

			Course Textbook
			Solomon, J. (n.d.). Numerical algorithms: Methods for computer vision, machine learning, and graphics (1st ed.).
			Page 118 Fig. 6.4 (b)
		
			Course Slides
			https://webmail.cs.okstate.edu/svn/cs3513/2016Spring/Handouts/symEigen-pres.pdf
			Slide Number 31


Covergence Test

	I had original thought of a way to test for convergence by checking the absolute difference between each
	element of the EigenVector(at k iterations) and the Eigenvector (at k - 1 iterations). If the difference
	of each element was less than std::numeric_limits<long double>::epsilon(), then the Covergence Method would
	return true.
	
	However, I ran into a problem where, in one of the example files, convergence was not occuring
	(even after 200 million iterations!!).

	As a quick fix I decided to return the eigenvector if the iterations had exceeded 200 thousand
        and covergence had still not occurred (it appeared that this was producing accurate results; however,
	this could be very expensive.
	I was researching std::numeric_limits and came across an example that tested if two doubles were equal
	for a some given units of precision. I found that the eigenvalue I was calculating actually two
	units of precision more than the eigenvalue calculated using the Eigen Library.

	So, after spending a couple of hours trying to solve the problem on my own I decided to implement
	the 'Almost Equal' approach I found online. I calculate the distance between the two vectors then
	compare that to the long double epsilon * given units of precision.

	File:		powermethod.h
	Lines:		21 to 44
	Function:	bool Coverged(const Eigen::VectorX& A, const Eigen::VectorXd& B, int UnitsOfPrecision)

	Source:		http://en.cppreference.com/w/cpp/types/numeric_limits/epsilon	



Backward Substitution Algorithm

	I based my implementation of the Backward Substitution Method from a combination of
	the pseudocode given in the course textbook and lecture slides/notes.

	File:		iterativeMethods.h
	Lines:		24 to 33
	Function:	Eigen::VectorXd BackwardSub(const Eigen::Ref<Eigen::MatrixXd>& U, const Eigen::VectorXd& y)
	
	Source:		Course Textbook
			Solomon, J. (n.d.). Numerical algorithms: Methods for computer vision, machine learning, and graphics (1st ed.).
			Page 57 Fig. 3.2


	
Forward Substitution Algorithm

	I based my implementation of the Forward Substitution Method from a combination of
	the psuedocode given in the course textbook and lecture slides/note.   

	File:		iterativeMethods.h
	Lines:		39 to 50
	Function:	Eigen::VectorXd ForwardSub(const Eigen::Ref<Eigen::MatrixXd>& L, const Eigen::Vector& b)
	
	Source:		Course Textbook
			Solomon, J. (n.d.). Numerical algorithms: Methods for computer vision, machine learning, and graphics (1st ed.).
			Page 56 Fig. 3.1


Cholesky Factorization Algorithm

	I based my implementation of the Cholesky Factorization Method from a combination of
	the psuedocode given in the course textbook and lecture slides/notes.

	File:		cholesky.h
	Lines:		9 to 31
	Function:	Eigen::MatrixXd choleskyDecomp(const Eigen::Ref<const Eigen::MatrixXd>& G)
	
	Source:		Course Textbook
			Solomon, J. (n.d.). Numerical algorithms: Methods for computer vision, machine learning, and graphics (1st ed.).
			Page 80 Fig. 4.6

Covariance

	I based my implementation of the Covariance matrix method from the course slides/notes.

	File:		covariance.h
	Lines:		12 to 49
	Function:	CovarianceMatrix(const Eigen::Ref<const Eigen::MatrixXd>& P, long long index, long long k, long long dimension = 3)
	
	Source:		Course Slides
			https://webmail.cs.okstate.edu/svn/cs3513/2016Spring/Handouts/symEigen-pres.pdf
			Slide 6 through 8
	