#include"random.h"
//Justin Lye CS 3513 - PP4
//srand was not working for the large values used in the program.
//I am using a linear congruential generator from the standard library std::stdmin_rand.
/*random number generation:
	r() -- get new randomly generated whole number
	(r() mod (maxElemnentValue - minElementValue) + minElementValue) - minElementValue -- get whole number from remainder in the desired range less minimum value
	std::rand()/RAND_MAX -- get random floating point number x where 0 < x <= 1
	multiple whole number by random floating point x to get random floating point value.
	Add minimum value back to random floating point number to ensure it is in range.
*/

double* nm::randomArray::generate(long arraySize, long minElementValue, long maxElementValue) {
	double* randArray = new double[arraySize];
	for(long i = 0; i < arraySize; i++)
		randArray[i] = static_cast<double>(((rd() % (maxElementValue - minElementValue) + minElementValue) - minElementValue) * (static_cast<double>(std::rand())/static_cast<double>(RAND_MAX)) + static_cast<double>(minElementValue));
	return randArray;		
}
void nm::randomArray::generate(double* outputArray, long arraySize, long minElementValue, long maxElementValue) {
	std::srand(time(0));
	rd.seed(std::rand());
	for(long i = 0; i < arraySize; i++)
		outputArray[i] = static_cast<double>(((rd() % (maxElementValue - minElementValue) + minElementValue) - minElementValue) * (static_cast<double>(std::rand())/static_cast<double>(RAND_MAX)) + static_cast<double>(minElementValue));
}
