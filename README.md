# pl32lib-ng

pl32lib was a library containing boilerplate routines that I use for every
project I made. pl32lib-ng is both a rewrite and relicensing of both the build
system and the library itself. It builds C99-only modules by default, but it
can be configured to compile platform-specific modules if it's supported

# Build instructions

There are two ways to build pl32lib-ng: By using Meson or by running 
`./configure && ./compile`

## Meson

**This is the recommended way to build pl32lib-ng**

To build using Meson, create an empty build folder within the project root, and
then run `meson setup` and `meson compile`:
```sh
cd $PL32_DIR
mkdir build
meson setup
meson compile
```

To install the library and headers, run `meson install`

## `./configure` and `./compile`

**NOTICE: Only use this one for experimental features**

To build the pl32lib-ng libraries, you first run `./configure` and then you run
`./compile` to compile both the static and shared libraries. If you want to
only compile the static or shared libraries, run `./compile --build static_lib`
and `./compile --build shared_lib` respectively.

To build the test program, run `./compile --build test`.

To install the library and headers, run `./compile --install`

# Bug Reporting

Bug reporting can be done by going to the repo's issues page and opening a new
issue.
