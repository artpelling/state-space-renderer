AUTOMAKE_OPTIONS = subdir-objects

.PHONY: bench bench-json


# DEPENDENCIES
$(srcdir)/benchmark/.git:
	cd $(srcdir) && git submodule update --init --remote benchmark

$(srcdir)/benchmark/build: $(srcdir)/benchmark/.git
	mkdir -p $(srcdir)/benchmark/build

$(srcdir)/benchmark/build/src/libbenchmark.a: $(srcdir)/benchmark/build
	cd $(srcdir)/benchmark/build && \
	cmake -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on ../ && \
	make -j

# STATE SPACE RENDERER
BUILT_SOURCES = $(srcdir)/benchmark/build/src/libbenchmark.a 

bin_PROGRAMS = test_system test_solver test_renderer solver_benchmark system_benchmark blas_benchmark xgemv_benchmark

LDADD = -lblas -lhdf5_serial

AM_CPPFLAGS = $(HDF5_CFLAGS)
AM_LDFLAGS = -std=c++11 -pthread 
if ON_ARM64
AM_CPPFLAGS += -I/Library/Developer/CommandLineTools/SDKs/MacOSX14.2.sdk/System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Headers
AM_LDFLAGS += -framework Accelerate
endif

test_system_SOURCES = tests/test_system.cpp src/state_space_system.cpp src/utils.cpp
test_system_DEPENDENCIES = system_file

test_solver_SOURCES = tests/test_solver.cpp src/state_space_system.cpp src/solver.cpp src/utils.cpp
test_solver_DEPENDENCIES = system_file

test_renderer_SOURCES = tests/test_renderer.cpp src/state_space_system.cpp src/renderer.cpp src/solver.cpp src/utils.cpp
test_renderer_DEPENDENCIES = system_file
test_renderer_LDADD = $(LDADD) -ljack

solver_benchmark_SOURCES = src/solver_benchmark.cpp src/state_space_system.cpp src/solver.cpp src/utils.cpp
solver_benchmark_CPPFLAGS = $(AM_CPPFLAGS) -isystem $(srcdir)/benchmark/include
solver_benchmark_LDFLAGS = $(AM_LDFLAGS) -L$(srcdir)/benchmark/build/src
solver_benchmark_LDADD = $(LDADD) -lbenchmark
solver_benchmark_DEPENDENCIES = $(srcdir)/benchmark/build/src/libbenchmark.a benchmark_files

xgemv_benchmark_SOURCES = src/xgemv_benchmark.cpp src/state_space_system.cpp src/solver.cpp src/utils.cpp
xgemv_benchmark_CPPFLAGS = $(AM_CPPFLAGS) -isystem $(srcdir)/benchmark/include
xgemv_benchmark_LDFLAGS = $(AM_LDFLAGS) -L$(srcdir)/benchmark/build/src
xgemv_benchmark_LDADD = $(LDADD) -lbenchmark
xgemv_benchmark_DEPENDENCIES = $(srcdir)/benchmark/build/src/libbenchmark.a benchmark_files

system_benchmark_SOURCES = src/system_benchmark.cpp src/state_space_system.cpp src/utils.cpp
system_benchmark_CPPFLAGS = $(AM_CPPFLAGS) -isystem $(srcdir)/benchmark/include
system_benchmark_LDFLAGS = $(AM_LDFLAGS) -L$(srcdir)/benchmark/build/src
system_benchmark_LDADD = $(LDADD) -lbenchmark
system_benchmark_DEPENDENCIES = $(srcdir)/benchmark/build/src/libbenchmark.a benchmark_files

blas_benchmark_SOURCES = src/blas_benchmark.cpp src/utils.cpp
blas_benchmark_CPPFLAGS = $(AM_CPPFLAGS) -isystem $(srcdir)/benchmark/include
blas_benchmark_LDFLAGS = $(AM_LDFLAGS) -L$(srcdir)/benchmark/build/src
blas_benchmark_LDADD = $(LDADD) -lbenchmark
blas_benchmark_DEPENDENCIES = $(srcdir)/benchmark/build/src/libbenchmark.a benchmark_files

# PYTHON
VENVDIR = $(builddir)/venv
VENVBIN = $(VENVDIR)/bin/activate
venv: $(srcdir)/systems/requirements.txt
	$(PYTHON) -m venv $(VENVDIR)
	. $(VENVBIN) && pip install -U pip && pip install -r $(srcdir)/systems/requirements.txt
	touch venv

system_file: venv $(srcdir)/systems/create_system.py
	mkdir -p $(builddir)/systems
	. $(VENVBIN) && which python && python $(srcdir)/systems/create_system.py --shape 4 3 5 --format h5
	. $(VENVBIN) && which python && python $(srcdir)/systems/create_system.py --shape 4 3 5 --format npz

benchmark_files: venv $(srcdir)/systems/create_benchmark_systems.py
	mkdir -p $(builddir)/systems/benchmark
	. $(VENVBIN) && python $(srcdir)/systems/create_benchmark_systems.py


# BENCHMARKS
bench: solver_benchmark
	./solver_benchmark

bench-json: solver_benchmark
	./solver_benchmark --benchmark_time_unit=ms --benchmark_format=json | tee benchmark_result.json
	. $(VENVBIN) && python $(srcdir)/parse_json.py

tests: test_system
	./test_system systems/n4p3m5d16doublegeneral.h5 General
	./test_system systems/n4p3m5d16singlegeneral.h5 General
	./test_solver systems/n4p3m5d16doublegeneral.h5 General
#	./test_solver systems/n4p3m5d16singlegeneral.h5 General
#	./test_system systems/n4p3m5d16doubletriangular.h5 Triangular
#	./test_solver systems/n4p3m5d16doubletriangular.h5 Triangular
#	./test_system systems/n4p3m5d16doubletridiagonal.h5 Tridiagonal
#	./test_solver systems/n4p3m5d16doubletridiagonal.h5 Tridiagonal
#	./test_system systems/n4p3m5d16doublediagonal.h5 Diagonal
#	./test_solver systems/n4p3m5d16doublediagonal.h5 Diagonal
#	./test_system systems/n4p3m5d16doublefullhessenberg.h5 FullHessenberg
#	./test_solver systems/n4p3m5d16doublefullhessenberg.h5 FullHessenberg
#	./test_system systems/n4p3m5d16doublemixedhessenberg.h5 MixedHessenberg
#	./test_solver systems/n4p3m5d16doublemixedhessenberg.h5 MixedHessenberg



# CLEAN
clean-system:
	-rm systems/*.npz
	-rm -rf systems/benchmark
clean-local:
	-rm -rf $(VENVDIR)
	-rm systems/*.npz
	-rm -rf systems/benchmark
	-rm -rf $(srcdir)/benchmark
