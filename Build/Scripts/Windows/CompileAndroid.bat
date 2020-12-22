SET PATH=%ANDROID_NDK%\prebuilt\windows-x86_64\bin\;%ANDROID_NDK%\prebuilt\windows\bin\;%PATH%
cmake -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE=../../../Build/CMake/Toolchains/android.toolchain.cmake -DCMAKE_BUILD_TYPE=%1 -DANDROID_ABI=%2 ../../../
make.exe -j4
