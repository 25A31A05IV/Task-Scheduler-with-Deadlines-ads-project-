@echo off
title Task Scheduler Launcher
cd /d "%~dp0"

echo =====================================
echo       TASK SCHEDULER PROJECT
echo =====================================
echo.

echo Stopping any previous server...
taskkill /F /IM server.exe >nul 2>&1

timeout /t 1 /nobreak >nul

echo Compiling C++ backend...
g++ -std=c++17 server.cpp Scheduler.cpp DP.cpp -o server.exe -lws2_32

if errorlevel 1 (
    echo.
    echo Compilation failed. Check the errors above.
    pause
    exit /b 1
)

echo Starting C++ server...
start "Task Scheduler Server" "%~dp0server.exe"

timeout /t 2 /nobreak >nul
start "" "http://127.0.0.1:8080"

echo.
echo Website launched!
pause