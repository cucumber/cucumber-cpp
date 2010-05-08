CukeBins allows Cucumber to support step definitions written in C++


CukeBins Website: http://github.com/paoloambrosio/cukebins
CukeBins Documentation: http://wiki.github.com/paoloambrosio/cukebins/
Cucumber Website: http://cukes.info/


The current library is header-only, so it doesn't need to be compiled
separately, but it needs the following libraries at link time:

 * Boost 1.40 or later - http://www.boost.org/
 * GTest 1.4 or later - http://code.google.com/p/googletest/

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

