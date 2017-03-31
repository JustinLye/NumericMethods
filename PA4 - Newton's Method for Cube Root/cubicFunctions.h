//Justin Lye CS 3513 PP4
#if !defined(__NM_PP4_H__)
#define __NM_PP4_H__

#include<iostream>
#include<iomanip>
#include"random.h"
#include"cubic.h"
#include"BenchTimer.h"

namespace nm {
	void pp4(long, long, long);
	void printTimer(const char* TimerName, Eigen::BenchTimer& timer);
	double norm1(const double*, const double*, long);
}

#endif
