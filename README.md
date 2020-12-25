![GitHub](https://img.shields.io/github/license/dparo/dpcc)
![GitHub Workflow Status](https://img.shields.io/github/workflow/status/dparo/dpcc/linux-build-test?label=Linux%20Build)
![GitHub Workflow Status](https://img.shields.io/github/workflow/status/dparo/dpcc/macos-build-test?label=MacOS%20Build)


# dpcc: DParo's own C-alike Compiler frontend

![Example of running a DPL script](./gifs/showoff.gif)



## Building

- `Linux` or `MacOS`
- `GCC` or `Clang`
- `CMake >= 3.11`
- `Flex`
- `Bison >= 3.6`

```sh
git clone --depth 1 https://github.com/dparo/dpcc
cd dpcc
mkdir -p build
cd build
cmake ../
cmake --build ./
```

## Docs

- [Flex File](https://github.com/dparo/dpcc/blob/main/src/lexer.l)
- [Bison parser / grammar file](https://github.com/dparo/dpcc/blob/main/src/parser.y)
- [Read the project report here](https://github.com/dparo/dpcc/releases/download/1.0.1/DPL_ProjectAssignmentReport.pdf).

For **more details** refer to the [Project Wiki](https://github.com/dparo/dpcc/wiki)
