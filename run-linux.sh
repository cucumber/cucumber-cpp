#!/bin/sh
set -e #break script on non-zero exitcode from any command
set -x #display command being executed

CTEST_OUTPUT_ON_FAILURE=ON
export CTEST_OUTPUT_ON_FAILURE

cmake -E make_directory build
cmake -E chdir build cmake \
    -G Ninja \
    -DCUKE_ENABLE_BOOST_TEST=on \
    -DCUKE_ENABLE_GTEST=on \
    -DCUKE_ENABLE_QT_6=on \
    -DCUKE_ENABLE_EXAMPLES=on \
    -DCUKE_TESTS_UNIT=on \
    -DCUKE_CODE_COVERAGE=on \
    ..
cmake --build build --parallel --verbose

#
# Run tests
#

cmake --build build --target test

#
# Execute Calc examples
#

for TEST in \
    build/examples/Calc/GTestCalculatorSteps \
    build/examples/Calc/QtTestCalculatorSteps \
    build/examples/Calc/BoostCalculatorSteps \
    build/examples/Calc/FuncArgsCalculatorSteps \
; do
    "${TEST}" > /dev/null &
    sleep 1
    (cd examples/Calc; cucumber)
    wait %
done

#
# Execute QtCalc examples
#

for TEST in \
    build/examples/CalcQt/GTestCalculatorQtSteps \
    build/examples/CalcQt/QtTestCalculatorQtSteps \
    build/examples/CalcQt/BoostCalculatorQtSteps \
; do
    "${TEST}" 2> /dev/null &
    sleep 1
    (cd examples/CalcQt; cucumber)
    wait %
done

#
# Execute feature showcase on Unix socket
#

SOCK=`pwd`/cucumber.wire.sock
TEST=build/examples/FeatureShowcase/FeatureShowcaseSteps
echo "unix: ${SOCK}" > examples/FeatureShowcase/features/step_definitions/cucumber.wire
"${TEST}" --unix "${SOCK}" > /dev/null &
(cd examples/FeatureShowcase; cucumber)
wait %

mkdir -p coverage
gcovr build/ --html-details --output coverage/index.html --xml coverage/cobertura.xml

sudo cmake --install build
