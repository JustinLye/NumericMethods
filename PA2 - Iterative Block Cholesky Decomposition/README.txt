COMPILING:

open the visual studio command prompt and navigate to the folder
containing the submitted documents (i.e. CS3513 PP2 - Justin Lye - Submission 2)
using the cd (change directory command)

cd [Path to submitted folder]

next compile using the following command:

cl ibchk.cpp /IEigen /EHsc /arch:SSE2

note: /arch:SSE2 was a failed attempt to fix the precision problem.

PROBLEMS:

When read in the input file and output it to another file the
was a loss in precision. I tried change the compiler settings
and attempted to download MinGW (download failed). After spending
a couple of hours trying to fix the problem I switched my focus
to the decomposition problem. Currently there is still a
precision problem.


ERRORS:

The first submission contained errors because block G11 was not
being update with each iteration. Once I discovered the error
I fixed it and uploaded a second submission to D2L.

Error Description for Submission 1:
Error is calculated as the largest (absolute value) element found
in the array A - L * L(transpose).

I found the error on matrix a1 to be very small; however the error
on matrix a0 was very large 9.0949470177292824e-013 and 2.023916e+003
when using the default 8 for block size. 
