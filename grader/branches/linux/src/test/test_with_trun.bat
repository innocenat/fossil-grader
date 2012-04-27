@echo off

SET TCCOPT=notset

IF "%TCCOPT%" == "notset" (
   copy test_with_trun.bat test_with_trun.local.bat > nul:
   echo ----------------------------------------------------
   echo             Don't run this file to test
   echo ----------------------------------------------------
   echo Script copied to test_with_trun.local.bat.
   echo Please edit that file with correct options of TCCOPT
   echo ----------------------------------------------------
   exit/b
)

IF NOT EXIST ..\..\..\bin\trun.exe (
   echo Can't find trun.exe.
   echo Please compile and copy it to ..\..\..\bin
   exit/b
)

echo Creating temp dir: test
mkdir test
cd test

echo 10 20 > 1.in
copy ..\submissions\add\*
copy ..\submissions\muinum\*

echo Compiling test submissions...
tcc %TCCOPT% add.c
tcc %TCCOPT% addcpp.cpp
tcc %TCCOPT% addloop.c
tcc %TCCOPT% muicr.c

copy ..\..\..\bin\trun.exe .

echo ----------------------------------------------
echo Correct submission in C, you should see 30:
echo ----------------------------------------------
trun add.exe -i 1.in -t 1

echo ----------------------------------------------
echo Correct submission in C++, you should see 30:
echo ----------------------------------------------
trun addcpp.exe -i 1.in -t 1

echo ----------------------------------------------
echo Loop submission
echo ----------------------------------------------
trun addloop.exe -i 1.in -t 1

echo ----------------------------------------------
echo Crashing submission...  
echo you'll see some error dialog please close it.
echo ----------------------------------------------
trun muicr.exe -i muicr.in -t 1

echo ----------------------------------------------
echo Dir "test" has not been deleted intentionally.
echo Please do that yourself.
echo ----------------------------------------------
echo .
echo OK.


cd ..
