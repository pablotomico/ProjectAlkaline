# ProjectAlkaline

## Project Setup

1. Visual Studio Code: for extensions I have a few
   1. `C/C++`, `Lua` and `Lua Language Server` minimum
   2. https://code.visualstudio.com/docs/?dv=win64user
2. CLANG: install LLVM and make sure you have clang++ as an executable command
   1. https://github.com/llvm/llvm-project/releases/tag/llvmorg-18.1.8 - I recommend [LLVM-18.1.8-win64.exe](https://github.com/llvm/llvm-project/releases/download/llvmorg-18.1.8/LLVM-18.1.8-win64.exe)
3. Doxygen: for generating xml docs, Doxygen config file already included in /build
   1. https://www.doxygen.nl/download.html
4. Python: for generating lua globals from xml docs
   1. https://www.python.org/downloads/ Whatever version you want, I have `3.12.9`