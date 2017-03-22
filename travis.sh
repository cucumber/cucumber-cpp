#!/bin/sh
set -e #break script on non-zero exitcode from any command

if [ "${TRAVIS_OS_NAME}" = "osx" ]; then source osx-install-qt.sh; fi

gem install bundler
bundle install

CTEST_OUTPUT_ON_FAILURE=ON
export CTEST_OUTPUT_ON_FAILURE

cmake -E make_directory build
cmake -E chdir build cmake \
    -G Ninja \
    -DCUKE_ENABLE_EXAMPLES=on \
    ${GMOCK_PATH:-"-DGMOCK_VER=${GMOCK_VER}"} \
    ${GMOCK_PATH:+"-DGMOCK_SRC_DIR=${GMOCK_PATH}"} \
    ..
cmake --build build
cmake --build build --target test
cmake --build build --target features

GTEST=build/examples/Calc/GTestCalculatorSteps
BOOST=build/examples/Calc/BoostCalculatorSteps
QTTEST=build/examples/Calc/QtTestCalculatorSteps
if [ -f $GTEST ]; then
    $GTEST >/dev/null &
    cucumber examples/Calc
    wait
fi
if [ -f $BOOST ]; then
    $BOOST >/dev/null &
    cucumber examples/Calc
    wait
fi
if [ -f $QTTEST ]; then
    $QTTEST >/dev/null &
    cucumber examples/Calc
    wait
fi
