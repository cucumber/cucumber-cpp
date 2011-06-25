CukeBins allows Cucumber to support step definitions written in C++


CukeBins Website: http://github.com/paoloambrosio/cukebins
CukeBins Documentation: https://github.com/paoloambrosio/cukebins/wiki/
Cucumber Website: http://cukes.info/


The current library relies on a few libraries:

 * Boost 1.40 or later - http://www.boost.org/
   Required libraries: thread, system, regex, and date_time
   Optional library for Boost Test driver: test
 * GTest 1.4 or later - http://code.google.com/p/googletest/
   Optional for CukeBins test suite and for the GTest driver
 * CppSpec development branch - https://github.com/tpuronen/cppspec
   Optional for the CppSpec driver

This header-only library is included in the source code:

 * JSON Spirit - http://www.codeproject.com/KB/recipes/JSON_Spirit.aspx


Building tests and sample application:

cmake -E make_directory build
cmake -E chdir build cmake ..
cmake --build build
cmake --build build --target test

Running the example on Unix:

build/examples/Calc/CalculatorSteps >/dev/null &
cucumber examples/Calc/CalcFeatures

Running the example on Windows (NMake):

start build\examples\Calc\CalculatorSteps.exe
cucumber examples\Calc\CalcFeatures

