@setlocal
@set CIVSIM_DIRECTORY=../civsim
@PATH=%PATH%;%CIVSIM_DIRECTORY%/cmake/bin
@call %CIVSIM_DIRECTORY%\cmake-scripts\generate.cmd msvc2010-x86
@pause
@endlocal
