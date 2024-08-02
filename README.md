# Cucumber-CPP

## Overview

Cucumber-Cpp allows Cucumber to support step definitions written in C++.

* [Cucumber-Cpp Website](https://github.com/cucumber/cucumber-cpp)
* [Cucumber-Cpp Documentation](https://github.com/cucumber/cucumber-cpp/wiki/)
* [Cucumber Website](https://cucumber.io/)
* [Get in touch](https://cucumber.io/docs/community/get-in-touch/)

If you need to ask a question, post on the [Cucumber discussion group](https://github.com/orgs/cucumber/discussions).

If you want to contribute code to the project, guidelines are in [CONTRIBUTING.md](CONTRIBUTING.md).

## Dependencies

It relies on a few executables:

* [cmake](https://cmake.org/download/) 3.16 or later.
  Required to setup environment and build software

It relies on a few libraries:

* [Asio](https://think-async.com/Asio/) 1.18.1 or later.
* [Boost.Test](https://www.boost.org/) 1.70. Optional for the Boost Test driver.
* [GTest](https://github.com/google/googletest) 1.11.0 or later. Optional for the GTest driver.
* [GMock](https://github.com/google/googletest) 1.11.0 or later. Optional for the internal test suite.
* [nlohmann-json](https://github.com/nlohmann/json) 3.10.5 or later.
* [Qt6 or Qt5](https://qt-project.org/). Optional for the CalcQt example and QtTest driver.
* [TCLAP](https://tclap.sourceforge.net/) 1.2.5 or later.

It might work with earlier versions of the libraries, but it was not tested with them.
See the [CI scripts](.github/workflows/run-all.yml) for details about dependency installation.

Cucumber-Cpp uses the wire protocol at the moment, so you will need
Cucumber-Ruby installed and available on the path. It is also needed
to run the functional test suite.

Please mind that Cucumber-Cpp is not compatible with Cucumber-Ruby 3.x
due to a [bug in its wire protocol](https://github.com/cucumber/cucumber-ruby/issues/1183)
implementation.

To install the Ruby prerequisites:

```
gem install bundler // For windows: gem install bundle
bundle install
```

### Windows vs. Linux

To get an inspiration on how to set up the dependencies on your specific system (Windows or Linux), you may want to have a look at the
workflow files [for Windows](.github/workflows/windows-build.yml) and [for Linux](.github/workflows/linux-build.yml).


## Build

Building Cucumber-Cpp with tests and samples:

```
# Create build directory
cmake -E make_directory build

# Generate Makefiles
cmake -E chdir build cmake \
    -DCUKE_ENABLE_BOOST_TEST=on \
    -DCUKE_ENABLE_GTEST=on \
    -DCUKE_ENABLE_QT_6=on \
    -DCUKE_TESTS_UNIT=on \
    -DCUKE_ENABLE_EXAMPLES=on \
    ..

# Build cucumber-cpp
cmake --build build

# Run unit tests
cmake --build build --target test

# Run install
cmake --install build
```

Running the Calc example on Unix:

```
build/examples/Calc/BoostCalculatorSteps >/dev/null &
(cd examples/Calc; cucumber)
```

Running the Calc example on Windows (NMake):

```
start build\examples\Calc\BoostCalculatorSteps.exe
cucumber examples\Calc
```

## The way it works
(This is a great explanation by [paoloambrosio](https://github.com/paoloambrosio) copied from [stackoverflow](https://stackoverflow.com/questions/50760865/cucumber-cpp-required-software-for-running-example))

The way Cucumber-CPP currently works is by having Cucumber-Ruby connecting to a TCP port where the C++ implementation is listening. When the wire protocol is defined in the cucumber.wire file, with host and port where your C++ wire protocol server is listening, Cucumber-Ruby will try and run them with Cucumber-CPP.

C++ is a compiled language, so step definitions must be compiled first. The examples provided use CMake, as described in the README. Cucumber-CPP needs to be linked to the step definitions and to everything that they use (usually the application under test), creating an executable file that will listen to the wire protocol port (defaults to localhost:3902) for Cucumber-Ruby to connect to (and exiting when it disconnects).

```
                    +------------------------------------------+
                    |                                          |
+----------+        | +----------+  +----------+  +----------+ |
|          |        | |          |  |          |  |          | |
| Cucumber |        | | Cucumber |  | C++ Step |  | Your     | |
| Ruby     |--------->| CPP Wire |--| Defs     |--| CPP App  | |
|          |        | | Server   |  |          |  |          | |
|          |        | |          |  |          |  |          | |
+----------+        | +----------+  +----------+  +----------+ |
                    |                                          |
                    +------------------------------------------+
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

Then create your step definition runner (an example is available [here](examples/Calc/features/step_definitions/BoostCalculatorSteps.cpp)). In order to compile the step definition runner, make sure to add [cucumber include directory](include/cucumber-cpp) to the include path and link with *libcucumber-cpp.a* and additional testing libraries (boost unit test).

Run the step definition runner in the background and then cucumber, like in the Calc example in the previous section. The step definition runner should exit after the feature is run and cucumber exits.
