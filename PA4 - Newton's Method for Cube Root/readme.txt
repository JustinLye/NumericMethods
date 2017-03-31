CS 3513 - Numeric Methods - PP4 - Justin Lye



HOW TO COMPILE:

	Use MinGW64 terminal.
	Run the following commands in sequence:
	1. g++ -O2 -Wall --std=c++11 -c random.cpp
	2. g++ -O2 -Wall --std=c++11 -c cubicFunctions.cpp random.o -I.
	3. g++ -O2 -Wall --std=c++11 -c driver.cpp random.o cubicFunctions.o -I.

	4. g++ -O2 -Wall --std=c++11 -o pp4 driver.o random.o cubicFunctions.o



HOW TO RUN:
	pp4 [size] [minimum value] [maximum value]
	If no arguments are entered size will be 10,000, minimum value 1,000,000 and maximum value 100,000,000



PROBLEMS/ISSUES ENCOUNTERED:
	The Eigen::BenchTimer will occasionally produce inconsistent times i.e. best time is -59 seconds or the real time is zero seconds.
	Also
 the best timer for the null timer is consistently 100000000.0000000 (must be an error).



RESULTS:
	The following are results captured from 3 sperate runs of the program:


	
	(1)


	
	Optimized Timer
	Best : 0.000000000000000 seconds in real time; 0.000631488917861 seconds in cpu time
	Worst: 0.000000000000000 seconds in real time; 0.000820819084765 seconds in cpu time
	Total: 0.000000000000000 seconds in real time; 0.006665004446404 seconds in cpu time
	Standard Timer
	Best : 0.000000000000000 seconds in real time; 0.001723487075651 seconds in cpu time
	Worst: 0.015999999999998 seconds in real time; 0.001819317258196 seconds in cpu time
	Total: 0.061999999999983 seconds in real time; 0.034896462573670 seconds in cpu time
	Newton's Timer
	Best : 0.000000000000000 seconds in real time; 0.004087783949217 seconds in cpu time
	Worst: 0.016000000000005 seconds in real time; 0.004243908537319 seconds in cpu time
	Total: 0.031000000000006 seconds in real time; 0.041403886076296 seconds in cpu time
	Null Timer
	Best : 1000000000.000000000000000 seconds in real time; 1000000000.000000000000000 seconds in cpu time
	Worst: 0.000000000000000 seconds in real time; 0.000000000000000 seconds in cpu time
	Total: 0.000000000000000 seconds in real time; 0.000000000000000 seconds in cpu time
	Results:
	Optimized Run:
	run 0: 0.417473
	run 1: 0.417473
	run 2: 0.417473
	run 3: 0.417473
	run 4: 0.417473
	run 5: 0.417473
	run 6: 0.417473
	run 7: 0.417473
	run 8: 0.417473
	run 9: 0.417473
	Newtons Run:
	run 0: 0.450931
	run 1: 0.450931
	run 2: 0.450931
	run 3: 0.450931
	run 4: 0.450931
	run 5: 0.450931
	run 6: 0.450931
	run 7: 0.450931
	run 8: 0.450931
	run 9: 0.450931
	
	(2)
	Optimized Timer
	Best : 0.000000000000000 seconds in real time; 0.000631780188996 seconds in cpu time
	Worst: 0.000000000000000 seconds in real time; 0.000720619747881 seconds in cpu time
	Total: 0.000000000000000 seconds in real time; 0.006575873616384 seconds in cpu time
	Newton's Timer
	Best : 0.000000000000000 seconds in real time; 0.004090696718777 seconds in cpu time
	Worst: 0.015000000000001 seconds in real time; 0.004209537815768 seconds in cpu time
	Total: 0.030000000000001 seconds in real time; 0.041060178977204 seconds in cpu time
	Standard Timer
	Best : 0.000000000000000 seconds in real time; 0.001714166224701 seconds in cpu time
	Worst: 0.016000000000000 seconds in real time; 0.001846697326982 seconds in cpu time
	Total: 0.064000000000000 seconds in real time; 0.034975981339812 seconds in cpu time
	Null Timer
	Best : 1000000000.000000000000000 seconds in real time; 1000000000.000000000000000 seconds in cpu time
	Worst: 0.000000000000000 seconds in real time; 0.000000000000000 seconds in cpu time
	Total: 0.000000000000000 seconds in real time; 0.000000000000000 seconds in cpu time
	Variance Results:
	Optimized Run:
	run 0: 0.419911
	run 1: 0.419911
	run 2: 0.419911
	run 3: 0.419911
	run 4: 0.419911
	run 5: 0.419911
	run 6: 0.419911
	run 7: 0.419911
	run 8: 0.419911
	run 9: 0.419911
	Newtons Run:
	run 0: 0.453284
	run 1: 0.453284
	run 2: 0.453284
	run 3: 0.453284
	run 4: 0.453284
	run 5: 0.453284
	run 6: 0.453284
	run 7: 0.453284
	run 8: 0.453284
	run 9: 0.453284
	
	(3)
	Optimized Timer
	Best : 0.000000000000000 seconds in real time; 0.000631488917861 seconds in cpu time
	Worst: 0.014999999999999 seconds in real time; 0.000684501370415 seconds in cpu time
	Total: 0.014999999999999 seconds in real time; 0.006492568296380 seconds in cpu time
	Newton's Timer
	Best : 0.000000000000000 seconds in real time; 0.004085744993063 seconds in cpu time
	Worst: 0.016000000000000 seconds in real time; 0.004300125030568 seconds in cpu time
	Total: 0.048000000000000 seconds in real time; 0.041298152471427 seconds in cpu time
	Standard Timer
	Best : 0.000000000000000 seconds in real time; 0.001720574306091 seconds in cpu time
	Worst: 0.000000000000000 seconds in real time; 0.001837085175794 seconds in cpu time
	Total: 0.000000000000000 seconds in real time; 0.034845489135478 seconds in cpu time
	Null Timer
	Best : 1000000000.000000000000000 seconds in real time; 1000000000.000000000000000 seconds in cpu time
	Worst: 0.000000000000000 seconds in real time; 0.000000000000000 seconds in cpu time
	Total: 0.000000000000000 seconds in real time; 0.000000000000000 seconds in cpu time
	Variance Results:
	Optimized Run:
	run 0: 0.415921
	run 1: 0.415921
	run 2: 0.415921
	run 3: 0.415921
	run 4: 0.415921
	run 5: 0.415921
	run 6: 0.415921
	run 7: 0.415921
	run 8: 0.415921
	run 9: 0.415921
	Newtons Run:
	run 0: 0.449783
	run 1: 0.449783
	run 2: 0.449783
	run 3: 0.449783
	run 4: 0.449783
	run 5: 0.449783
	run 6: 0.449783
	run 7: 0.449783
	run 8: 0.449783
	run 9: 0.449783
