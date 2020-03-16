@echo off
echo 	Mod Compilation Started!
call C:/android-ndk-r20b/ndk-build.cmd NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk NDK_APPLICATION_MK=./Application.mk

