#rvm install 2.2
ruby -v
gem install bundler
rm Gemfile.lock
rm build -rf
cmake -E make_directory build
cmake -E chdir build cmake -DCUKE_ENABLE_EXAMPLES=on -DCUKE_DISABLE_BOOST_TEST=on ..
cmake --build build
cmake --build build --target test
cmake --build build --target features
build/examples/Calc/GTestCalculatorSteps >/dev/null &
cucumber examples/Calc
killall GTestCalculatorSteps 
