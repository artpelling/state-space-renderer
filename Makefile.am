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

$(srcdir)/cnpy/.git:
	cd $(srcdir) && git submodule update --init --remote cnpy

$(srcdir)/cnpy/build: $(srcdir)/cnpy/.git
	mkdir -p $(srcdir)/cnpy/build

$(srcdir)/cnpy/build/libcnpy.a: $(srcdir)/cnpy/build
	cd $(srcdir)/cnpy/build && \
	cmake -DCMAKE_POSITION_INDEPENDENT_CODE=off ../ && \
	make -j


# STATE SPACE RENDERER
BUILT_SOURCES = $(srcdir)/benchmark/build/src/libbenchmark.a $(srcdir)/cnpy/build/libcnpy.a

bin_PROGRAMS = test_system test_solver solver_benchmark system_benchmark

LDADD = -lblas -l:libcnpy.a -lz

AM_CPPFLAGS =
AM_LDFLAGS = -std=c++11 -pthread -L$(srcdir)/cnpy/build/
if ON_ARM64
AM_CPPFLAGS += -I/Library/Developer/CommandLineTools/SDKs/MacOSX14.2.sdk/System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Headers
AM_LDFLAGS += -framework Accelerate
endif

test_system_SOURCES = tests/test_system.cpp src/state_space_system.cpp
test_system_DEPENDENCIES = $(srcdir)/cnpy/build/libcnpy.a system_file

test_solver_SOURCES = tests/test_solver.cpp src/state_space_system.cpp src/solver.cpp src/utils.cpp
test_solver_DEPENDENCIES = $(srcdir)/cnpy/build/libcnpy.a system_file

solver_benchmark_SOURCES = src/solver_benchmark.cpp src/state_space_system.cpp src/solver.cpp src/utils.cpp
solver_benchmark_CPPFLAGS = $(AM_CPPFLAGS) -isystem $(srcdir)/benchmark/include
solver_benchmark_LDFLAGS = $(AM_LDFLAGS) -L$(srcdir)/benchmark/build/src
solver_benchmark_LDADD = $(LDADD) -lbenchmark
solver_benchmark_DEPENDENCIES = $(srcdir)/benchmark/build/src/libbenchmark.a $(srcdir)/cnpy/build/libcnpy.a benchmark_files

system_benchmark_SOURCES = src/system_benchmark.cpp src/state_space_system.cpp src/utils.cpp
system_benchmark_CPPFLAGS = $(AM_CPPFLAGS) -isystem $(srcdir)/benchmark/include
system_benchmark_LDFLAGS = $(AM_LDFLAGS) -L$(srcdir)/benchmark/build/src
system_benchmark_LDADD = $(LDADD) -lbenchmark
system_benchmark_DEPENDENCIES = $(srcdir)/benchmark/build/src/libbenchmark.a $(srcdir)/cnpy/build/libcnpy.a benchmark_files


# PYTHON
VENVDIR = $(builddir)/venv
VENVBIN = $(VENVDIR)/bin/activate
venv: $(srcdir)/systems/requirements.txt
	$(PYTHON) -m venv $(VENVDIR)
	. $(VENVBIN) && pip install -U pip && pip install -r $(srcdir)/systems/requirements.txt
	touch venv

system_file: venv $(srcdir)/systems/create_system.py
	mkdir -p $(builddir)/systems
	. $(VENVBIN) && which python && python $(srcdir)/systems/create_system.py

benchmark_files: venv $(srcdir)/systems/create_benchmark_systems.py
	mkdir -p $(builddir)/systems/benchmark
	. $(VENVBIN) && python $(srcdir)/systems/create_benchmark_systems.py


# BENCHMARKS
bench: solver_benchmark
	./solver_benchmark

bench-json: solver_benchmark
	./solver_benchmark --benchmark_format=json | tee benchmark_result.json

tests: test_system test_solver system_file
	./test_solver systems/n10p2m5d16double.npz
	./test_system systems/n10p2m5d16double.npz


# CLEAN
clean-local:
	-rm -rf $(VENVDIR)
	-rm systems/*.npz
	-rm -rf systems/benchmark
	-rm -rf $(srcdir)/benchmark
	-rm -rf $(srcdir)/cnpy
