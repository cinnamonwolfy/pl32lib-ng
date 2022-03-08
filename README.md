# pl32lib

pl32lib is a library containing boilerplate routines that I use for every
project I make. Most of it is strictly C99-compliant and thus fully portable.
The `pl32-term` module (not yet implemented) is the only component that isn't
fully portable (it uses POSIX calls, meaning that while portable to any POSIX
system, it won't compile on everything).

# Build instructions

**NOTICE: This project doesn't have a build system in place, you will need to
build it manually. This will only cover Unix-like/Cygwin systems with the GCC
compiler.

In addition to this, pl32lib is undergoing testing. Currently, `pl32-memory`
and `pl32-shell` are the only confirmed fully functional modules. Other modules
might be broken or buggy.**

Open a terminal (if on a graphical environment), and run the following from the
project directory:

```
cd src
# For shared
gcc -c -fPIC -I../include pl32-memory.c pl32-shell.c pl32-file.c
gcc -shared *.o -o libpl32.so
# For static
gcc -c -I../include pl32-memory.c pl32-shell.c pl32-file.c
ar rc libpl32.a *.o
```

# Bug Reporting

Currently, you can report bugs to GitHub, but I rarely log into GitHub. A better
way of reporting bugs would be to DM me on either Discord or Twitter (it's on my
GitHub profile).
