# How to Build

## Linux
```bash
cmake -B build
cmake --build build
```

## BSD
Not implemented yet, but shouldn't be too hard so we might do it soon.

## Windows
### Linux
```bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE=../mingw-toolchain.cmake
cmake --build build
```
### Native
#### MinGW
```powershell
cmake -B build
cmake --build build
```
#### MSVC
Not implemented yet, might come in the future

## After building
Make sure to use comb.sh to build the final `libZFB_fat.a` file where all symbols will be combined
```bash
comb.sh
```
