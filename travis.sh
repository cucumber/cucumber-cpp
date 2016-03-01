#!/bin/sh
set -e #break script on non-zero exitcode from any command
gem install gem-shut-the-fuck-up
gem install bundler
cmake -E make_directory build
cmake -E chdir build cmake -DCUKE_ENABLE_EXAMPLES=on -DGTEST_INCLUDE_DIR=/usr/src/gmock/gtest/include/  ..
cmake --build build
cmake --build build --target test
cmake --build build --target features

GTEST=build/examples/Calc/GTestCalculatorSteps 
BOOST=build/examples/Calc/BoostCalculatorSteps
if [ -f $GTEST ]; then
    $GTEST >/dev/null &
    cucumber examples/Calc
fi
if [ -f $BOOST ]; then 
    $BOOST >/dev/null &
    cucumber examples/Calc
fi

