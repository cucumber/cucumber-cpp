# Cucumber-CPP

[![Join the chat at https://gitter.im/cucumber/cucumber-cpp](https://badges.gitter.im/cucumber/cucumber-cpp.svg)](https://gitter.im/cucumber/cucumber-cpp?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Linux and OSX build status](https://travis-ci.org/cucumber/cucumber-cpp.svg)](https://travis-ci.org/cucumber/cucumber-cpp)
[![Windows build status](https://ci.appveyor.com/api/projects/status/5jeap3a4si9w8kka?svg=true)](https://ci.appveyor.com/project/paoloambrosio/cucumber-cpp-qqrt7)

Cucumber-Cpp allows Cucumber to support step definitions written in C++.

* [Cucumber-Cpp Website](http://github.com/cucumber/cucumber-cpp)
* [Cucumber-Cpp Documentation](https://github.com/cucumber/cucumber-cpp/wiki/)
* [Cucumber Website](http://cukes.info/)
* [Cucumber Discussion Group](http://groups.google.com/group/cukes)

If you need to ask a question, don't open a ticket on GitHub! Please post
your question on the Cucumber discussion group instead, prefixing the title
with [CPP].

It relies on a few executables:

* [cmake](https://cmake.org/download/) 2.8.12 or later.
  Required to setup environment and build software

It relies on a few libraries:

* [Boost](http://www.boost.org/) 1.46 or later (1.51+ on Windows).
  Required libraries: *thread*, *system*, *regex*, *date_time* and *program_options*.
  Optional library for Boost Test driver: *test*.
* [GTest](http://code.google.com/p/googletest/) 1.6 or later.
  Optional for the GTest driver. By default downloaded and built by CMake.
* [GMock](http://code.google.com/p/googlemock/) 1.6 or later.
  Optional for the internal test suite. By default downloaded and built by CMake.
* [Qt 4 or 5](http://qt-project.org/). Optional for the CalcQt example.

This header-only library is included in the source code:

* [JSON Spirit](http://www.codeproject.com/KB/recipes/JSON_Spirit.aspx)

It might work with earlier versions of the libraries, but it was not
tested with them.

Cucumber-Cpp uses the wire protocol at the moment, so you will need
Cucumber-Ruby installed and available on the path. It is also needed
to run the functional test suite.

To install the Ruby prerequisites:

```
gem install bundler
bundle install
```

Building Cucumber-Cpp with tests and samples:

```
# Download test suite
git submodule init
git submodule update

# Create build directory
cmake -E make_directory build

# Generate Makefiles
cmake -E chdir build cmake -DCUKE_ENABLE_EXAMPLES=on ..

# Build cucumber-cpp and tests
cmake --build build

# Run unit tests
cmake --build build --target test

# Check implementation against common cucumber test suite
cmake --build build --target features
```

Running the Calc example on Unix:

```
build/examples/Calc/BoostCalculatorSteps >/dev/null &
cucumber examples/Calc
```

Running the Calc example on Windows (NMake):

```
start build\examples\Calc\BoostCalculatorSteps.exe
cucumber examples\Calc
```

## Getting started

Here is a basic example on how to get started with *cucumber-cpp*. First you need to create the basic feature structure:

```
cucumber --init
```

Then create a *cucumber.wire* file in the *features/step_definitions* folder with the following content:

```
host: localhost
port: 3902
```

Create your first feature (an example is available [here](examples/Calc/features/addition.feature)).

Then create your step definition runner (an example is available [here](examples/Calc/features/step_definitions/BoostCalculatorSteps.cpp)). In order to compile the step definition runner, make sure to add [cucumber include directory](includes) to the include path and link with *libcucumber-cpp.a* and additional testing libraries (boost unit test).

Run the step definition runner in the background and then cucumber, like in the Calc example in the previous section. The step definition runner should exit after the feature is run and cucumber exits.
