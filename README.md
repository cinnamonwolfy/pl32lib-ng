# pl32lib-ng

pl32lib was a library containing boilerplate routines that I use for every
project I made. pl32lib-ng is both a rewrite and relicensing of both the build
system and the library itself. It builds C99-only modules by default, but it can
be made

# Build instructions

To build the pl32lib-ng libraries, you first run `./configure` and then you run
`./compile --build lib` to compile both the static and shared libraries. If you
want to only compile the static or shared libraries, run
`./compile --build static_lib` and `./compile --build shared_lib` respectively.

To build the test program, run `./compile --build test`.

# Bug Reporting

Bug reporting can be done by going to the repo's issues page and opening a new
issue.
