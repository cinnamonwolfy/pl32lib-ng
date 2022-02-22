# pl32lib

pl32lib is a library containing boilerplate routines that I use for every
project I make. Most of it is strictly C99-compliant and thus fully portable.
The `pl32-term` module is the only component that isn't fully portable (it uses
POSIX calls, meaning that while portable to any POSIX system, it won't compile
on everything).

# Build instructions

**NOTICE: This project doesn't have a build system in place