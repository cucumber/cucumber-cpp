set PATH=C:\Ruby200\bin;%BOOST_LIBRARYDIR%;%PATH%
if defined MINGW_ROOT set PATH=%MINGW_ROOT%\bin;C:\msys64\usr\bin\;%PATH%
if "%CMAKE_GENERATOR%"=="NMake Makefiles" call "%VS140COMNTOOLS%\..\..\VC\vcvarsall.bat" %PLATFORM%

git submodule init
git submodule update
call gem install bundle
call bundle install
if defined MINGW_ARCH bash -lc "pacman --needed --noconfirm -S mingw-w64-%MINGW_ARCH%-boost

cmake -E make_directory build
cmake -E chdir build cmake -G "%CMAKE_GENERATOR%" -DCUKE_ENABLE_EXAMPLES=on -DBOOST_ROOT="%BOOST_ROOT%" -DBOOST_INCLUDEDIR="%BOOST_INCLUDEDIR%" -DBOOST_LIBRARYDIR="%BOOST_LIBRARYDIR%" -DCMAKE_PREFIX_PATH="%QT_DIR%" ..
cmake --build build

set CTEST_OUTPUT_ON_FAILURE=ON
cmake --build build --target test
cmake --build build --target features

