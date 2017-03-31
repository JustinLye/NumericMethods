//Justin Lye CS 3513 - PP4
//code based off of what was presented in class
#include"cubicFunctions.h"
void nm::pp4(long size, long min, long max) {
	nm::randomArray rd; //nm::randomArray defined in randomArray.cpp. Used to generate random floating point values.
	double* valueArray = rd.generate(size, min, max); //array containing random floating point values.
	double* optimizedArray = new double[size]; //output array for optimized cubic root function.
	double* standardArray = new double[size]; //output array for standard cubic root function std::pow(y, (1.0f/3.0f)).
	double* nullArray = new double[size]; //output array for null cubic root function 1.0f.
	double* newtonsArray = new double[size]; //output array for newton's cubic root function.

	double resultVariance[2][10]; //output array to hold difference between optimized/newton's cubic root functions and the standard.

	Eigen::BenchTimer optimizedTimer; //timer to track optimized cubic root function.
	Eigen::BenchTimer standardTimer; //timer to track standard cubic root function.
	Eigen::BenchTimer nullTimer; //timer to track null cubic root function (overhead).
	Eigen::BenchTimer newtonsTimer; //timer to track newton's cubic root function.
	//loop 10 times, calling each version of the cubic root function.
	for(int k = 0; k < 10; k++) {
		optimizedTimer.start(); //start optimized timer.
		for(long i = 0; i < size; i++)
			optimizedArray[i] = nm::cubicOptimized(valueArray[i]); //store optimized cubic root result in output array for each element in the random value array.
		optimizedTimer.stop(); //stop optimized timer.
		standardTimer.start(); //start standard timer.
		for(long i = 0; i < size; i++)
			standardArray[i] = nm::cubicStandard(valueArray[i]); //store standard cubic root result in output array for each element in the random value array.
		standardTimer.stop(); //stop standard timer.
		nullTimer.start(); //start null timer.
		for(long i = 0; i < size; i++)
			nullArray[i] = nm::cubicNull(valueArray[i]); //store null cubic root result in output array for each element in the random value array.
		standardTimer.stop(); //stop null timer.		
		newtonsTimer.start(); //start newton's timer.
		for(long i = 0; i < size; i++)
			newtonsArray[i] = nm::cubicNewtons(valueArray[i]); //sort newton's cubic root result in output array for each element in the random value array.
		newtonsTimer.stop(); //stop newton's timer.
		resultVariance[0][k] = nm::norm1(optimizedArray,standardArray, size); //calculate the sum of abs. difference between optimized output array and standard output array.
		resultVariance[1][k] = nm::norm1(newtonsArray, standardArray, size); //calculate the sum of abs. difference between newton's output array and standard output array.
	}

	nm::printTimer("Optimized Timer", optimizedTimer); //output optimized timer to standard output.
	nm::printTimer("Newton's Timer", newtonsTimer); //output newton's timer to standard output.  
	nm::printTimer("Standard Timer", standardTimer); //output standard timer to standard output.
	nm::printTimer("Null Timer", nullTimer); //output null timer to standard output.
	//Print difference between various versions.
	std::cout << "Variance Results:\n";
	for(int j = 0; j < 2; j++) {
		if(j == 0)
			std::cout << "Optimized Run:\n";
		else
			std::cout << "Newtons Run:\n";
		for(int i = 0; i < 10; i++) 
			std::cout << "run " << i << ": " << resultVariance[j][i] << '\n';

	}
	//reset timers
	optimizedTimer.reset();
	standardTimer.reset();
	nullTimer.reset();
	newtonsTimer.reset();
	//delete output arrays
	delete standardArray;
	delete nullArray;
	delete optimizedArray;
	delete newtonsArray;


}

void nm::printTimer(const char* TimerName, Eigen::BenchTimer& timer) {
	std::ios_base::fmtflags prev_flags = std::cout.flags(); //save current standard output flags.
	std::streamsize prev_precision = std::cout.precision(); //save current standard output precision.
	std::cout << std::fixed << std::setprecision(15); //set output precision to 15 places.
	//print timer results
	std::cout << TimerName << std::endl << "Best : " << timer.best(Eigen::REAL_TIMER) << " seconds in real time; " << timer.best(Eigen::CPU_TIMER) << " seconds in cpu time\n";
	std::cout << "Worst: " << timer.worst(Eigen::REAL_TIMER) << " seconds in real time; " << timer.worst(Eigen::CPU_TIMER) << " seconds in cpu time\n";
	std::cout << "Total: " << timer.total(Eigen::REAL_TIMER) << " seconds in real time; " << timer.total(Eigen::CPU_TIMER) << " seconds in cpu time\n";
	std::cout << std::setprecision(prev_precision);
	std::cout.flags(prev_flags);
}

double nm::norm1(const double* vec1, const double* vec2, long size) {
	double value = 0.0f;
	for(long i = 0; i < size; i++)
		value += std::fabs(vec1[i] - vec2[i]);

	return value;
}
