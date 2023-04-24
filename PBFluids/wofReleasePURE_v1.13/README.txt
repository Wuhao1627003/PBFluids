Welcome to the WOF Point Cloud Mesher v1.13, January 18th, 2023
===============================================================

WOF is a 3D surface reconstruction software. Use it as an
executable OR as a dynamic C++ library.


QUICK START
-----------

You might quickly evaluate WOF using the command line application
in the binaries-folder. Type wof.exe --help for a list of options.

Note: *.PLY files are supported in ASCII and binary form but
the STL reader takes only ASCII STL.


DIRS:
-----

- binaries: Contains the WOF command line application and
  the dynamic WOF libraries for several platforms.

- include: Header files

- doc: PDF documentation. Find the HTML-documentation online:
  https://www.geom.at/wof-point-cloud-mesher-doc/html

- example: Example source code main.cpp to demonstrate how
  to use the dynamic library.


PURE vs. LM
-----------

There are two WOF builds:


- WOF PURE comes without any dependencies and is thought
for smooth integration. A 500 000 points student/eval
license is included.

- WOF LM includes a license manager (LM) and allows an
unlimited number of points during the trial period.



LIBRARY QUICKSTART
------------------

Use the example in ./example/main.cpp. It contains small
and easy to understand functions that use the WOF library.



How to compile the example
==========================

Supported compilers: Windows: VS2015, VS2017, VS2019, Linux: GCC


Compiling under Windows
-----------------------

1. Make sure you have CMake installed. 
2. Change to the example path e.g., cd c:\wofReleasePURE_v1.xx\example\
3. Create a project with one of
  - cmake -B VS2015_x64 -G "Visual Studio 14 2015" -A x64  .
  - cmake -B VS2017_x64 -G "Visual Studio 15 2017" -A x64  .
  - cmake -B VS2019_x64 -G "Visual Studio 16 2019" -A x64  .
4. Open VS20xx_x64 project and compile it in x64 / Release mode


The executable will appear in example\Release\. Type
  - cd Release
  - wof_example.exe


Compiling under Linux: 
----------------------

Use CMake:

1. mkdir example/build, cd exampe/build/
2. cmake ..
3. make

...or use the included classic Makefile (prepared for Ubuntu 22.04)

1. mkdir example/build, cd example/build
2. make -C ..

In any case the resulting executable is example/build/wof_example.
  - Type ./wof_example


Disclaimer
----------
In no case can Geom Software be made responsible for damages of any kind
that arise in connection with the use or non-usability of the software
or information provided on our internet pages. If you can't accept these
terms, you are not allowed to use the software. Using WOF for military
research and applications is not accepted.

If you have any questions, get in contact:


Geom Software
Bernhard Kornberger
https://www.geom.at
bkorn@geom.at

