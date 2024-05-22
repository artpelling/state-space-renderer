![image](ssm.pdf?raw=true)
# Benchmarks

Current benchmarks can be found [here](https://artpelling.github.io/state-space-renderer/benchmarks/)

# Installation

## Dependencies

- autotools
- cmake
- g++

## Compilation

The package can be compiled by running

```shell
    autoreconf -vif
    mkdir build && cd build
    ../configure
    make
```

### Apple silicon

In order to compile against Apple's Accelerate framework, the library path needs to be supplied to the configure script, e.g.:

```shell
mkdir build && cd build
../configure CPPFLAGS=-I/Library/Developer/CommandLineTools/SDKs/MacOSX14.2.sdk/System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Headers
make
```
