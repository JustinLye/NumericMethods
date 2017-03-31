//Justin Lye CS 3513 - PP4
#include<iostream>
#include"random.h" //random array generater is defined in random.cpp
#include"cubic.h" //cubic root functions are defined in cubic.h.  
#include"cubicFunctions.h" //the looping through a random array calculating the cubic roots using various implementations is defined in cubicFunctions.cpp
int main(int argc, char* argv[]) try {
	if(argc < 4)
		nm::pp4(10000, 1000000, 100000000);
	else
		nm::pp4(std::atol(argv[1]),std::atol(argv[2]),std::atol(argv[3]));
} catch(std::exception& e) {
	std::cout << e.what() << '\n';
}
