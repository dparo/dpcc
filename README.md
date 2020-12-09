![GitHub Workflow Status](https://img.shields.io/github/workflow/status/dparo/dpcc/linux-build-test?label=Linux%20Build)
![GitHub Workflow Status](https://img.shields.io/github/workflow/status/dparo/dpcc/windows-build-test?label=Windows%20Build)
![GitHub Workflow Status](https://img.shields.io/github/workflow/status/dparo/dpcc/macos-build-test?label=MacOS%20Build)


![Travis (.org)](https://img.shields.io/travis/dparo/dpcc?label=Travis%20CI%20build)



# dpcc: DParo's own C-alike Compiler frontend

## Cloning

```sh
git clone --recursive https://github.com/dparo/dpcc
```

## Dependencies
- `gcc`
- `make`
- `CMake`
- `python3`
- `Flex`
- `Bison >= 3.6`:
  - Ubuntu 20.10 (Tested):
  ```sh
  sudo apt install bison
  ```
  - Ubuntu 18.04 (Tested), Ubuntu 20.04:
  We need to pull a deb package from the official ubuntu launchpad repos, and then install it
  ```sh
  wget 'https://launchpad.net/ubuntu/+source/bison/2:3.7+dfsg-1/+build/19640339/+files/bison_3.7+dfsg-1_amd64.deb'
  sudo apt install ./bison_3.7+dfsg-1_amd64.deb
  ```

## Building

```sh
mkdir -p build
cd build
cmake ../
make all
```
