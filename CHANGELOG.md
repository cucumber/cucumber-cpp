Please see [CONTRIBUTING.md](https://github.com/cucumber/cucumber/blob/master/CONTRIBUTING.md) on how to contribute to Cucumber.

## In Git

### Added

* QtTest based test driver for cucumber-cpp ([#165](https://github.com/cucumber/cucumber-cpp/pull/165) Kamil Strzempowicz)
* Listen on localhost by default to avoid firewall warnings ([#158](https://github.com/cucumber/cucumber-cpp/pull/158) Nik Reiman)
* Better integrate Qt into buildsystem, it can now be disabled, and test it in CI ([#160](https://github.com/cucumber/cucumber-cpp/pull/160) Kamil Strzempowicz & Giel van Schijndel)
* Support taking regex captures as arguments to the step definition's function ([#159](https://github.com/cucumber/cucumber-cpp/pull/159) Giel van Schijndel)

### Changed

* Renamed HISTORY.md to CHANGELOG.md and updated to bring inline with [cucumber/cucumber #251](https://github.com/cucumber/cucumber/issues/251) ([#166](https://github.com/cucumber/cucumber-cpp/pull/166) [jaysonesmith](https://github.com/jaysonesmith))

### Fixed

* Fix issue #81 by intercepting messages from BOOST_*_MESSAGE macros ([#164](https://github.com/cucumber/cucumber-cpp/pull/164) Kamil Strzempowicz)
* Allow running all GTest cases without filter separation ([#144](https://github.com/cucumber/cucumber-cpp/pull/144) Giel van Schijndel)
* Fix QNX build by depending on standard C++ instead of specific implementation ([#156](https://github.com/cucumber/cucumber-cpp/issues/156) Giel van Schijndel)
* Ensure CMake 3.1+ is available, 2.8.12 wasn't enough for quite a while ([#152](https://github.com/cucumber/cucumber-cpp/pull/152) Giel van Schijndel)
* Fix crash during termination of the CalcQt example's step definitions ([#62](https://github.com/cucumber/cucumber-cpp/issues/62) Giel van Schijndel)

## [0.4](https://github.com/cucumber/cucumber-cpp/compare/v0.3.1...v0.4) (31 March 2017)

### New Features

* Support for MinGW build ([#130](https://github.com/cucumber/cucumber-cpp/pull/130) Michel Estermann)
* Add support for Unix sockets ([#126](https://github.com/cucumber/cucumber-cpp/pull/126) Giel van Schijndel)
* Add support for using ephemeral ports ([#131](https://github.com/cucumber/cucumber-cpp/pull/131) Giel van Schijndel)
* Removed CppSpec support ([#118](https://github.com/cucumber/cucumber-cpp/pull/118) Paolo Ambrosio)
* Support for GoogleTest 1.8 ([#120](https://github.com/cucumber/cucumber-cpp/pull/120) Kamil Strzempowicz)
* Disable Nagle on TCP socket ([#125](https://github.com/cucumber/cucumber-cpp/pull/125) Giel van Schijndel)

### Bugfixes

* Fixed suggested step definition when step sentence contains double quote ([#116](https://github.com/cucumber/cucumber-cpp/issues/116) Kamil Strzempowicz, [fbc49a3](https://github.com/cucumber/cucumber-cpp/commit/fbc49a34e12a0b9b2a6e121d97ba1ad8f46dce8f) Paolo Ambrosio)
* Fixed `defs.hpp` deprecation warning on MSVC ([#124](https://github.com/cucumber/cucumber-cpp/pull/124) Antoine Allard)
* Fixed parallel build ([#135](https://github.com/cucumber/cucumber-cpp/pull/135) Giel van Schijndel)
* Fixed memory leaks and better memory management ([#134](https://github.com/cucumber/cucumber-cpp/pull/134) Giel van Schijndel)
* Fixed clang warning about bad `typeid` usage ([#138](https://github.com/cucumber/cucumber-cpp/pull/138) Kamil Strzempowicz)

## [0.3.1](https://github.com/cucumber/cucumber-cpp/compare/v0.3...v0.3.1) (11 April 2016)

### New Features

* Support for Boost 1.60 ([#101](https://github.com/cucumber/cucumber-cpp/pull/101) Kai Unger)
* Support for CMake inclusion in other projects ([#76](https://github.com/cucumber/cucumber-cpp/pull/76) Eric Brayet)
* Added Qt5 support in CalcQt example ([#98](https://github.com/cucumber/cucumber-cpp/pull/98) Kamil Strzempowicz)
* Improved Generic Driver to write steps without testing framework ([#99](https://github.com/cucumber/cucumber-cpp/pull/99) Kamil Strzempowicz)

### Bugfixes

None

## [0.3](https://github.com/cucumber/cucumber-cpp/compare/v0.2...v0.3) (22 December 2013)

### New Features

* Added BEFORE_ALL and AFTER_ALL macros ([#65](https://github.com/cucumber/cucumber-cpp/pull/65) Larry Price)
* Added CalcQt example ([#58](https://github.com/cucumber/cucumber-cpp/pull/58) Gianni Ambrosio)
* Replaced USING_CONTEXT with ScenarioScope<T> ([27256e9](https://github.com/cucumber/cucumber-cpp/commit/27256e932c75e9d4d57d4839042317e6a04cfe46) Paolo Ambrosio)
* Changed include name from core.hpp to defs.hpp ([5bbac06](https://github.com/cucumber/cucumber-cpp/commit/5bbac062e19dcf9de2761f4ded115aa7212c14d7) Paolo Ambrosio)
* Project rename from CukeBins to Cucumber-Cpp ([efecfd0](https://github.com/cucumber/cucumber-cpp/commit/efecfd0813efa1b6d406c2fd0cd03d8a84bed3ff) Paolo Ambrosio)

### Bugfixes

* Fixed socket server bug in VS2012 forcing Boost 1.51 ([#57](https://github.com/cucumber/cucumber-cpp/pull/57) Jared Szechy, [e41a9b7](https://github.com/cucumber/cucumber-cpp/commit/e681c5028a756d8f711574a86e84ca8b98333d5c) Paolo Ambrosio)
* Fixed crashes on some architectures ([#52](https://github.com/cucumber/cucumber-cpp/pull/52) Sabst)
* Fixed AFTER hook ordering issue ([#43](https://github.com/cucumber/cucumber-cpp/pull/43) Greg Williams)
* Added default empty constructor to work with less permissive gcc 4.6 settings ([#38](https://github.com/cucumber/cucumber-cpp/pull/38) Hugo Ferreira)


## [0.2](https://github.com/cucumber/cucumber-cpp/compare/v0.1...v0.2) (25 June 2011)

TODO


## [0.1](https://github.com/cucumber/cucumber-cpp/commits/v0.1) (03 May 2010)

* Initial implementation

