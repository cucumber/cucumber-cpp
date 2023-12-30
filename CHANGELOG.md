Please see [CONTRIBUTING.md](https://github.com/cucumber/cucumber/blob/main/CONTRIBUTING.md) on how to contribute to Cucumber.

## In Git

### Added

### Changed

### Fixed

## [0.7.0](https://github.com/cucumber/cucumber-cpp/compare/v0.6...v0.7.0) (30 December 2023)

### Added

* add version info to cucumber-cpp-main ([8a0d16b8](https://github.com/cucumber/cucumber-cpp/pull/275/commits/8a0d16b8f59dd1c9d75045aa016d9ce235257311) Urs Fässler)
* Remove dependency to Boost ([#275](https://github.com/cucumber/cucumber-cpp/pull/275) Urs Fässler)
* List headers in cmake ([#274](https://github.com/cucumber/cucumber-cpp/pull/274) Urs Fässler)
* Check code format on PR and main branch ([#268](https://github.com/cucumber/cucumber-cpp/pull/268) Urs Fässler)

### Changed

* use semantic versioning ([#280](https://github.com/cucumber/cucumber-cpp/pull/280) Urs Fässler)
* No googletest auto-download ([#279](https://github.com/cucumber/cucumber-cpp/pull/279) Urs Fässler)
* Remove dependency to Boost ([#275](https://github.com/cucumber/cucumber-cpp/pull/275) Urs Fässler)
  * use asio without boost  ([d131ae39](https://github.com/cucumber/cucumber-cpp/pull/275/commits/d131ae3951b9aeb03dd787bf6fee4a06a81c1f04))
  * use TCLAP to parse program options ([b688cf41](https://github.com/cucumber/cucumber-cpp/pull/275/commits/b688cf41647803daa8a7656595b0d1bcbabf2310))
* use nlohmann/json library ([#273](https://github.com/cucumber/cucumber-cpp/pull/273) Urs Fässler)
* remove boost::multi_array ([#269](https://github.com/cucumber/cucumber-cpp/pull/269) Urs Fässler)
* Modernize overloading ([#267](https://github.com/cucumber/cucumber-cpp/pull/267) Urs Fässler)
* refactoring: use stl regex instead of boost::regex ([#266](https://github.com/cucumber/cucumber-cpp/pull/266) Urs Fässler)

### Fixed

* mention CI scripts for details about dependency installation ([#278](https://github.com/cucumber/cucumber-cpp/pull/278) Urs Fässler)
* remove broken E2E tests ([#272](https://github.com/cucumber/cucumber-cpp/pull/272) Urs Fässler)
* remove broken AppVeyor build ([#271](https://github.com/cucumber/cucumber-cpp/pull/271) Urs Fässler)

## [0.6](https://github.com/cucumber/cucumber-cpp/compare/v0.5...v0.6) (17 December 2023)

### Added

### Changed

* Using C++ standard library where possible instead of boost ([#264](https://github.com/cucumber/cucumber-cpp/pull/264) Urs Fässler)

### Fixed

* Statically linking `boost_system` ([#197](https://github.com/cucumber/cucumber-cpp/pull/197) Matthieu Longo)
* Unable to `add_subdirectory(cucumber-cpp)` ([#211](https://github.com/cucumber/cucumber-cpp/pull/211) Sergey Bon)
* Warning C4265 on Visual Studio ([#195](https://github.com/cucumber/cucumber-cpp/pull/195) Matthieu Longo)
* Fix handling of optional regex captures ([#221](https://github.com/cucumber/cucumber-cpp/pull/221) Alain Martin)
* Fix compilation with Boost 1.70.0 ([#225](https://github.com/cucumber/cucumber-cpp/pull/225) Krystian Młynarczyk)
* Support step definitions with multi-byte characters ([#224](https://github.com/cucumber/cucumber-cpp/pull/224) Spencer Rudnick)
* Supress warning about deprecated QSignalMapper ([#228](https://github.com/cucumber/cucumber-cpp/pull/228) Lukas Woodtli)
* Remove CACHE FORCE arguments from CMAKE_CXX_FLAG on colored terminal output ([#232](https://github.com/cucumber/cucumber-cpp/pull/232)  Alex Cani)
* Enable compiling with clang's -Wsuggest-override ([#244](https://github.com/cucumber/cucumber-cpp/pull/244) Tobias Hahn)
* Add posibility to build with sanitizers enabled ([#247](https://github.com/cucumber/cucumber-cpp/pull/247) Lukas Woodtli)
* Add support for latest GoogleTest and Boost ([#249](https://github.com/cucumber/cucumber-cpp/pull/249) Canmor Lam)
* add file extensions to adhere to policy CMP0115 ([#250](https://github.com/cucumber/cucumber-cpp/pull/250) Urs Fässler)
* Support latest Qt and test on Ubuntu 22.04 ([#253](https://github.com/cucumber/cucumber-cpp/pull/253) Urs Fässler)
* Update table.feature ([#258](https://github.com/cucumber/cucumber-cpp/pull/258) mbed101)

## [0.5](https://github.com/cucumber/cucumber-cpp/compare/v0.4...v0.5) (2 July 2018)

### Added

* QtTest based test driver for cucumber-cpp ([#165](https://github.com/cucumber/cucumber-cpp/pull/165) Kamil Strzempowicz)
* Listen on localhost by default to avoid firewall warnings ([#158](https://github.com/cucumber/cucumber-cpp/pull/158) Nik Reiman)
* Better integrate Qt into buildsystem, it can now be disabled, and test it in CI ([#160](https://github.com/cucumber/cucumber-cpp/pull/160) Kamil Strzempowicz & Giel van Schijndel)
* Support taking regex captures as arguments to the step definition's function ([#159](https://github.com/cucumber/cucumber-cpp/pull/159) Giel van Schijndel)
* Support building as shared library on Windows and hide internal symbols on all platforms ([#147](https://github.com/cucumber/cucumber-cpp/pull/147) [Nik Reiman](https://github.com/nre-ableton))
* Support installing library targets along with headers ([#182](https://github.com/cucumber/cucumber-cpp/pull/182) [Matthieu](https://github.com/matlo607))

### Changed

* Renamed HISTORY.md to CHANGELOG.md and updated to bring inline with [cucumber/cucumber #251](https://github.com/cucumber/cucumber/issues/251) ([#166](https://github.com/cucumber/cucumber-cpp/pull/166) [jaysonesmith](https://github.com/jaysonesmith))

### Fixed

* Fix compiler warning ([#192](https://github.com/cucumber/cucumber-cpp/issues/192) [Giel van Schijndel](https://github.com/muggenhor)), ([#184](https://github.com/cucumber/cucumber-cpp/pull/184) [Kamil Strzempowicz](https://github.com/konserw))
* Add missing virtual destructor in base class SocketServer used by TCPSocketServer and UnixSocketServer ([#183](https://github.com/cucumber/cucumber-cpp/pull/183) Matthieu Longo)
* Fix breaking changes in API of boost-1.66.0 ([#180](https://github.com/cucumber/cucumber-cpp/pull/180)  Matthieu Longo)
* Fix conflicting "using std" declaration with "using boost::thread" ([#181](https://github.com/cucumber/cucumber-cpp/pull/181)  Matthieu Longo)
* Fix building with boost > 1.64 by adding 2 new methods to BoostLogInterceptor ([#175](https://github.com/cucumber/cucumber-cpp/pull/175) Kamil Strzempowicz)
* Fixing Visual Studio 2013 error: no appropriate default constructor available ([#188](https://github.com/cucumber/cucumber-cpp/pull/188) Antoine Allard)
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

