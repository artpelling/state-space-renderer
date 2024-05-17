AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = test_system test_solver benchmark_system benchmark_solver

LDADD = -lcnpy -lblas -lpthread
AM_LDFLAGS = -std=c++11

test_system_SOURCES = tests/test_system.cpp src/state_space_system.cpp
test_solver_SOURCES = tests/test_solver.cpp src/state_space_system.cpp src/solver.cpp src/utils.cpp

if ON_ARM64
AM_CPPFLAGS = -I/Library/Developer/CommandLineTools/SDKs/MacOSX14.2.sdk/System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Headers
AM_LDFLAGS += -framework Accelerate -rpath /usr/local/lib/
endif

benchmark_system_SOURCES = benchmark/benchmark_runtime_system.cpp src/state_space_system.cpp
benchmark_solver_SOURCES = benchmark/benchmark_runtime_solver.cpp src/state_space_system.cpp src/solver.cpp src/utils.cpp


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

clean-local:
	-rm -rf $(VENVDIR)
	-rm systems/*.npz
	-rm -rf systems/benchmark


benchmarks: benchmark_system benchmark_solver benchmark_files
	./benchmark_solver
	./benchmark_system

tests: test_system test_solver system_file
	./test_solver systems/n10p2m5d16double.npz
	./test_system systems/n10p2m5d16double.npz
