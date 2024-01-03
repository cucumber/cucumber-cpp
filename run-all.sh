#!/bin/sh
set -e #break script on non-zero exitcode from any command
set -x #display command being executed

startXvfb () {
    Xvfb $DISPLAY -screen 0 640x480x8 -nolisten tcp > /dev/null 2>&1 &
    XVFBPID=$!
    sleep 5
}

killXvfb () {
    if [ -n "${XVFBPID:-}" ]; then
        # Stop virtual X display server
        kill $XVFBPID
    fi
}

CTEST_OUTPUT_ON_FAILURE=ON
export CTEST_OUTPUT_ON_FAILURE

cmake -E make_directory build
cmake -E chdir build cmake \
    -G Ninja \
    -DCUKE_ENABLE_BOOST_TEST=on \
    -DCUKE_ENABLE_GTEST=on \
    -DCUKE_ENABLE_QT=on \
    -DCUKE_ENABLE_EXAMPLES=on \
    -DCUKE_TESTS_UNIT=on \
    -DCUKE_CODE_COVERAGE=on \
    ..
cmake --build build --parallel

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

startXvfb # Start virtual X display server

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

killXvfb

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
