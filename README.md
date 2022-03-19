# pl32lib

pl32lib is a library containing boilerplate routines that I use for every
project I make. Most of it is strictly C99-compliant and thus fully portable.
The `pl32-term` module (not yet implemented) is the only component that isn't
fully portable (it uses POSIX calls, meaning that while portable to any POSIX
system, it won't compile on everything).

# Build instructions

**NOTICE: This project is undergoing testing. The only fully functional modules
at this time are `pl32-memory` and `pl32-shell`. The other modules might be
broken, incomplete, and/or buggy**

Just run `./compile` in from the project directory and it will compile
everything, including the test programs. This might change in a later release.

# Bug Reporting

Currently, you can report bugs to GitHub, but I rarely log into GitHub. A better
way of reporting bugs would be to DM me on either Discord or Twitter (it's on my
GitHub profile).
