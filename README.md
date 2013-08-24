# Cucumber-CPP

[![Build Status](https://travis-ci.org/cucumber/cucumber-cpp.png)](https://travis-ci.org/cucumber/cucumber-cpp)

Cucumber-Cpp allows Cucumber to support step definitions written in C++.

* [Cucumber-Cpp Website](http://github.com/cucumber/cucumber-cpp)
* [Cucumber-Cpp Documentation](https://github.com/cucumber/cucumber-cpp/wiki/)
* [Cucumber Website](http://cukes.info/)
* [Cucumber Discussion Group](http://groups.google.com/group/cukes)

If you need to ask a question, don't open a ticket on GitHub! Please post
your question on the Cucumber discussion group instead, prefixing the title
with [CPP].

It relies on a few libraries:

* [Boost](http://www.boost.org/) 1.40 or later.
  Required libraries: *thread*, *system*, *regex*, and *date_time*.
  Optional library for Boost Test driver: *test*.
* [GTest](http://code.google.com/p/googletest/) 1.4 or later.
  Optional for the GTest driver.
* [CppSpec](https://github.com/tpuronen/cppspec) development branch. 
  Optional for the CppSpec driver.
* [GMock](http://code.google.com/p/googlemock/) 1.6 or later.
  Optional for the internal test suite.
* [Qt 4](http://qt-project.org/). Optional for the CalcQt example.

This header-only library is included in the source code:

* [JSON Spirit](http://www.codeproject.com/KB/recipes/JSON_Spirit.aspx)

It might work with earlier versions of the libraries, but it was not
tested with them.

Cucumber-Cpp uses the wire protocol at the moment, so you will need
Cucumber-Ruby installed and available on the path. It is also needed
to run the functional test suite.

Building Cucumber-Cpp with tests and samples:

```
cmake -E make_directory build
cmake -E chdir build cmake -DCUKE_ENABLE_EXAMPLES=on ..
cmake --build build
cmake --build build --target test
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

