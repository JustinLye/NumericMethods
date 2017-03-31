//Justin Lye CS 3513 PP4
#if !defined(__NM_RANDOM_H__)
#define __NM_RANDOM_H__

#include<ctime>
#include<random>

namespace nm {
	class randomArray {
	private:
		std::minstd_rand rd;
	public:
		randomArray() { std::srand(time(0)); rd.seed(std::rand()); }
		double* generate(long arraySize, long minElementValue, long maxElementValue);
		void generate(double* outputArray, long arraySize, long minElementValue, long maxElementValue);
	};
}

#endif
