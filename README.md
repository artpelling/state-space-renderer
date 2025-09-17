![State Space Model](ssm.png)

# Benchmarks

Current benchmarks can be found [here](https://artpelling.github.io/state-space-renderer/benchmarks/)

# Installation

## Dependencies

- autotools

  ```shell
      sudo apt-get install autotools-dev
      sudo apt-get install autoconf
  ```

- cmake

  ```shell
      sudo apt-get install cmake
  ```

- g++
- HDF5 C API

  ```shell
      sudo apt-get install libhdf5-dev
  ```

- Jack Audio Connection Kit

  ```shell
      sudo apt-get install libjack-dev
  ```

- BLAS

  ```shell
      sudo apt-get install libblas-dev
  ```

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

## Structure

We have constructed the following templated base classes to process state space systems:

1. `StateSpaceRenderer`: Defines the discrete linear state space equations,

   $$
   x_{t+1}=Ax_t + Bu_t\\ y_t=Cx_t+Du_t
   $$

   where $A\in\mathbb{R}^{n\times n}, B\in\mathbb{R}^{n\times m},C\in\mathbb{R}^{p\times n},D\in\mathbb{R}^{p\times m}$.

   Inputs are defined by $u_t\in\mathbb{R}^m$ and outputs by $y_t\in\mathbb{R}^p$.

2. `Solver`: Defines the way the discrete linear state space equations are solved.
3. `Renderer`: Defines how the solver is mounted to a sound server for implementation.

The flow of the classes is structured in such a way:

```mermaid
graph TD;
    `StateSpaceRenderer`-->`Solver`;
    `Solver`-->`Renderer`;
```
