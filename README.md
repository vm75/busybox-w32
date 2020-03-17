### Status
This is a fork of [rmyorston/busybox-w32](https://github.com/rmyorston/busybox-w32).
There are some customizations, which will be documented below.
Attempts will be made to keep this up-to-date with upstream changes.

### Building

You need a MinGW compiler and a POSIX environment (so that `make menuconfig` works).  I cross-compile on Linux.  On Fedora or RHEL/CentOS+EPEL installing mingw32-gcc and mingw32-windows-default-manifest (32-bit build) or mingw64-gcc and mingw64-windows-default-manifest (64-bit build) will pull in everything needed.

To start, run `make mingw32_defconfig` or `make mingw64_defconfig`.  You can then customize your build with `make menuconfig`.

In particular you may need to adjust the compiler by going to Busybox Settings -> Build Options -> Cross Compiler Prefix

Then just `make`.

### Limitations
Same as for [rmyorston/busybox-w32](https://github.com/rmyorston/busybox-w32)
