CukeBins allows Cucumber to support step definitions written in C++


It is designed to support several testing backends and communication
methods, but for now there is just one implementation using:

 - GTest (Google C++ Testing Framework)
 - Wire Protocol


The current library is header-only, so it doesn't need to be compiled
separately, but it needs the following libraries at link time:

 - Boost 1.40 - http://www.boost.org/
     Should work with some previous versions too, but it wasn't tested
 - GTest 1.4+ - http://code.google.com/p/googletest/
     Needed by the test suite and the GTest implementation. Works with
     GTest 1.4 and 1.5 (not tested with 1.3)

This header-only library is included in the source code:

 - JSON Spirit - http://www.codeproject.com/KB/recipes/JSON_Spirit.aspx
     Needed by the Wire Protocol connector


CukeBins includes a sample borrowed from Cuke4Nuke that uses the same
feature definitions but tests a C++ implementation. To run the tests
and the example, you need the following external tool:

 - CMake 2.8 - http://www.cmake.org/
     2.8 because it needs FindGTest

To build the tests and the sample application:

cmake -E make_directory build
cmake -E chdir build cmake ..
cmake --build build
cmake --build build --target test

To run the example on Unix:

build/examples/Calc/CalculatorSteps >/dev/null &
cucumber examples/Calc/CalcFeatures

To run the example on Windows (NMake):

start build\examples\Calc\CalculatorSteps.exe
cucumber examples\Calc\CalcFeatures


CMake hints:

 - For GTest, set GTEST_ROOT to the location of the GTest install prefix
     If the GTest libraries were installed in /usr/local/lib and the includes
     are in /usr/local/include/gtest, then add "-DGTEST_ROOT=/usr/local" to
     the cmake command line

 - For Boost, set BOOST_ROOT to the location of the Boost install prefix
   if CMake has troubles finding it

