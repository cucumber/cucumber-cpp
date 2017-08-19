#!/bin/sh
set -e #break script on non-zero exitcode from any command
set -x #display command being executed

gem install bundler
bundle install

CTEST_OUTPUT_ON_FAILURE=ON
export CTEST_OUTPUT_ON_FAILURE

# Make Qt discoverable by CMake on OSX
for QT_DIR in /usr/local/Cellar/qt*/*; do
    if [ -d "${QT_DIR}" ]; then
        CMAKE_PREFIX_PATH="${CMAKE_PREFIX_PATH:+${CMAKE_PREFIX_PATH};}${QT_DIR}"
    fi
done

cmake -E make_directory build
cmake -E chdir build cmake \
    -G Ninja \
    -DCUKE_ENABLE_EXAMPLES=on \
    ${CMAKE_PREFIX_PATH:+"-DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}"} \
    ${COVERALLS_SERVICE_NAME:+"-DCMAKE_BUILD_TYPE=Debug"} \
    ${COVERALLS_SERVICE_NAME:+"-DCMAKE_CXX_FLAGS='--coverage'"} \
    ${VALGRIND_TESTS:+"-DVALGRIND_TESTS=${VALGRIND_TESTS}"} \
    ${GMOCK_PATH:-"-DGMOCK_VER=${GMOCK_VER}"} \
    ${GMOCK_PATH:+"-DGMOCK_SRC_DIR=${GMOCK_PATH}"} \
    ..
cmake --build build
cmake --build build --target test
cmake --build build --target features

# Start virtual X display server

# Starting Xvfb hangs on OSX, that's why we do this on Linux only now
if [ "${TRAVIS_OS_NAME}" = "linux" ]; then
    DISPLAY=:99
    export DISPLAY

    # Xvfb sends SIGUSR1 to its parent when it finished startup, this causes the 'wait' below to stop waiting
    trap : USR1
    (trap '' USR1; Xvfb $DISPLAY -screen 0 640x480x8 -nolisten tcp > /dev/null 2>&1) &
    XVFBPID=$!
    wait || :
    trap '' USR1
    if ! kill -0 $XVFBPID 2> /dev/null; then
        echo "Xvfb failed to start" >&2
        exit 1
    fi
else
    unset DISPLAY
fi

for TEST in \
    build/examples/Calc/GTestCalculatorSteps \
    build/examples/Calc/BoostCalculatorSteps \
    build/examples/Calc/FuncArgsCalculatorSteps \
; do
    if [ -f "${TEST}" ]; then
        "${TEST}" > /dev/null &
        sleep 1
        cucumber examples/Calc
        wait %
    fi
done

for TEST in \
    build/examples/CalcQt/GTestCalculatorQtSteps \
    build/examples/CalcQt/BoostCalculatorQtSteps \
; do
    if [ -f "${TEST}" -a -n "${DISPLAY:-}" ]; then
        "${TEST}" 2> /dev/null &
        sleep 1
        cucumber examples/CalcQt
        wait %
    fi
done

if [ -n "${XVFBPID:-}" ]; then
    # Stop virtual X display server
    kill $XVFBPID
    wait
fi

if [ -n "${COVERALLS_SERVICE_NAME}" ]; then
    lcov --directory ./build --capture  --output-file coverage.info
    lcov --remove coverage.info "/usr/*" --output-file coverage.info
    lcov --remove coverage.info "${TRAVIS_BUILD_DIR}/build/*" --output-file coverage.info
    lcov --remove coverage.info "${TRAVIS_BUILD_DIR}/tests/*" --output-file coverage.info
    lcov --remove coverage.info "${TRAVIS_BUILD_DIR}/include/json_spirit/*" --output-file coverage.info
    lcov --list coverage.info
    coveralls-lcov coverage.info
fi
