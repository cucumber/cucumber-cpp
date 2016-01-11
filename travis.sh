ruby -v
gem install bundler
cmake -E make_directory build
cmake -E chdir build cmake -DCUKE_ENABLE_EXAMPLES=on ..
cmake --build build
cmake --build build --target test
cmake --build build --target features
build/examples/Calc/GTestCalculatorSteps >/dev/null &
cucumber examples/Calc
