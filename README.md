gem5-NVP
===
The gem5-NVP is a simulating framework for non-volatile processors (NVP) and non-volatile systems based on the gem5 simulator. It provides multiple interfaces for adding description of SimObjects' energy behavior as well as a energy management module which allows users to simulate state changes (power-on, power-off, etc.) easily.

## Pre-Requirement
gem5-NVP depends on nothing besides the dependencies of gem5 simulator. Tools needed before building gem5-NVP can be found at http://www.gem5.org/Dependencies.

## Download
gem5-NVP uses github to control its version. The latest stable version can be found at https://github.com/zlfben/gem5:master, and there are previous versions among tags of the repository. You can get the latest stable version like this:
```Bash
git clone https://github.com/zlfben/gem5.git
cd gem5
git checkout master
```

## Build
The build process of gem5-NVP is exactly the same as gem5 simulator. Scons is used to build target system to be simulated. Theoretically, different ISAs are supported by gem5-NVP. However, gem5-NVP is tested under ARM, so the following build option is recommended:
```Bash
scons build/ARM/gem.debug
```

## Run

## Usages


gem5 README
===
This is the gem5 simulator.

The main website can be found at http://www.gem5.org

A good starting point is http://www.gem5.org/Introduction, and for
more information about building the simulator and getting started
please see http://www.gem5.org/Documentation and
http://www.gem5.org/Tutorials.

To build gem5, you will need the following software: g++ or clang,
Python (gem5 links in the Python interpreter), SCons, SWIG, zlib, m4,
and lastly protobuf if you want trace capture and playback
support. Please see http://www.gem5.org/Dependencies for more details
concerning the minimum versions of the aforementioned tools.

Once you have all dependencies resolved, type 'scons
build/<ARCH>/gem5.opt' where ARCH is one of ALPHA, ARM, NULL, MIPS,
POWER, SPARC, or X86. This will build an optimized version of the gem5
binary (gem5.opt) for the the specified architecture. See
http://www.gem5.org/Build_System for more details and options.

With the simulator built, have a look at
http://www.gem5.org/Running_gem5 for more information on how to use
gem5.

The basic source release includes these subdirectories:
   - configs: example simulation configuration scripts
   - ext: less-common external packages needed to build gem5
   - src: source code of the gem5 simulator
   - system: source for some optional system software for simulated systems
   - tests: regression tests
   - util: useful utility programs and files

To run full-system simulations, you will need compiled system firmware
(console and PALcode for Alpha), kernel binaries and one or more disk
images. Please see the gem5 download page for these items at
http://www.gem5.org/Download

If you have questions, please send mail to gem5-users@gem5.org

Enjoy using gem5 and please share your modifications and extensions.
