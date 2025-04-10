# How to Build

## Linux
```bash
cmake -B build
cmake --build build
```

## Windows
```powershell
cmake -B build -DCMAKE_TOOLCHAIN_FILE=../mingw-toolchain.cmake
cmake --build build
```
