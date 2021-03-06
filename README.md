
<img src="doc/criterion-title.png" height="96" alt="Criterion Logo" />
=========

[![Unix Build Status](https://travis-ci.org/Snaipe/Criterion.svg?branch=bleeding)](https://travis-ci.org/Snaipe/Criterion) 
[![Windows Build Status](https://ci.appveyor.com/api/projects/status/github/Snaipe/Criterion?svg=true&branch=bleeding)](https://ci.appveyor.com/project/Snaipe/Criterion/branch/bleeding)
[![Coverage Status](https://img.shields.io/codecov/c/github/Snaipe/Criterion/bleeding.svg)](https://codecov.io/github/Snaipe/Criterion?branch=bleeding) 
[![License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](https://github.com/Snaipe/Criterion/blob/master/LICENSE) 
[![Version](https://img.shields.io/github/tag/Snaipe/Criterion.svg?label=version&style=flat)](https://github.com/Snaipe/Criterion/releases) 

![Analytics](https://ga-beacon.appspot.com/UA-68371536-1/Criterion/README.md?pixel)

A dead-simple, yet extensible, C and C++ unit testing framework.

![Screencast](./doc/screencast.gif)

## Philosophy

Most test frameworks for C require a lot of boilerplate code to
set up tests and test suites -- you need to create a main,
then register new test suites, then register the tests within
these suits, and finally call the right functions.

This gives the user great control, at the unfortunate cost of simplicity.

Criterion follows the KISS principle, while keeping the control
the user would have with other frameworks:

* [x] C99 and C++11 compatible.
* [x] Tests are automatically registered when declared.
* [x] Implements a xUnit framework structure.
* [x] A default entry point is provided, no need to declare a main
  unless you want to do special handling.
* [x] Test are isolated in their own process, crashes and signals can be
  reported and tested.
* [x] Unified interface between C and C++: include the criterion header and it *just* works.
* [x] Supports parameterized tests and theories.
* [x] Progress and statistics can be followed in real time with report hooks.
* [x] TAP output format can be enabled with an option.
* [x] Runs on Linux, FreeBSD, Mac OS X, and Windows (Compiling with MinGW GCC and Visual Studio 2015+).

## Downloads

### Packages

* Mac OS X: `brew install snaipe/soft/criterion`
* [AUR](https://aur.archlinux.org/packages/criterion/): `yaourt -S criterion`

### Binary archives

* [Linux (x86_64)](https://github.com/Snaipe/Criterion/releases/download/v2.2.0/criterion-v2.2.0-linux-x86_64.tar.bz2)
* [OS X (x86_64)](https://github.com/Snaipe/Criterion/releases/download/v2.2.0/criterion-v2.2.0-osx-x86_64.tar.bz2)
* [Windows (MSVC - x86_64)](https://github.com/Snaipe/Criterion/releases/download/v2.2.0/criterion-v2.2.0-windows-msvc-x86_64.tar.bz2)
* [Windows (MinGW - x86_64)](https://github.com/Snaipe/Criterion/releases/download/v2.2.0/criterion-v2.2.0-windows-mingw-x86_64.tar.bz2)

[comment]: # (Don't forget to change x86_64 to x64 on windows links on the next release)

If you have a different platform, you can still [build the library from source](http://criterion.readthedocs.org/en/latest/setup.html#installation)

## Developer Resources

### Documentation

An online documentation is available on [ReadTheDocs][online-docs]
([PDF][pdf-docs] | [Zip][zip-docs] | [Epub][epub-docs])

### Samples

Sample tests can be found in the [sample directory][samples].

* [A simple test][sample-simple]
* [Using multiple suites][sample-suites]
* [Writing assertions][sample-asserts]
* [Adding test fixtures][sample-fixtures]
* [Tests with signals][sample-signal]
* [Using report hooks][sample-report]

### Getting help

Gitter.im chat room: [![Join the chat at https://gitter.im/Snaipe/Criterion](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/Snaipe/Criterion?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)  
IRC channel: [#criterion][irc-chan] on irc.freenode.net  

### Misc

* [CMake find module for Criterion][find-module]

## Credits

Logo done by [Greehm](http://www.cargocollective.com/pbouigue)

[online-docs]: http://criterion.readthedocs.org/
[pdf-docs]: http://readthedocs.org/projects/criterion/downloads/pdf/latest/
[zip-docs]: http://readthedocs.org/projects/criterion/downloads/htmlzip/latest/
[epub-docs]: http://readthedocs.org/projects/criterion/downloads/epub/latest/

[samples]: ./samples/
[sample-simple]: ./samples/simple.c
[sample-suites]: ./samples/suites.c
[sample-asserts]: ./samples/asserts.c
[sample-fixtures]: ./samples/fixtures.c
[sample-signal]: ./samples/signal.c
[sample-report]: ./samples/report.c

[find-module]: ./dev/FindCriterion.cmake

[irc-chan]: http://webchat.freenode.net/?channels=%23criterion&uio=MTY9dHJ1ZSYyPXRydWUmOT10cnVlJjExPTE5NQ4e
