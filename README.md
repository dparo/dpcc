![GitHub](https://img.shields.io/github/license/dparo/dpcc)
![GitHub Workflow Status](https://img.shields.io/github/workflow/status/dparo/dpcc/linux-build-test?label=Linux%20Build)
![GitHub Workflow Status](https://img.shields.io/github/workflow/status/dparo/dpcc/macos-build-test?label=MacOS%20Build)


# dpcc: DParo's own C-alike Compiler frontend

![Example of running a DPL script](./gifs/showoff.gif)

## Docs

Read the project report [here](https://github.com/dparo/dpcc/releases/download/1.0.1/DPL_ProjectAssignmentReport.pdf).

For **more details** refer to the [Project Wiki](https://github.com/dparo/dpcc/wiki)


## Building

- `CMake >= 3.11`
- `Flex`
- `Bison >= 3.6`
- `GCC` or `Clang`

```sh
git clone https://github.com/dparo/dpcc
cd dpcc
mkdir -p build
cd build
cmake ../
cmake --build ./
```
