set QT_PATH=C:\Qt
set QT_VERSION=5.10.1
set MINGW_VERSION=mingw53_32

set QT_MINGW_PATH=%QT_PATH%\%QT_VERSION%\%MINGW_VERSION%\bin
set QT_TOOL_MINGW_PATH=%QT_PATH%\Tools\mingw530_32\bin

:: Example path for MSVC 32-bit
:: PATH=%QT_PATH%\%QT_VERSION%\msvc2013\bin;%PATH%

:: Path for MinGW g++
PATH=%QT_MINGW_PATH%;%QT_TOOL_MINGW_PATH%;%PATH% 

windeployqt.exe --release --force %~dp0\build-desktop\release\SpectrumAnalyzer.exe
pause