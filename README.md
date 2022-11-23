# pl32lib

**NOTICE: This is the old pl32lib codebase. It is unsupported and it's only here as 
a historical archive**

pl32lib is a library containing boilerplate routines that I use for every
project I make. Most of it is strictly C99-compliant and thus fully portable.
The `pl32-term` module is the only component that isn't fully portable. (it 
uses POSIX calls, meaning that while portable to any POSIX system, it won't
compile on everything).

# Build instructions

The library can be built with the command `./compile --build lib`. You can
specify whether to build just the shared lib or just the static lib by appending
`no-shared` or `no-static` to the build command.

To build the test program, you would run `./compile --build test`.

# Bug Reporting

Currently, you can report bugs to GitHub, but I rarely log into GitHub. A better
way of reporting bugs would be to DM me on either Discord or Twitter (it's on my
GitHub profile).
