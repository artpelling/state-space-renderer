define([AC_CACHE_LOAD],)
define([AC_CACHE_SAVE],)

AC_INIT([state-space-renderer], [0.1], [a.pelling@tu-berlin.de])

# Safety checks in case user overwritten --srcdir
AC_CONFIG_SRCDIR([src])

AC_CONFIG_HEADERS([config.h])
# Store the auxiliary build tools (e.g., install-sh, config.sub, config.guess)
# in this dir (build-aux)
# AC_CONFIG_AUX_DIR([build-aux])

# Init automake, and specify this program use relaxed structures.
# i.e. this program doesn't follow the gnu coding standards, and doesn't have
# ChangeLog, COPYING, AUTHORS, INSTALL, README etc. files.
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# AC_CANONICAL_HOST is needed to access the 'host_os' variable
AC_CANONICAL_HOST

on_x86=no
on_arm=no
AS_CASE([$host_cpu],
  [x86_64], [on_x86=yes],
  [aarch64], [on_arm=yes],
  [AC_MSG_ERROR("No assembler code for CPU $host_cpu")]
)
# Pass the conditionals to automake
AM_CONDITIONAL([ON_ARM64], [test x$on_arm = xyes])
AM_CONDITIONAL([ON_X86_64], [test x$on_x86 = xyes])

# Check for C++ compiler
AC_LANG([C++])
AC_PROG_CC
PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([HDF5], [hdf5 >= 1.8])
PKG_CHECK_MODULES([PIPEWIRE], [libpipewire-0.3 >= 0.3],
  [have_pipewire=yes],
  [AC_MSG_ERROR([PipeWire 0.3 not found. Please install libpipewire-0.3-dev])])

# Export variables to Makefiles
AC_SUBST([PIPEWIRE_CFLAGS])
AC_SUBST([PIPEWIRE_LIBS])

AC_PROG_CXX

AC_CHECK_HEADERS(cblas.h)
AC_CHECK_HEADERS(jack/jack.h)
# We can add more checks in this section

# Tells automake to create a Makefile
# See https://www.gnu.org/software/automake/manual/html_node/Requirements.html
AC_CONFIG_FILES([Makefile])

AM_PATH_PYTHON([3])
AM_CONDITIONAL([HAVE_PYTHON], [test "$PYTHON" != :])
#AC_CONFIG_LINKS([systems/create_system.py:systems/create_system.py])



# Generate the output
AC_OUTPUT
