#!/bin/sh
set -e #break script on non-zero exitcode from any command
set -x #display command being executed
gem install bundler
bundle install

CTEST_OUTPUT_ON_FAILURE=ON
export CTEST_OUTPUT_ON_FAILURE

export COVERALLS_SERVICE_NAME=travis-ci
export COVERALLS_REPO_TOKEN=ThdhpdagGJJVJj0KFGdSgdpzRLALbNAIg

cmake -E make_directory build
cmake -E chdir build cmake \
    -G Ninja \
    -DCUKE_ENABLE_EXAMPLES=on \
    ${VALGRIND_TESTS:+"-DVALGRIND_TESTS=${VALGRIND_TESTS}"} \
    ${COVERAGE:+"-DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS='--coverage' -DCMAKE_LINK_INTERFACE_LIBRARIES='gcov'"} \
    ${GMOCK_PATH:-"-DGMOCK_VER=${GMOCK_VER}"} \
    ${GMOCK_PATH:+"-DGMOCK_SRC_DIR=${GMOCK_PATH}"} \
    ..
cmake --build build
cmake --build build --target test
if [ "${COVERAGE}" = "ON" ]; then
    ~/.local/bin/coveralls --exclude examples --exclude tests --gcov-options '\-lp'
else
    cmake --build build --target features

    GTEST=build/examples/Calc/GTestCalculatorSteps
    BOOST=build/examples/Calc/BoostCalculatorSteps
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
fi
